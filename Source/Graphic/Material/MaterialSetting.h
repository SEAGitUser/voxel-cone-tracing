#pragma once

#include "OpenGL_Includes.h"


#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

#include <unordered_map>



//TODO: what is this for???
namespace {
	const char * diffuseColorName = "material.diffuseColor";
	const char * specularColorName = "material.specularColor";
	const char * emissivityName = "material.emissivity";
	const char * transparencyName = "material.transparency";
	const char * refractiveIndexName = "material.refractiveIndex";
	const char * specularReflectanceName = "material.specularReflectivity";
	const char * diffuseReflectanceName = "material.diffuseReflectivity";
	const char * specularDiffusionName = "material.specularDiffusion";
}



/// <summary> Represents a setting for a material that can be used for a shader </summary>
class MaterialSetting
{
    
public:
    enum class Type
    {
        VEC4,
        VEC3,
        VEC2,
        FLOAT,
        INT,
        SAMPLER,
        NONE
        
    };
    
private
    :
    
    union SettingValue
    {
        glm::vec4 vector4;
        glm::vec3 vector3;
        glm::vec2 vector2;
        GLfloat   floatValue;
        GLint     intValue;
        
        SettingValue()
        {
            vector4 = glm::vec4(0,0,0,0);
        }
        
    };
    
    SettingValue value;
    Type type;
    const GLchar* name = nullptr;
    
public:
    
    inline Type getType(){return type;}
    

    
    using SettingsGroup =  std::unordered_map<const GLchar* ,MaterialSetting>;
    using KeyValue = std::pair<const GLchar*, MaterialSetting> ;
    
    MaterialSetting():value(),type(Type::NONE)
    {
        
    }
    inline GLint getIntValue()
    {
        assert(type == Type::INT);
        return value.intValue;
    }
    
    inline GLfloat getFloatValue()
    {
        assert(type == Type::FLOAT);
        return value.floatValue;
    }
    
    inline glm::vec2 getVec2Value()
    {
        assert( type == Type::VEC2);
        return value.vector2;
    }
    
    inline glm::vec3 getVec3Value()
    {
        assert( type == Type::VEC3);
        return value.vector3;
    }
    
    inline glm::vec4 getVec4Value()
    {
        assert(type == Type::VEC4);
        return value.vector4;
    }
    
    inline MaterialSetting& operator=(const GLfloat &value)
    {
        type = Type::FLOAT;
        this->value.floatValue = value;
        
        return *this;
    }
    
    inline MaterialSetting& operator=(const glm::vec4 &value)
    {
        type = Type::VEC4;
        this->value.vector4 = value;
        
        return *this;
    }
    
    inline MaterialSetting& operator=(const glm::vec3 &value)
    {
        type = Type::VEC3;
        this->value.vector3 = value;
        
        return *this;
    }
    
    inline MaterialSetting& operator=(const glm::vec2 &value)
    {
        type = Type::VEC2;
        this->value.vector2 = value;
        
        return *this;
    }
    
    inline MaterialSetting& operator=(const GLint &value)
    {
        type = Type::INT;
        this->value.intValue = value;
        
        return *this;
    }

	// Basic constructor.
    
    MaterialSetting(glm::vec4 _value)
    {
        value.vector4 = _value;
        type = Type::VEC4;
    }
    
    MaterialSetting(glm::vec3 _value)
    {
        value.vector3 = _value;
        type = Type::VEC3;
    }
    
    MaterialSetting(glm::vec2 _value)
    {
        value.vector2 = _value;
        type = Type::VEC2;
    }
    
    MaterialSetting(GLint _value)
    {
        value.intValue = _value;
        type = Type::INT;
    }
    
    MaterialSetting(GLfloat _value)
    {
        value.floatValue= _value;
        type = Type::FLOAT;
    }
    


    static void Default(SettingsGroup& group) ;

    static void White(SettingsGroup& group);

    static void Cyan(SettingsGroup& group);

    static void Purple(SettingsGroup& group);

    static void Red(SettingsGroup& group);

    static void Green(SettingsGroup& group);

    static void Blue(SettingsGroup& group);

    static void Emissive(SettingsGroup& group);
    
    static const GLchar* diffuseColor;
    static const GLchar* specularColor;
    static const GLchar* specularReflectivity;
    static const GLchar* diffuseReflectivity;
    static const GLchar* emissivity;
    static const GLchar* specularDiffusion;
    static const GLchar* transparency;
    static const GLchar*  refractiveIndex;
    
private:
    
    static void AddToGroup(SettingsGroup& group, KeyValue value);
};


                      




