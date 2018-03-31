#include "Shader.h"

#include "Resource.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>


const std::string Shader::shaderResourcePath =  "/Shaders/";

GLuint Shader::compile() {
	// Create and compile shader.
    
	shaderID = glCreateShader(static_cast<GLint>(shaderType));
	const char * source = rawShader.c_str();
	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);

	// Check if we succeeded.
	std::string typeName = " (" + GetShaderTypeName() + ")";
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar log[1024];
		glGetShaderInfoLog(shaderID, 1024, nullptr, log);
		std::cerr << "- Failed to compile shader '" << path << "' : " << static_cast<GLint>(shaderType) << typeName << "!" << std::endl;
		std::cerr << "LOG: " << std::endl << log << std::endl;
        //the line std::getchar() makes xcode behave oddly,I'll put an assert instead
        assert(false);
		return 0;
	}
	if (shaderID == 0) {
		std::cerr << "- Could not compile shader '" << path << "' : " << static_cast<GLint>(shaderType) << typeName << "!" << std::endl;
		//std::getchar();
        assert(false);
		return 0;
	}
	std::cout << "- Shader '" << path << "' : " << static_cast<GLint>(shaderType) << typeName << " compiled successfully." << std::endl;
	return shaderID;
}

Shader::Shader(const char* _path, ShaderType _type) :  shaderType(_type) {
	
    // Load the shader instantly.
    path = Resource::resourceRoot + Shader::shaderResourcePath + _path;
    
	std::ifstream fileStream(path, std::ios::in);
	if (!fileStream.is_open()) {
		std::cerr << "Couldn't load shader '" + std::string(path) + "'." << std::endl;
		fileStream.close();
        assert(false);
		return;
	}
	std::string line = "";
	rawShader = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		rawShader.append(line + "\n");
	}
	fileStream.close();
    
    compile();
}

Shader::~Shader()
{
    glDeleteShader(shaderID);
}

const std::string Shader::GetShaderTypeName() const
{
	switch (shaderType) {
	case ShaderType::FRAGMENT:					return "fragment";
	case ShaderType::VERTEX:					return "vertex";
	case ShaderType::GEOMETRY:					return "geometry";
	case ShaderType::TESSELATION_CONTROL:		return "tesselation control";
	case ShaderType::TESSELATION_EVALUATION:	return "tesselation evaluation";
	default:									return "unknown";
	}
}
