//
//  MaterialSetting.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/26/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "MaterialSetting.h"


 const GLchar* MaterialSetting::diffuseColor = "diffuseColor";
 const GLchar* MaterialSetting::specularColor = "specularColor";
 const GLchar* MaterialSetting::specularReflectivity = "specularReflectivity";
 const GLchar* MaterialSetting::diffuseReflectivity = "diffuseReflectivity";
 const GLchar* MaterialSetting::emissivity = "emissivity";
 const GLchar* MaterialSetting::specularDiffusion = "specularDiffusion";
 const GLchar* MaterialSetting::transparency = "transparency";
 const GLchar* MaterialSetting::refractiveIndex = "refractiveIndex";


void MaterialSetting::Default(SettingsGroup& group)
{
    KeyValue pair1(diffuseColor, MaterialSetting(glm::vec3(1.000f, 0.412f, 0.706f))); //pink color by default
    KeyValue pair2(specularColor, MaterialSetting(glm::vec3(1.0f)));
    KeyValue pair3(specularReflectivity, MaterialSetting(0.0f));
    KeyValue pair4(diffuseReflectivity, MaterialSetting(1.0f));
    KeyValue pair5(emissivity, MaterialSetting(0.0f));
    KeyValue pair6(specularDiffusion, MaterialSetting(2.0f));
    KeyValue pair7(transparency, MaterialSetting(0.0f));
    KeyValue pair8(refractiveIndex, MaterialSetting(1.4f));

    AddToGroup(group, pair1);
    AddToGroup(group, pair2);
    AddToGroup(group, pair3);
    AddToGroup(group, pair4);
    AddToGroup(group, pair5);
    AddToGroup(group, pair6);
    AddToGroup(group, pair7);
    AddToGroup(group, pair8);
}


void MaterialSetting::AddToGroup(SettingsGroup& group, KeyValue keyValue)
{
    if(group.count(keyValue.first) == 0)
    {
        group.insert(keyValue);
    }
}

void MaterialSetting::White(SettingsGroup& group)
{
    MaterialSetting::Default(group);
    
    group[diffuseColor] = MaterialSetting(glm::vec3(0.97f, 0.97f, 0.97f));

}

void  MaterialSetting::Cyan(SettingsGroup& group)
{
    
    MaterialSetting::Default(group);
    
    group[diffuseColor] = MaterialSetting(
                                          glm::vec3(0.30f, 0.95f, 0.93f)
                                          );
}

void MaterialSetting::Purple( SettingsGroup& group )
{
    MaterialSetting::Default(group);
    
    group[diffuseColor] = MaterialSetting(
                                          glm::vec3(0.97f, 0.05f, 0.93f)
                                          );
}

void MaterialSetting::Red(SettingsGroup& group)
{
    MaterialSetting::Default(group);
    
    group[diffuseColor] = MaterialSetting(
                                          glm::vec3(1.0f, 0.26f, 0.27f)
                                          );

}

void MaterialSetting::Green(SettingsGroup& group)
{
    MaterialSetting::Default(group);
    group[diffuseColor] = MaterialSetting(
                                          glm::vec3(0.27f, 1.0f, 0.26f)
                                          );
}

void  MaterialSetting::Blue(SettingsGroup& group)
{
    MaterialSetting::Default(group);
    group[diffuseColor] = MaterialSetting(
                                          glm::vec3(0.35f, 0.38f, 1.0f)
                                          );
}

void  MaterialSetting::Emissive(SettingsGroup& group)
{
    MaterialSetting::Default(group);
    group[diffuseColor] = MaterialSetting(
                                          glm::vec3(0.85f, 0.9f, 1.0f)
                                          );
    group[emissivity] = MaterialSetting(1.0f);
}
