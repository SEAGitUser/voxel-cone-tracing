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
	const ShaderSharedPtr& vertexShader,
	const ShaderSharedPtr& fragmentShader,
	const ShaderSharedPtr& geometryShader,
	const ShaderSharedPtr& tessEvaluationShader,
	const ShaderSharedPtr& tessControlShader) : name(_name)
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

void Material::uploadGPUParameters(Scene& scene)
{
    glUseProgram(program);
    
    setLightingParameters(scene.pointLights);
    setCameraParameters(*scene.renderingCamera);
}
void Material::uploadGPUParameters(ShaderParameter::ShaderParamsGroup& group, Scene& scene)
{
    glUseProgram(program);
    
    uploadGPUParameters(scene);
    uploadRenderingSettings();
    uploadGPUParameters(group);
}

void Material::uploadGPUParameters(ShaderParameter::ShaderParamsGroup &group)
{
    glUseProgram(program);
    
    for (std::pair<const GLchar* , ShaderParameter > pair : group)
    {
        const GLchar* name = pair.first;
        ShaderParameter setting = pair.second;
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

void Material::setValue(ShaderParameter &setting, const GLchar* name)
{
    assert(setting.getType() != ShaderParameter::Type::NONE);
    
    GLint result = 0;
    //TODO: let's use virtual functions and templates for this instead
    switch (setting.getType())
    {
        case ShaderParameter::Type::MAT4 :
        {
            glm::mat4 value = setting.getMat4Value();
            result = SetParamatermat4(name, value);
            break;
        }
        case ShaderParameter::Type::VEC4 :
        {
            glm::vec4 value = setting.getVec4Value();
            result = SetParameterv4(name, value);
            break;
        }
        case ShaderParameter::Type::VEC3:
        {
            glm::vec3 value = setting.getVec3Value();
            result = SetParameterv3(name, value);
            break;
        }
        case ShaderParameter::Type::VEC2:
        {
            glm::vec2 value = setting.getVec2Value();
            result = SetParameterv2(name, value);
            break;
        }
        case ShaderParameter::Type::FLOAT:
        {
            GLfloat value = setting.getFloatValue();
            result = SetParameterf(name, value);
            break;
        }
        case ShaderParameter::Type::INT:
        {
            GLint value = setting.getIntValue();
            result = SetParameteri(name, value);
            break;
        }
        
        case ShaderParameter::Type::UINT:
        {
            GLuint value = setting.getUnsignedInt();
            result = SetParameterui(name, value);
            break;
        }
            
        case ShaderParameter::Type::SAMPLER_2D:
        {
            ShaderParameter::Sampler2D sampler = setting.getSampler2DValue();
            result = SetParameterSampler2D(name, sampler);
            break;
        }
        case ShaderParameter::Type::SAMPLER_3D:
        {
            ShaderParameter::Sampler3D sampler = setting.getSampler3DValue();
            result = SetParameterSampler3D(name, sampler);
            break;
        }
        case ShaderParameter::Type::POINT_LIGHT:
        {
            PointLight &light = setting.getPointLightValue();
            result = SetPointLight(name, light);
            break;
        }
        default:
        {
            assert(false && "unrecognized setting for shader" );
            break;
        }
    }
    
    glError();
    if(result == -1)
    {
        printf("WARNING: parameter %s was not found for material %s\n", name, this->name);
    }
}

void Material::AssembleProgram(
                              const ShaderSharedPtr& vertexShader,
                              const ShaderSharedPtr& fragmentShader,
                              const ShaderSharedPtr& geometryShader,
                              const ShaderSharedPtr& tessEvaluationShader,
                              const ShaderSharedPtr& tessControlShader
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


