#include "Material.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "glm/gtc/type_ptr.hpp"
#include "Graphic/Lighting/PointLight.h"


#include "Shader.h"


const char * const Material::Commands::PROJECTION_MATRIX_NAME = "P";
const char * const Material::Commands::VIEW_MATRIX_NAME = "V";
const char * const Material::Commands::CAMERA_POSITION_NAME = "cameraPosition";
const char * const Material::Commands::NUMBER_OF_LIGHTS_NAME = "numberOfLights";
const char * const Material::Commands::MODEL_MATRIX_NAME = "M";
const char * const Material::Commands::SCREEN_SIZE_NAME = "screenSize";
const char * const Material::Commands::APP_STATE_NAME = "state";

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
    
    unsigned int vertexShaderID, fragmentShaderID, geometryShaderID, tessEvaluationShaderID, tessControlShaderID;
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
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar log[1024];
        glGetProgramInfoLog(program, 1024, nullptr, log);
        std::cerr << "- Failed to link program and material '" << name << "' (" << program << ")." << std::endl;
        std::cerr << "LOG: " << std::endl << log << std::endl;
    }
    else {
        std::cout << "- Material '" << name << "' (program " << program << ") sucessfully created." << std::endl;
    }
}


///Comands

Material::Commands::Commands(Material* _material):
textureUnits(0)
{
    material = _material;
    glUseProgram(material->program);
}

void Material::Commands::setValue(ShaderParameter &setting, const GLchar* name)
{
    assert(setting.getType() != ShaderParameter::Type::NONE);
    glError();
    int result = 0;
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
            float value = setting.getFloatValue();
            result = SetParameterf(name, value);
            break;
        }
        case ShaderParameter::Type::INT:
        {
            int value = setting.getIntValue();
            result = SetParameteri(name, value);
            break;
        }
        case ShaderParameter::Type::BOOLEAN:
        {
            bool value = setting.getBoolValue();
            result = SetParameterBool(name, value);
            break;
        }
            
        case ShaderParameter::Type::UINT:
        {
            unsigned int value = setting.getUnsignedInt();
            result = SetParameterui(name, value);
            break;
        }
            
        case ShaderParameter::Type::SAMPLER_2D:
        {
            Texture2D* texture = setting.getSampler2DValue();
            result = SetParameterSampler2D(name, texture);
            break;
        }
        case ShaderParameter::Type::SAMPLER_3D:
        {
            Texture3D* texture = setting.getSampler3DValue();
            result = SetParameterSampler3D(name, texture);
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
        printf("WARNING: parameter %s was not found for material %s\n", name, material->name);
    }
}


int Material::Commands::SetParameteri(const GLchar* parameterName, int const value)
{
    int location = glGetUniformLocation(material->program, parameterName);
    glUniform1i(location, value);
    return location;
}

int Material::Commands::SetParameterui(const GLchar* parameterName, unsigned int const value)
{
    unsigned int location = glGetUniformLocation(material->program, parameterName);
    glUniform1ui(location, value);
    return location;
}

int Material::Commands::SetParameterf(const GLchar* parameterName, float const value)
{
    int location = glGetUniformLocation(material->program, parameterName);
    glUniform1f(location,(value));
    return location;
}

int Material::Commands::SetParameterv4(const GLchar* parameterName, const glm::vec4 &value)
{
    int location = glGetUniformLocation(material->program, parameterName);
    glUniform4fv(location, 1, glm::value_ptr(value));
    return location;
}

int Material::Commands::SetParameterv3(const GLchar *parameterName, const glm::vec3 &value)
{
    int location = glGetUniformLocation(material->program, parameterName);
    glUniform3fv(location, 1, glm::value_ptr(value));
    return location;
}

int Material::Commands::SetParameterv2(const GLchar *parameterName, const glm::vec2 &value)
{
    int location = glGetUniformLocation(material->program, parameterName);
    glUniform2fv(location, 1, glm::value_ptr(value));
    return location;
}

int Material::Commands::SetParamatermat4(const GLchar* parameterName, const glm::mat4 &value)
{
    int location = glGetUniformLocation(material->program, parameterName);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    return location;
}

int Material::Commands::SetParameterSampler2D(const GLchar* parameterName, Texture2D* texture)
{
    int result = ActivateTexture2D(parameterName, texture->GetTextureID() , textureUnits);
    return result;
}

int Material::Commands::SetParameterSampler3D(const GLchar* parameterName, Texture3D* texture)
{
    int result = ActivateTexture3D(parameterName, texture->GetTextureID() , textureUnits);
    return result;
}


int Material::Commands::SetPointLight(const GLchar *parameterName, const PointLight &light)
{
    int location = glGetUniformLocation(material->program, ("pointLights[" + std::to_string(light.index) + "].position").c_str());
    glUniform3fv(location, 1, glm::value_ptr(light.position));
    location = location != -1 ?  glGetUniformLocation(material->program, ("pointLights[" + std::to_string(light.index) + "].color").c_str()) : location;
    glUniform3fv(location, 1, glm::value_ptr(light.color));
    return location;
}

int Material::Commands::ActivateTexture2D(const GLchar* samplerName, const int textureName, const int textureUnit)
{
    assert(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS > textureUnit);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureName);
    int location = glGetUniformLocation(material->program, samplerName);
    glUniform1i(location, textureUnit);
    
    return location;
}

int Material::Commands::SetParameterBool(const GLchar *parameterName, bool value)
{
    int result = glGetUniformLocation(material->program, parameterName);
    glUniform1i(result, value);
    return result;
}

int Material::Commands::ActivateTexture2D(const GLchar* samplerName, const Texture2D* texture, const int textureUnit)
{
    int result = ActivateTexture2D(samplerName, texture->GetTextureID(), textureUnit);
    return result;
}

int Material::Commands::ActivateTexture3D(const GLchar* samplerName, const Texture3D* texture, const int textureUnit)
{
    int result = ActivateTexture3D(samplerName, texture->GetTextureID(), textureUnit);
    return result;
}


int Material::Commands::ActivateTexture3D(const GLchar* samplerName, const int textureName, const int textureUnit)
{
    assert(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS > textureUnit);
    //bind texture to texture unit
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_3D, textureName);
    
    //bind shader uniform location to texture unit
    int location = glGetUniformLocation(material->program, samplerName);
    glUniform1i(location, textureUnit);
    return location;
}

int Material::Commands::SetMatrix(const GLchar* parameterName, const glm::mat4& mat)
{
    int location = glGetUniformLocation(material->program, parameterName);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    return location;
}

void Material::Commands::uploadParameters(ShaderParameter::ShaderParamsGroup &group)
{
    textureUnits = 0;
    for (std::pair<const GLchar* , ShaderParameter > pair : group)
    {
        const GLchar* name = pair.first;
        ShaderParameter setting = pair.second;
        glError();
        setValue(setting, name);
        glError();
        textureUnits += (setting.getType() == ShaderParameter::Type::SAMPLER_2D ||
                         setting.getType() == ShaderParameter::Type::SAMPLER_3D ) ? 1:0;
    }
    
    textureUnits = 0;
}

Material::Commands::~Commands()
{
    glUseProgram(0);
}


