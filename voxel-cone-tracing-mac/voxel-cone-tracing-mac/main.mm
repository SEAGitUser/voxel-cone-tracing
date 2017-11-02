//
//  main.m
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 10/29/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//


#import "../../Source/Application.h"

int main(int argc, const char * argv[]) {
    
    Application &app = Application::getInstance();
    app.init();
    
    app.run();
    return 0;
    
    
}
