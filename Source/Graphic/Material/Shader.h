#pragma once

#include "OpenGL_Includes.h"

#include <string>
#include "Resource.h"


/// <summary> Represents a shader program. </summary>
class Shader : public Resource{
public:
	enum ShaderType {
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
		TESSELATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
		TESSELATION_CONTROL = GL_TESS_CONTROL_SHADER
	};

    static const std::string shaderResourcePath;
	const ShaderType shaderType;

	/// <summary> Returns the name of the shader type of this shader. </summary>
	const std::string GetShaderTypeName() const;
    
    const GLint ShaderID() const { return shaderID; };
    
    /// <summary> Creates and loads a shader from disk. Does not compile it. </summary>
    Shader(const char* _path, ShaderType _type);
    
    /// <summary> Compiles the shader. Returns the OpenGL shader ID. </summary>
    GLuint compile();

    
    ~Shader();
protected:
	/// <summary> The shader path. </summary>
	std::string path;
    
    GLint shaderID;




private:
	std::string rawShader;
	Shader();
};
