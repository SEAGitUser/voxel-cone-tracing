
OpenGL will be retired from MacOS
---------------
I started this project before the announcment that OpenGL will be depricated from MacOS, as a result, I have abandoned it, please check my Vulkan implementation here:

https://github.com/phonowiz/vulkan-gui-test/blob/master/README.md



Purpose
----------
I started this project to create platform in which I can develop demos of different research topics and display my results visually. Game engines certainly are great tools in which this could also be done, however, they are designed to do "the heavy lifting" so that game developers concentrate of content creation, I believe this limits exposure I'd like to have to different technologies and software topics.

For me, what matters is not necessarily the end result, but rather the journey and the lessons you learn as you come up with ideas that may prove to be right or wrong.  It is my opinion that the best way to grow is to constantly seek the unknown, and get comfortable at being uncomfortable.

Voxel Cone Tracing For Mac OS X (OpenGL 4.1)
--------------

Improvements vs previous implementation:
* Added Retina support
* Geometry is no longer bound to a box of size between -1 and 1 in all 3 axis, world can be arbitrarily large.
* Lowered the required OpenGL version from 4.5 to 4.1 ( The voxel paper calls out OpenGL extensions not supported in 4.1)
    - Implemented 3D rendering using geometry shaders, previously it was being done with image load/store OpenGL extensions not supported in 4.1
    - Implemented interpolation between 3D texture mipmaps in shader code.
    - Implemented OpenGL functions not found in 4.1:
        - glClearTexImage
        - glTexStorage3D
    - Implemented Mipmapping for 3D textures using compute shaders ( this feature is missing in 4.1)
* Structure of the code has been heavily modified and compartmentalized.
* Re-wrote the voxel cone tracing shader to reduced the number of cones from 9 ( 10 if specular highlight was being rendered ) to 5 ( 6 if specular highlight needs rendering).  As it was originally written, it couldn't be ran on my Mac due to prohibitive performance. 

Things left to do:

* Performance is still not great on my Mac OS X MacBook Pro 2015 (mobile GPU as of this date is about 7 years old):
    - Full deferred rendering could be used to speed up frame rates.
    - Do cone tracing on a smaller texture, then upsample this texture, and through deferred shading color indirect illumination
    - If the scene doesn't change, we shouldn't be updating the mipmaps
    - Use faster data types for the indirect illumination (i.e.: fixed point vs. floating point)
    - Don't update voxel textures every frame, depending on how fast things move, every 3 or 4 frames might be enough.
    - Ultimately, the algorithm is very expensive, it's likely not possible to make it run in acceptable frame rates for a game on my MacBook Pro 2015 due to inherent drawbacks in the algorithm such as unpredictable sampling of 3D textures, this cannot be avoided unless caching is used, but this reduces the dynamic nature of this lighting technique.
    
* Fredrik in his original implementation had different cones such refraction and specular, computational resources permitting these could be added here as well
* A fundamental problem with this implementation is that it is not in HDR.  The lighting equations are not restricted to values between 0 and 1, this causes a disconnect between the intention of how color should look like and how it actually displays.
* Gamma correction.
* Get rid of raw pointers and use smart pointers.
* I did not implement the Octree mentioned in the paper due to its complexity, and also because I think this would kill performance even more.  Instead, I like the approach taken by James MacLaren as described here: https://www.gamasutra.com/view/news/286023/Graphics_Deep_Dive_Cascaded_voxel_cone_tracing_in_The_Tomorrow_Children.php


Build Requirements
-------

* Project is always developed on the latest Mac OS X build, make sure you are up to date. 

* Install the latest Xcode build, download from the app store: https://itunes.apple.com/us/app/xcode/id497799835?mt=12

* Once you have Xcode, you'll want to install FreeType on your Mac.  This library is used to render text to screen:

    - Download FreeType 2.9: https://sourceforge.net/projects/freetype/files/freetype2/2.9/ and unzip the file. 
    - In Finder, go to your Downloads folder and double click the freetype-2.8.1.tar.gz file to unpack it into a folder named freetype-2.9.1. Move that folder now into the src folder you created in your Home folder.
    - Open Terminal.app, type `cd ~/Downloads/freetype-2.9` and press the return key. Continue with `./configure` and press the return key again. You will see a bunch of text rolling through the Terminal app. 
    - When it is done, type `make` and press the return key to see more text rolling through.
    - When it is done, type `sudo make install` and press the return key. Enter your Mac login password, then press the return key again.

Screen Shots
------

<p align="center">

The following is a screenshot of the ambient occlusion:
<img src="https://github.com/phonowiz/voxel-cone-tracing/blob/master/Assets/Screenshots/ambient-occlusion.png">

The following is a screenshot of the indirect illumination:
<img src="https://github.com/phonowiz/voxel-cone-tracing/blob/master/Assets/Screenshots/indirect-illumination.png">
</a>

The following is a screenshot for the full illumination:
<img src="https://github.com/phonowiz/voxel-cone-tracing/blob/master/Assets/Screenshots/full-illumination.png">
</p>

The following is a screenshot for the full illumination:
<img src="https://github.com/phonowiz/voxel-cone-tracing/blob/master/Assets/Screenshots/full-illumination-2.png">
</p>

Inspired by "Interactive Indirect Illumination Using Voxel Cone Tracing" by Crassin et al.

(Cyril Crassin, Fabrice Neyret, Miguel Saintz, Simon Green and Elmar Eisemann)
https://research.nvidia.com/sites/default/files/publications/GIVoxels-pg2011-authors.pdf



