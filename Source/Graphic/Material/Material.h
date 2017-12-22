#pragma once

#include "OpenGL_Includes.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Graphic/Material/Resource.h"
#include "Graphic/Material/MaterialSetting.h"
#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Camera/Camera.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Scene/Scene.h"
#include "Shader.h"

/// <summary> Represents a material that references a gl program, textures and settings. </summary>
class Material : public Resource {
public:
	virtual ~Material();
	Material(const GLchar *_name,
		const ShaderSharedPtr& vertexShader,
		const ShaderSharedPtr& fragmentShader,
		const ShaderSharedPtr& geometryShader = nullptr,
		const ShaderSharedPtr& tessEvaluationShader = nullptr,
		const ShaderSharedPtr& tessControlShader = nullptr);

    Material(const GLchar *name): name(name){}
    
    virtual void Activate(MaterialSetting::SettingsGroup& settingsGroup, Scene& scene);
    void ApplySettings(MaterialSetting::SettingsGroup& settingsGroup);
    
    inline void SetParameteri(const GLchar* parameterName, GLint const value);
    inline void SetParameterv4(const GLchar* parameterName, const glm::vec4 &value);
    inline void SetParameterv3(const GLchar* parameterName, const glm::vec3 & value);
    inline void SetParameterv2(const GLchar* parameterName, const glm::vec2 & value);
    inline void SetParameterf(const GLchar* parameterName, GLfloat const value);
    inline void SetParamatermat4(const GLchar *parameterName, const glm::mat4 &value);
    inline void ActivateTexture2D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit);
    inline void ActivateTexture3D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit);
    inline void ActivateTexture2D(const GLchar* samplerName, const Texture2D* texture, const GLint textureUnit);
    inline void ActivateTexture3D(const GLchar* samplerName, const Texture3D* texture, const GLint textureUnit);
    
    
    inline void SetModelMatrix(glm::mat4& mat);
    
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
    
    void setValue(MaterialSetting& setting, const GLchar* name);
    
    /// <summary> The actual OpenGL / GLSL program identifier. </summary>
    GLuint program;

public:


	/// <summary> A name. Just an identifier. Doesn't do anything practical. </summary>
	const GLchar* name;
};

void Material::SetParameteri(const GLchar* parameterName, GLint const value)
{
    GLint location = glGetUniformLocation(program, parameterName);
    glUniform1i(location, value);
}

void Material::SetParameterf(const GLchar* parameterName, GLfloat const value)
{
    GLint location = glGetUniformLocation(program, parameterName);
    glUniform1f(location,(value));
}

void Material::SetParameterv4(const GLchar* parameterName, const glm::vec4 &value)
{
    GLint location = glGetUniformLocation(program, parameterName);
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Material::SetParameterv3(const GLchar *parameterName, const glm::vec3 &value)
{
    GLint location = glGetUniformLocation(program, parameterName);
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Material::SetParameterv2(const GLchar *parameterName, const glm::vec2 &value)
{
    std::cout << parameterName << " with value < " << value.x  << " " << value.y <<  " > " << std::endl;
    
    GLint location = glGetUniformLocation(program, parameterName);
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void Material::SetParamatermat4(const GLchar* parameterName, const glm::mat4 &value)
{
    GLint location = glGetUniformLocation(program, parameterName);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Material::ActivateTexture2D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureName);
    glUniform1i(glGetUniformLocation(program, samplerName), textureUnit);
}

void Material::ActivateTexture2D(const GLchar* samplerName, const Texture2D* texture, const GLint textureUnit)
{
    ActivateTexture2D(samplerName, texture->GetTextureID(), textureUnit);
}

void Material::ActivateTexture3D(const GLchar* samplerName, const Texture3D* texture, const GLint textureUnit)
{
    ActivateTexture3D(samplerName, texture->GetTextureID(), textureUnit);
}


void Material::ActivateTexture3D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_3D, textureName);
    glUniform1i(glGetUniformLocation(program, samplerName), textureUnit);
}

void Material::SetModelMatrix(glm::mat4& mat)
{
    GLint location = glGetUniformLocation(program, MODEL_MATRIX_NAME);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

using MaterialSharedPtr = std::shared_ptr<Material>;




