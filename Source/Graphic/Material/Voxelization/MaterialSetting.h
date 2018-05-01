#pragma once

#include "OpenGL_Includes.h"


#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

#include <unordered_map>
#include "Graphic/Lighting/PointLight.h"

class Texture2D;
class Texture3D;


/// <summary> Represents a setting for a material that can be used for a shader </summary>

//TODO: LET'S TRY IMPLEMENTING THIS WITH TEMPLATE CLASSES
class MaterialSetting
{
    
public:
    enum class Type
    {
        MAT4,
        VEC4,
        VEC3,
        VEC2,
        FLOAT,
        INT,
        SAMPLER_2D,
        SAMPLER_3D,
        POINT_LIGHT,
        NONE
        
    };
    
    struct Sampler2D
    {
        int textureUnit;
        Texture2D* texture;
        Sampler2D():textureUnit(0),texture(nullptr)
        {}
    };
    
    struct Sampler3D
    {
        int textureUnit;
        Texture3D* texture;
        Sampler3D():textureUnit(0), texture(nullptr)
        {}
    };
    
private
    :
    
    union SettingValue
    {
        glm::vec4 vector4;
        glm::vec3 vector3;
        glm::vec2 vector2;
        GLfloat   floatValue;
        int     intValue;
        Sampler2D   sampler2D;
        Sampler3D   sampler3D;
        glm::mat4 mat4;
        PointLight pointLight;
        
        SettingValue()
        {
            mat4 = glm::mat4(0);
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
    inline int getIntValue()
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
    
    inline Sampler2D getSampler2DValue()
    {
        assert(type == Type::SAMPLER_2D);
        return value.sampler2D;
    }
    
    inline Sampler3D getSampler3DValue()
    {
        assert(type == Type::SAMPLER_3D);
        return value.sampler3D;
    }
    
    inline glm::mat4 getMat4Value()
    {
        assert(type == Type::MAT4);
        return value.mat4;
    }
    
    inline PointLight& getPointLightValue()
    {
        assert(type == Type::POINT_LIGHT);
        return value.pointLight;
    }
    
    inline MaterialSetting& operator=(const glm::mat4 &value)
    {
        type = Type::MAT4;
        this->value.mat4 = value;
        
        return *this;
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
    
    inline MaterialSetting& operator=(const int &value)
    {
        type = Type::INT;
        this->value.intValue = value;
        
        return *this;
    }
    
    inline MaterialSetting& operator=(const Sampler2D &sampler)
    {
        type = Type::SAMPLER_2D;
        this->value.sampler2D = sampler;
        
        return *this;
    }
    
    inline MaterialSetting& operator=(const PointLight &light)
    {
        type = Type::POINT_LIGHT;
        this->value.pointLight = light;
        return *this;
    }

    MaterialSetting(glm::mat4 _value)
    {
        value.mat4 = _value;
        type = Type::MAT4;
    }
    
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
    
    MaterialSetting(int _value)
    {
        value.intValue = _value;
        type = Type::INT;
    }
    
    MaterialSetting(GLfloat _value)
    {
        value.floatValue= _value;
        type = Type::FLOAT;
    }

    MaterialSetting(Sampler2D _value)
    {
        value.sampler2D = _value;
        type = Type::SAMPLER_2D;
    }

    MaterialSetting(Sampler3D _value)
    {
        value.sampler3D = _value;
        type = Type::SAMPLER_3D;
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


                      




