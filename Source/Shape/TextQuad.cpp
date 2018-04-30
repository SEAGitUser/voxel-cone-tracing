//
//  TextQuad.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 4/27/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#include "TextQuad.h"
#include "Texture2D.h"
#include "Material.h"
#include "MaterialStore.h"
#include "FBO_2D.h"


TextQuad::TextQuad(glm::vec2 & _dimensions):
Mesh()
{
    indices.reserve(6);
    vertexData.reserve(6);
    screenDimensions = _dimensions;
    scale = 1.0f;
    textDisplay = MaterialStore::GET_MAT<Material>("text-display");
    color = glm::vec4(1.0f);
    orthoProjection = glm::ortho(0.0f, screenDimensions.x, 0.0f, screenDimensions.y, -.1f, 3.0f);
    loadTextures();
    staticMesh = false;
    
    //this doesn't change
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(5);
    
    setupMeshRenderer();
    
}

void TextQuad::loadTextures()
{
    int result = FT_Init_FreeType(&ft);
    assert(result == 0 && "FreeType Library failed to initialize");
    
#ifdef __APPLE__
    
    // this font comes with MacOS X
    result = FT_New_Face(ft, "/Library/Fonts/Devanagari Sangam MN.ttc", 0, &face);
    
#else
    //other platforms not supported
    #error "If you would like text to render on screen, you'll need to specify a .ttc file on your computer where the font comes from"

#endif
    
    
    assert(result == 0 && "FreeType font not found");
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    for(GLubyte c = 0; c < 128; c++)
    {
        GLuint result = FT_Load_Char(face, c , FT_LOAD_RENDER);
        assert(result == 0);
        
        Texture2D* texture = new Texture2D(true);
        Texture2D::Commands textureCommands(texture);
        textureCommands.unpackAlignment(1);
        textureCommands.end();
        
        texture->SetWidth(face->glyph->bitmap.width);
        texture->SetHeight(face->glyph->bitmap.rows);
        texture->SetDataType(GL_UNSIGNED_BYTE);
        texture->SetPixelFormat(GL_RED);
        texture->SetMinFilter(GL_LINEAR);
        texture->SetMagFilter(GL_LINEAR);
        texture->SetBuffer(face->glyph->bitmap.buffer);
        texture->SaveTextureState();
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        

        characters.insert(std::pair<GLchar, Character>(c, character));
    }
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
}

void TextQuad::updateVertices( Character& ch, glm::vec2& position)
{

    float xpos = position.x + ch.Bearing.x * scale;
    float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;//position.y + ch.Bearing.y * scale;
    
    VertexData data;
    vertexData.clear();
    indices.clear();
    
    GLfloat w = ch.Size.x * scale;
    GLfloat h = ch.Size.y * scale;
    
    data.position = glm::vec3(xpos, ypos + h, 0.0f);
    data.texCoord = glm::vec2(0.0f, 0.0f);
    vertexData.push_back(data);
    
    data.position = glm::vec3(xpos, ypos, 0.0f);
    data.texCoord = glm::vec2(0.0f, 1.0f);
    vertexData.push_back(data);
    
    data.position = glm::vec3(xpos + w, ypos, 0.0f);
    data.texCoord = glm::vec2(1.0f, 1.0f);
    vertexData.push_back(data);
    
    data.position = glm::vec3(xpos, ypos + h,0.0f);
    data.texCoord = glm::vec2( 0.0f, 0.0f);
    vertexData.push_back(data);
    
    data.position = glm::vec3(xpos + w, ypos,0.0f);
    data.texCoord = glm::vec2( 1.0f, 1.0f);
    vertexData.push_back(data);

    data.position = glm::vec3(xpos + w, ypos + h,0.0f);
    data.texCoord = glm::vec2( 1.0f, 0.0f);
    vertexData.push_back(data);
}

void TextQuad::setupMeshRenderer()
{
    glError();
    TextQuad::Commands commands(this);
    commands.uploadGPURenderingData();
}

void TextQuad::print(std::string& text, glm::vec2 position)
{
    FBO::Commands fboCommands(FBO_2D::getDefault().get());
    
    fboCommands.enableDepthTest(false);
    fboCommands.backFaceCulling(false);
    fboCommands.blendSrcAlphaOneMinusSrcAlpha();
    
    Commands commands(this);
    
    static ShaderParameter::ShaderParamsGroup group;
    Material::Commands matCommands(textDisplay.get());
    
    group["projection"] =orthoProjection;
    
    glError();
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character& ch =characters[*c];
        updateVertices(ch, position);

        TextQuad::Commands txtCommands(this);
        
        txtCommands.uploadGPUVertexSubData();
        glError();
        group["text"] = ch.texture;
        group["textColor"] = color;
        render(group, matCommands);
        position.x += (ch.Advance >> 6) * scale;
    }
}


void TextQuad::render(ShaderParameter::ShaderParamsGroup &group, Material::Commands &commands)
{
    if(enabled)
    {
        commands.uploadParameters(group);
        TextQuad::Commands meshCommands(this);
        meshCommands.render();
    }
    
}


///////COMMANDS

TextQuad::Commands::Commands(TextQuad* _textQuad):
Primitive::Commands(_textQuad)
{
    textQuad = _textQuad;
    primitive = static_cast<Primitive*>(textQuad);

}

void TextQuad::Commands::uploadGPUVertexData()
{

    auto dataSize = sizeof(VertexData);
    static const int NUMBER_OF_ELEMENTS = 3;
    glBindBuffer(GL_ARRAY_BUFFER, textQuad->vbo);
    glError();
    glBufferData(GL_ARRAY_BUFFER, 6 * dataSize, nullptr,
                 textQuad->staticMesh ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    glError();
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, NUMBER_OF_ELEMENTS, GL_FLOAT, GL_FALSE, dataSize, (GLvoid*)offsetof(VertexData, position));
    glEnableVertexAttribArray(TEXTURE_LOCATION);
    static const int NUMBER_OF_TEXTURE_ELEMENTS = 2;
    glVertexAttribPointer(TEXTURE_LOCATION, NUMBER_OF_TEXTURE_ELEMENTS, GL_FLOAT, GL_FALSE, dataSize, (GLvoid*)offsetof(VertexData, texCoord));
    
}

void TextQuad::Commands::uploadGPUVertexSubData()
{
    glBindBuffer(GL_ARRAY_BUFFER, textQuad->vbo);
    glError();
    auto dataSize = sizeof(VertexData);
    size_t s = textQuad->vertexData.size();
    glBufferSubData(GL_ARRAY_BUFFER,0,textQuad->vertexData.size() * dataSize, textQuad->vertexData.data());
    glError();
}

void TextQuad::Commands::render()
{
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

TextQuad::~TextQuad()
{
    for( std::map<GLchar, Character>::iterator it = characters.begin();
        it != characters.end(); ++it)
    {
        Character& ch = it->second;
        delete ch.texture;
    }
}



