#include "Material.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "glm/gtc/type_ptr.hpp"
#include "Graphic/Lighting/PointLight.h"


#include "Shader.h"

Material::~Material()
{
	glDeleteProgram(program);
}

Material::Material(
	const GLchar *_name,
	const Shader * vertexShader,
	const Shader * fragmentShader,
	const Shader * geometryShader,
	const Shader * tessEvaluationShader,
	const Shader * tessControlShader) : name(_name)
{

    AssembleProgram(vertexShader, fragmentShader, geometryShader, tessControlShader, tessControlShader);
}

void Material::setLightingParameters(std::vector<PointLight> & lights)
{
 
    GLuint index = 0;
    for(PointLight &light : lights)
    {
        glUniform3fv(glGetUniformLocation(program, ("pointLights[" + std::to_string(index) + "].position").c_str()), 1, glm::value_ptr(light.position));
        glUniform3fv(glGetUniformLocation(program, ("pointLights[" + std::to_string(index) + "].color").c_str()), 1, glm::value_ptr(light.color));
        
        ++index;
    }

    glUniform1i(glGetUniformLocation(program, NUMBER_OF_LIGHTS_NAME), lights.size());
}

void Material::setCameraParameters(Camera &camera)
{
    glUniformMatrix4fv(glGetUniformLocation(program, VIEW_MATRIX_NAME), 1, GL_FALSE, glm::value_ptr(camera.viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, PROJECTION_MATRIX_NAME), 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
    glUniform3fv(glGetUniformLocation(program, CAMERA_POSITION_NAME), 1, glm::value_ptr(camera.position));
}

void Material::Activate(MaterialSetting::SettingsGroup& group, std::vector<PointLight>& lights, Camera& camera)
{
    //TODO: glUseProgram MIGHT BE A PERFORMANCE BOTTLENECK, DO PROFILE
    glUseProgram(program);
    
    setLightingParameters(lights);
    setCameraParameters(camera);
    uploadRenderingSettings();
    
    for (std::pair<const GLchar* , MaterialSetting > pair : group)
    {
        const GLchar* name = pair.first;
        MaterialSetting setting = pair.second;
        setValue(setting, name);
    }
    

}

void Material::uploadRenderingSettings()
{
    glUniform1i(glGetUniformLocation(program, "settings.shadows"), shadows);
    glUniform1i(glGetUniformLocation(program, "settings.indirectDiffuseLight"), indirectDiffuseLight);
    glUniform1i(glGetUniformLocation(program, "settings.indirectSpecularLight"), indirectSpecularLight);
    glUniform1i(glGetUniformLocation(program, "settings.directLight"), directLight);
}

void Material::setValue(MaterialSetting &setting, const GLchar* name)
{
    assert(setting.getType() != MaterialSetting::Type::NONE);
    
    switch (setting.getType()) {
        case MaterialSetting::Type::VEC4 :
        {
            
            glm::vec4 value = setting.getVec4Value();
            SetParameterv4(name, value);
            break;
        }
        case MaterialSetting::Type::VEC3:
        {
            glm::vec3 value = setting.getVec3Value();
            SetParameterv3(name, value);
            break;
        }
        case MaterialSetting::Type::VEC2:
        {
            glm::vec2 value = setting.getVec2Value();
            SetParameterv2(name, value);
            break;
        }
        case MaterialSetting::Type::FLOAT:
        {
            GLfloat value = setting.getFloatValue();
            SetParameterf(name, value);
            break;
        }
        case MaterialSetting::Type::INT:
        {
            GLint value = setting.getIntValue();
            SetParameteri(name, value);
            break;
        }
        case MaterialSetting::Type::SAMPLER:
        {
            assert(false);
        }
            
        default:
            break;
    }
}

void Material::AssembleProgram(
                              const Shader * vertexShader,
                              const Shader * fragmentShader,
                              const Shader * geometryShader,
                              const Shader * tessEvaluationShader,
                              const Shader * tessControlShader
                            )
{
    assert(vertexShader != nullptr);
    assert(fragmentShader != nullptr);
    
    GLuint vertexShaderID, fragmentShaderID, geometryShaderID, tessEvaluationShaderID, tessControlShaderID;
    program = glCreateProgram();
    
    // Vertex shader.
    assert(vertexShader->shaderType == Shader::ShaderType::VERTEX);
    vertexShaderID = vertexShader->ShaderID();
    glAttachShader(program, vertexShaderID);
    
    // Fragment shader.
    assert(fragmentShader->shaderType == Shader::ShaderType::FRAGMENT);
    fragmentShaderID = fragmentShader->ShaderID();
    glAttachShader(program, fragmentShaderID);
    
    // Geometry shader.
    if (geometryShader != nullptr) {
        assert(geometryShader->shaderType == Shader::ShaderType::GEOMETRY);
        geometryShaderID = geometryShader->ShaderID();
        glAttachShader(program, geometryShaderID);
    }
    
    // Tesselation evaluation shader.
    if (tessEvaluationShader != nullptr) {
        assert(tessEvaluationShader->shaderType == Shader::ShaderType::TESSELATION_EVALUATION);
        tessEvaluationShaderID = tessEvaluationShader->ShaderID();
        glAttachShader(program, tessEvaluationShaderID);
    }
    
    // Tesselation control shader.
    if (tessControlShader != nullptr) {
        assert(tessControlShader->shaderType == Shader::ShaderType::TESSELATION_CONTROL);
        tessControlShaderID = tessControlShader->ShaderID();
        glAttachShader(program, tessControlShaderID);
    }
    
    glLinkProgram(program);
    
    // Check if we succeeded.
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar log[1024];
        glGetProgramInfoLog(program, 1024, nullptr, log);
        std::cerr << "- Failed to link program and material '" << name << "' (" << program << ")." << std::endl;
        std::cerr << "LOG: " << std::endl << log << std::endl;
    }
    else {
        std::cout << "- Material '" << name << "' (program " << program << ") sucessfully created." << std::endl;
    }
}


