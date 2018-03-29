//
//  RenderTarget.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "RenderTarget.h"
#include "Mesh.h"
#include "Shape.h"

void RenderTarget::Render( Scene& renderScene )
{
    for(Shape* shape : renderScene.shapes)
    {
        shape->render(renderScene);
    }
}
