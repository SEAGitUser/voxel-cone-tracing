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

    class Commands
    {
    public:
        Commands(Material* material);
        
        GLint SetParameteri(const GLchar* parameterName, GLint const value);
        GLint SetParameterui(const GLchar* parameterName, GLuint const value);
        GLint SetParameterv4(const GLchar* parameterName, const glm::vec4 &value);
        GLint SetParameterv3(const GLchar* parameterName, const glm::vec3 & value);
        GLint SetParameterv2(const GLchar* parameterName, const glm::vec2 & value);
        GLint SetParameterf(const GLchar* parameterName, GLfloat const value);
        GLint SetParamatermat4(const GLchar *parameterName, const glm::mat4 &value);
        GLint SetParameterSampler2D(const GLchar* parameterName, Texture2D* sampler);
        GLint SetParameterSampler3D(const GLchar* parameterName, Texture3D* sampler);
        GLint SetPointLight(const GLchar* parameterName,  const PointLight& light  );
        GLint SetMatrix(const GLchar* parameterName, const glm::mat4& mat);
        GLint SetParameterBool(const GLchar* parameterName, bool value);
        
        GLint ActivateTexture2D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit);
        GLint ActivateTexture3D(const GLchar* samplerName, const GLint textureName, const GLint textureUnit);
        GLint ActivateTexture2D(const GLchar* samplerName, const Texture2D* texture, const GLint textureUnit);
        GLint ActivateTexture3D(const GLchar* samplerName, const Texture3D* texture, const GLint textureUnit);

        void uploadParameters(ShaderParameter::ShaderParamsGroup& settingsGroup);
        
        void uploadGlobalConstants();
        
        void setValue(ShaderParameter& setting, const GLchar* name);
        
        static const char * const PROJECTION_MATRIX_NAME;
        static const char * const VIEW_MATRIX_NAME;
        static const char * const CAMERA_POSITION_NAME;
        static const char * const NUMBER_OF_LIGHTS_NAME ;
        static const char * const MODEL_MATRIX_NAME ;
        static const char * const SCREEN_SIZE_NAME;
        static const char * const APP_STATE_NAME ;
        
        ~Commands();
    private:
        Material* material = nullptr;
        
        GLuint textureUnits;
    };
    
	Material(const GLchar *_name,
		const ShaderSharedPtr& vertexShader,
		const ShaderSharedPtr& fragmentShader,
		const ShaderSharedPtr& geometryShader = nullptr,
		const ShaderSharedPtr& tessEvaluationShader = nullptr,
		const ShaderSharedPtr& tessControlShader = nullptr);

    Material(const GLchar *name): name(name){}
    

    inline void Deactivate(){ glUseProgram(0) ;}
    
    virtual ~Material();
    
public:
    
    bool shadows = true;
    bool indirectDiffuseLight = true;
    bool indirectSpecularLight = true;
    bool directLight = true;
    
    
protected:
    
    void AssembleProgram(
                        const ShaderSharedPtr& vertexShader,
                        const ShaderSharedPtr& fragmentShader,
                        const ShaderSharedPtr& geometryShader,
                        const ShaderSharedPtr& tessEvaluationShader,
                        const ShaderSharedPtr& tessControlShader
                        );
    

    
    /// <summary> The actual OpenGL / GLSL program identifier. </summary>
    GLuint program;

public:


	/// <summary> A name. Just an identifier. Doesn't do anything practical. </summary>
	const GLchar* name;
};


using MaterialSharedPtr = std::shared_ptr<Material>;




