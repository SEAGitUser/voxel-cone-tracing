#pragma once

#include "OpenGL_Includes.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Graphic/Material/Resource.h"
#include "Texture2D.h"
#include "Texture3D.h"

class Shader;

/// <summary> Represents a material that references a gl program, textures and settings. </summary>
class Material : public Resource {
public:
	virtual ~Material();
	Material(const GLchar *_name,
		Shader * vertexShader,
		Shader * fragmentShader,
		Shader * geometryShader = nullptr,
		Shader * tessEvaluationShader = nullptr,
		Shader * tessControlShader = nullptr);

    Material(const GLchar *name): name(name){}
    
    virtual void Activate();
    
    GLuint ProgramID()const { return program;}
    
    inline void SetParameteri(const GLchar* parameterName, GLint const value);
    inline void SetParameterv4(const GLchar* parameterName, const glm::vec4 &value);
    inline void SetParameterf(const GLchar* parameterName, GLfloat const value);
    inline void SetParamatermat4(const GLchar *parameterName, const glm::mat4 &value);
    inline void ActivateTexture2D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit);
    inline void ActivateTexture3D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit);
    inline void ActivateTexture2D(const GLchar* samplerName, const Texture2D* texture, const GLint textureUnit);
    inline void ActivateTexture3D(const GLchar* samplerName, const Texture3D* texture, const GLint textureUnit);
    
protected:
    
    void AssembleProgram(
                        const Shader * vertexShader,
                        const Shader * fragmentShader,
                        const Shader * geometryShader,
                        const Shader * tessEvaluationShader,
                        const Shader * tessControlShader
                        );
    
    /// <summary> The actual OpenGL / GLSL program identifier. </summary>
    GLuint program;

public:


	/// <summary> A name. Just an identifier. Doesn't do anything practical. </summary>
	const GLchar* name;
};

void Material::SetParameteri(const GLchar* name, GLint const value)
{
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform1i(location, value);
}

void Material::SetParameterf(const GLchar* name, GLfloat const value)
{
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform1f(location,(value));
}

void Material::SetParameterv4(const GLchar* name, const glm::vec4 &value)
{
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Material::SetParamatermat4(const GLchar* name, const glm::mat4 &value)
{
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
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




