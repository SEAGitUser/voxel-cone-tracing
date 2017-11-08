//
//  Resource.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/7/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "Graphic/Material/Resource.h"


#if __APPLE__
const std::string Resource::resourceRoot = "../Resources";

#else

const std::string Resource::resourceRoot = ".";

#endif
