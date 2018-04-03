#pragma once

#include "OpenGL_Includes.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Graphic/Material/Resource.h"
#include "Graphic/Material/ShaderParameter.h"
#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Camera/Camera.h"
#include "Graphic/Lighting/PointLight.h"
#include "Scene/Scene.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Shader.h"

class Scene;

/// <summary> Represents a material that references a gl program, textures and settings. </summary>
class Material : public Resource {
public:

    //TODO: YOU'LL NEED TO IMPLEMENT A COMMAND CLASS TO  ABSTRACT API CALLS AWAY FROM
    //RENDERING ENGINE CODE
    class Commands
    {
    public:
        
    private:
    };
    
	Material(const GLchar *_name,
		const ShaderSharedPtr& vertexShader,
		const ShaderSharedPtr& fragmentShader,
		const ShaderSharedPtr& geometryShader = nullptr,
		const ShaderSharedPtr& tessEvaluationShader = nullptr,
		const ShaderSharedPtr& tessControlShader = nullptr);

    Material(const GLchar *name): name(name){}
    
    virtual void uploadGPUParameters(ShaderParameter::ShaderParamsGroup& settingsGroup, Scene& scene);
    void uploadGPUParameters(ShaderParameter::ShaderParamsGroup& settingsGroup);
    void uploadGPUParameters(Scene& scene);
    
    inline GLint SetParameteri(const GLchar* parameterName, GLint const value);
    inline GLint SetParameterui(const GLchar* parameterName, GLuint const value);
    inline GLint SetParameterv4(const GLchar* parameterName, const glm::vec4 &value);
    inline GLint SetParameterv3(const GLchar* parameterName, const glm::vec3 & value);
    inline GLint SetParameterv2(const GLchar* parameterName, const glm::vec2 & value);
    inline GLint SetParameterf(const GLchar* parameterName, GLfloat const value);
    inline GLint SetParamatermat4(const GLchar *parameterName, const glm::mat4 &value);
    inline GLint SetParameterSampler2D(const GLchar* parameterName, const ShaderParameter::Sampler2D& sampler);
    inline GLint SetParameterSampler3D(const GLchar* parameterName, const ShaderParameter::Sampler3D& sampler);
    inline GLint SetPointLight(const GLchar* parameterName,  const PointLight& light  );
    
    inline GLint ActivateTexture2D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit);
    inline GLint ActivateTexture3D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit);
    inline GLint ActivateTexture2D(const GLchar* samplerName, const Texture2D* texture, const GLint textureUnit);
    inline GLint ActivateTexture3D(const GLchar* samplerName, const Texture3D* texture, const GLint textureUnit);
    
    
    inline GLint SetModelMatrix(const glm::mat4& mat);
    inline void Deactivate(){ glUseProgram(0) ;}
    
    virtual ~Material();
    
public:
    const char * const PROJECTION_MATRIX_NAME = "P";
    const char * const VIEW_MATRIX_NAME = "V";
    const char * const CAMERA_POSITION_NAME = "cameraPosition";
    const char * const NUMBER_OF_LIGHTS_NAME = "numberOfLights";
    const char * const MODEL_MATRIX_NAME = "M";
    const char * SCREEN_SIZE_NAME = "screenSize";
    const char * APP_STATE_NAME = "state";
    
    bool shadows = true;
    bool indirectDiffuseLight = true;
    bool indirectSpecularLight = true;
    bool directLight = true;
    
    
protected:
    
    void setLightingParameters(std::vector<PointLight> & lights);
    void setCameraParameters(Camera&);
    void uploadRenderingSettings();
    void uploadGlobalConstants();
    
    void AssembleProgram(
                        const ShaderSharedPtr& vertexShader,
                        const ShaderSharedPtr& fragmentShader,
                        const ShaderSharedPtr& geometryShader,
                        const ShaderSharedPtr& tessEvaluationShader,
                        const ShaderSharedPtr& tessControlShader
                        );
    
    void setValue(ShaderParameter& setting, const GLchar* name);
    
    /// <summary> The actual OpenGL / GLSL program identifier. </summary>
    GLuint program;

public:


	/// <summary> A name. Just an identifier. Doesn't do anything practical. </summary>
	const GLchar* name;
};

GLint Material::SetParameteri(const GLchar* parameterName, GLint const value)
{
    GLint location = glGetUniformLocation(program, parameterName);
    glUniform1i(location, value);
    return location;
}

GLint Material::SetParameterui(const GLchar* parameterName, GLuint const value)
{
    GLuint location = glGetUniformLocation(program, parameterName);
    glUniform1ui(location, value);
    return location;
}

GLint Material::SetParameterf(const GLchar* parameterName, GLfloat const value)
{
    GLint location = glGetUniformLocation(program, parameterName);
    glUniform1f(location,(value));
    return location;
}

GLint Material::SetParameterv4(const GLchar* parameterName, const glm::vec4 &value)
{
    GLint location = glGetUniformLocation(program, parameterName);
    glUniform4fv(location, 1, glm::value_ptr(value));
    return location;
}

GLint Material::SetParameterv3(const GLchar *parameterName, const glm::vec3 &value)
{
    GLint location = glGetUniformLocation(program, parameterName);
    glUniform3fv(location, 1, glm::value_ptr(value));
    return location;
}

GLint Material::SetParameterv2(const GLchar *parameterName, const glm::vec2 &value)
{
    GLint location = glGetUniformLocation(program, parameterName);
    glUniform2fv(location, 1, glm::value_ptr(value));
    return location;
}

GLint Material::SetParamatermat4(const GLchar* parameterName, const glm::mat4 &value)
{
    GLint location = glGetUniformLocation(program, parameterName);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    return location;
}

GLint Material::SetParameterSampler2D(const GLchar* parameterName, const ShaderParameter::Sampler2D& sampler)
{
    GLint result = ActivateTexture2D(parameterName, sampler.texture->GetTextureID() , sampler.textureUnit);
    return result;
}

GLint Material::SetParameterSampler3D(const GLchar* parameterName, const ShaderParameter::Sampler3D& sampler)
{
    GLint result = ActivateTexture3D(parameterName, sampler.texture->GetTextureID() , sampler.textureUnit);
    return result;
}


GLint Material::SetPointLight(const GLchar *parameterName, const PointLight &light)
{
    GLint location = glGetUniformLocation(program, ("pointLights[" + std::to_string(light.index) + "].position").c_str());
    glUniform3fv(location, 1, glm::value_ptr(light.position));
    location = location != -1 ?  glGetUniformLocation(program, ("pointLights[" + std::to_string(light.index) + "].color").c_str()) : location;
    glUniform3fv(location, 1, glm::value_ptr(light.color));
    return location;
}

GLint Material::ActivateTexture2D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureName);
    GLint location = glGetUniformLocation(program, samplerName);
    glUniform1i(location, textureUnit);
    
    return location;
}

GLint Material::ActivateTexture2D(const GLchar* samplerName, const Texture2D* texture, const GLint textureUnit)
{
    GLint result = ActivateTexture2D(samplerName, texture->GetTextureID(), textureUnit);
    return result;
}

GLint Material::ActivateTexture3D(const GLchar* samplerName, const Texture3D* texture, const GLint textureUnit)
{
    GLint result = ActivateTexture3D(samplerName, texture->GetTextureID(), textureUnit);
    return result;
}


GLint Material::ActivateTexture3D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit)
{
    assert(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS > textureUnit);
    //bind texture to texture unit
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_3D, textureName);
    
    //bind shader uniform location to texture unit
    GLint location = glGetUniformLocation(program, samplerName);
    glUniform1i(location, textureUnit);
    return location;
}

GLint Material::SetModelMatrix(const glm::mat4& mat)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, MODEL_MATRIX_NAME);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    return location;
}

using MaterialSharedPtr = std::shared_ptr<Material>;




