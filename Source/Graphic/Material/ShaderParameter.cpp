//
//  MaterialSetting.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/26/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "ShaderParameter.h"

void ShaderParameter::AddToGroup(ShaderParamsGroup& group, KeyValue keyValue)
{
    if(group.count(keyValue.first) == 0)
    {
        group.insert(keyValue);
    }
}

