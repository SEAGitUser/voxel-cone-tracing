//
//  TextQuad.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 4/27/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Primitive.h"
#include "Mesh.h"
#include "glm.hpp"
#include <string>
#include <map>


class Texture2D;
class Material;
class TextQuad : private Mesh
{
    
private:
    struct Character {
        Texture2D* texture;    // ID handle of the glyph texture
        glm::ivec2 Size;       // Size of glyph
        glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
        unsigned int     Advance;    // Offset to advance to next glyph
    };
    
public:
    TextQuad(glm::vec2& screenDimensions);
    
    class Commands: public Primitive::Commands
    {
    public:
        Commands(TextQuad* _textQuad);
        void uploadGPUVertexData() override;
        void render() override;
        void uploadGPUVertexSubData();
    private:
        TextQuad* textQuad = nullptr;
    };

    inline void setColor(glm::vec4& _color){ color = _color;}
    inline void setScale(float _scale ){ scale = _scale;}
    
    void print(std::string& text, glm::vec2 position);
    
    void render(ShaderParameter::ShaderParamsGroup& group, Material::Commands& commands) override;
    
    ~TextQuad();

private:
    
    void init();
    void loadTextures();
    void updateVertices(Character &character, glm::vec2 &position);
    void setupMeshRenderer() override;

private:
    glm::vec2 screenDimensions;
    glm::vec4 color;
    glm::mat4 orthoProjection;
    
    std::map<GLchar, Character> characters;
    std::shared_ptr<Material> textDisplay;

    FT_Library ft;
    FT_Face face;
    
    float scale;
};


