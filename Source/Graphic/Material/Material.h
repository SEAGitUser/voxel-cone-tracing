#pragma once

#include "OpenGL_Includes.h"

#include "glm/glm.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Graphic/Material/Resource.h"




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

    Material(const GLchar *name): name(name)
    {}
    
    
    virtual void Activate(){}
    
    GLuint ProgramID()const { return program;}
    

    
    
    
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


