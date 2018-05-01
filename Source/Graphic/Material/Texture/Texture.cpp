//
//  Texture.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/11/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "Texture.h"

unsigned int Texture::Commands::activeTexture = 0;

Texture::Commands::Commands(Texture* texture):
previousTexture(0)
{
    assert(activeTexture == 0 && "You cannot have two active textures at the same time, please call Texture::end() on active \
           command before creating a new command");
    activeTexture = texture->textureID;
    tex = texture;
}

void Texture::Commands::clear(glm::vec4 clearColor)
{
    glClearTexImage(tex->textureID, 0, GL_RGBA, GL_FLOAT, &clearColor);
}

Texture::Commands::~Commands()
{
    end();
}

void Texture::Commands::end()
{
    activeTexture = 0;
}

void Texture::Commands::deleteTexture()
{
    glDeleteTextures(1, &tex->textureID);
}

void Texture::Commands::unpackAlignment(unsigned int alignment)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
}
