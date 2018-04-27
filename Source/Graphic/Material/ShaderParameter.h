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
class ShaderParameter
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
        BOOLEAN,
        UINT,
        SAMPLER_2D,
        SAMPLER_3D,
        POINT_LIGHT,
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
        GLuint    uintValue;
        Texture2D*   sampler2D;
        Texture3D*   sampler3D;
        glm::mat4 mat4;
        PointLight pointLight;
        bool     boolean;
        
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
    

    
    using ShaderParamsGroup =  std::unordered_map<const GLchar* ,ShaderParameter>;
    using KeyValue = std::pair<const GLchar*, ShaderParameter> ;
    
    ShaderParameter():value(),type(Type::NONE)
    {
        
    }
    inline GLint getIntValue()
    {
        assert(type == Type::INT);
        return value.intValue;
    }
    
    inline GLuint getUnsignedInt()
    {
        assert(type == Type::UINT);
        return value.uintValue;
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
    
    inline Texture2D* getSampler2DValue()
    {
        assert(type == Type::SAMPLER_2D);
        return value.sampler2D;
    }
    
    inline Texture3D* getSampler3DValue()
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
    
    inline bool getBoolValue()
    {
        assert(type == Type::BOOLEAN);
        return value.boolean;
    }
    
    inline ShaderParameter& operator=(const glm::mat4 &value)
    {
        type = Type::MAT4;
        this->value.mat4 = value;
        
        return *this;
    }
    
    inline ShaderParameter& operator=(const GLfloat &value)
    {
        type = Type::FLOAT;
        this->value.floatValue = value;
        
        return *this;
    }
    
    inline ShaderParameter& operator=(const glm::vec4 &value)
    {
        type = Type::VEC4;
        this->value.vector4 = value;
        
        return *this;
    }
    
    inline ShaderParameter& operator=(const glm::vec3 &value)
    {
        type = Type::VEC3;
        this->value.vector3 = value;
        
        return *this;
    }
    
    inline ShaderParameter& operator=(const glm::vec2 &value)
    {
        type = Type::VEC2;
        this->value.vector2 = value;
        
        return *this;
    }
    
    inline ShaderParameter& operator=(const GLint &value)
    {
        type = Type::INT;
        this->value.intValue = value;
        
        return *this;
    }

    inline ShaderParameter& operator=(const GLuint &value)
    {
        type = Type::UINT;
        this->value.intValue = value;
        
        return *this;
    }
    
    inline ShaderParameter& operator=(Texture2D* sampler)
    {
        type = Type::SAMPLER_2D;
        this->value.sampler2D = sampler;
        
        return *this;
    }
    
    inline ShaderParameter& operator=(Texture3D* sampler)
    {
        type = Type::SAMPLER_3D;
        this->value.sampler3D = sampler;
        return *this;
    }
    
    inline ShaderParameter& operator=(const PointLight &light)
    {
        type = Type::POINT_LIGHT;
        this->value.pointLight = light;
        return *this;
    }
    
    inline ShaderParameter& operator=(const bool value)
    {
        type = Type::BOOLEAN;
        this->value.boolean = value;
        return *this;
    }

    ShaderParameter(glm::mat4 _value)
    {
        value.mat4 = _value;
        type = Type::MAT4;
    }
    
    ShaderParameter(glm::vec4 _value)
    {
        value.vector4 = _value;
        type = Type::VEC4;
    }
    
    ShaderParameter(glm::vec3 _value)
    {
        value.vector3 = _value;
        type = Type::VEC3;
    }
    
    ShaderParameter(glm::vec2 _value)
    {
        value.vector2 = _value;
        type = Type::VEC2;
    }
    
    ShaderParameter(GLint _value)
    {
        value.intValue = _value;
        type = Type::INT;
    }
    ShaderParameter(GLuint _value)
    {
        value.uintValue = _value;
        type = Type::UINT;
    }
    
    ShaderParameter(GLfloat _value)
    {
        value.floatValue= _value;
        type = Type::FLOAT;
    }

    ShaderParameter(Texture2D* _value)
    {
        value.sampler2D = _value;
        type = Type::SAMPLER_2D;
    }

    ShaderParameter(Texture3D* _value)
    {
        value.sampler3D = _value;
        type = Type::SAMPLER_3D;
    }
    
private:
    
    static void AddToGroup(ShaderParamsGroup& group, KeyValue value);
};


                      




