//
//  PointRenderer.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 3/13/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#pragma once

#include "Primitive.h"


class Points : protected Primitive
{
public:

    class Commands :  public Primitive::Commands
    {
    public:
        explicit Commands(Points* point);
        void render() override;
        
        ~Commands();
    private:
        Points* points = nullptr;
    };
    
    Points(unsigned int count);
    ~Points();
    
private:
    
    unsigned int count;
};

