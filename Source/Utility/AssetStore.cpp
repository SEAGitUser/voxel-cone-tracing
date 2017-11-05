//
//  AssetStore.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/4/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "Utility/AssetStore.h"


#if __APPLE__
const std::string AssetStore::resourceRoot = "../Resources";

#else

const std::string AssetStore::resourceRoot = ".";

#endif
