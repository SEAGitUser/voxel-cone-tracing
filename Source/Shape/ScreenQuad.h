//
//  ScreenQuad.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 3/30/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#pragma once

#include "Primitive.h"

class ScreenQuand : public Primitive
{
public:
    
    class Commands: public Primitive::Commands
    {
    public:
        explicit Commands(ScreenQuand* quad);
        void render() override;
        
    private:
        ScreenQuand* quad;
    };
    
    ScreenQuand();
};
