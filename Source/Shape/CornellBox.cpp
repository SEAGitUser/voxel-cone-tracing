//
//  CornellBox.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 4/1/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#include "CornellBox.h"
#include "Shape/Mesh.h"



CornellBox::CornellBox() :
Shape()
{
    
    ObjLoader::RawObjData objData;
    ObjLoader::loadRawObjData("/Assets/Models/cornell.obj", objData);
    
    loadMesh(objData.shapes);
    
    meshProperties.push_back(VoxProperties::Green());
    meshProperties.push_back(VoxProperties::White()); //bottom
    meshProperties.push_back(VoxProperties::White());//top
    meshProperties.push_back(VoxProperties::Red());
    meshProperties.push_back(VoxProperties::White()); //back
    meshProperties.push_back(VoxProperties::White());
    
    
    //random boxes inside of the cornell box mesh
    meshes[6]->enabled = false;
    meshes[5]->enabled = false;

}


