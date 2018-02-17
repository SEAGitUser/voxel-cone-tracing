//
//  MaterialSetting.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/26/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "MaterialSetting.h"


 const GLchar* MaterialSetting::diffuseColor = "material.diffuseColor";
 const GLchar* MaterialSetting::specularColor = "material.specularColor";
 const GLchar* MaterialSetting::specularReflectivity = "material.specularReflectivity";
 const GLchar* MaterialSetting::diffuseReflectivity = "material.diffuseReflectivity";
 const GLchar* MaterialSetting::emissivity = "material.emissivity";
 const GLchar* MaterialSetting::specularDiffusion = "material.specularDiffusion";
 const GLchar* MaterialSetting::transparency = "material.transparency";
 const GLchar* MaterialSetting::refractiveIndex = "material.refractiveIndex";



void MaterialSetting::AddToGroup(SettingsGroup& group, KeyValue keyValue)
{
    if(group.count(keyValue.first) == 0)
    {
        group.insert(keyValue);
    }
}

