Adapting work created by Fredrik Präntare for Mac OS X.  It is still under construction.  Most implemetations found on the internet use OpenGL 4.2 and OpenGL 4.5 and are also done
on Windows.  I've decided to implement this algorithm for Mac OS using OpenGL 4.1.  This is the version that Apple supports.


Voxel Cone Tracing For Mac OS (OpenGL 4.1)
--------------

Improvements vs previous implementation:
* Geometry is no longer bound to a box of size between -1 and 1 in all 3 axis, world can be arbitrarily large.
* Lowered the required OpenGL version from 4.5 to 4.1 ( The voxel paper calls out that OpenGL 4.5 is needed due to extensions used)
    - Implemented 3D rendering using geometry shaders, previously it was being done with image load/store OpenGL extensions
    - Implemented interporlation between 3D texture mipmaps in shader code.
    - Implemented OpenGL functions not found in 4.1:
        - glClearTexImage
        - glTexStorage3D
    - Implemented Mipmapping for 3D textures using compute shaders ( this feature is missing in 4.1)
* Sturcture of the code has been heavily modified and comparmentalized.
* Reduced the number of cones from 9 ( 10 if specular highlight was being rendered ) to 5 ( 6 if specular highlight needs rendering)

Things left to do:

* Performance is terrible on Mac OS in my MacBook Pro 2015 (mobile GPU as of this date is about 7 years old):
    - Full deferred rendering could be used to speed up frame rates.
    - Do cone tracing on a smaller texture, then upsample this texture, and through deferred shading color indirect illumination
    - If the scene doesn't change, we shouldn't be updating the mipmaps
    - Use smaller data type sizes for the indirect illumination (i.e.: fixed point vs. floating point)
    - Ultimately, the algorithm is very expensive, it's likely not possible to make it run in acceptable frame rates for a game on my MacBook Pro 2015 due to inherent drawbacks in the algorithm such as random sampling of 3D textures, this cannot be avoided unless caching is used, but this reduces the dynamic nature of this lighting technique. 
    
* Fredrik in his original implemenation had different cones such refraction and specular, computational resources permitting these could be added here as well
* A fundamental problem with this implementation is that it is not in HDR.  The lighting equations are not restricted to values between 0 and 1, this causes a disconnect between the intention of how color should look like and how it actually displays.
* Gamma correction.

Screen Shots ( indirect illumination & ambient occlusion exagerated for illustration)
------

<p align="center">

The following is a screenshot of the ambient occlusion:
<img src="https://github.com/phonowiz/voxel-cone-tracing/blob/master/Assets/Screenshots/ambient-occlusion.png">

The following is a screenshot of the indirect illumination:
<img src="https://github.com/phonowiz/voxel-cone-tracing/blob/master/Assets/Screenshots/indirect-illumination.png">
</a>

The following is a screenshot fo teh full illumination (exagerated for illustration purposes):
<img src="https://github.com/phonowiz/voxel-cone-tracing/blob/master/Assets/Screenshots/full-illumination.png">
</p>

Inspired by "Interactive Indirect Illumination Using Voxel Cone Tracing" by Crassin et al.

(Cyril Crassin, Fabrice Neyret, Miguel Saintz, Simon Green and Elmar Eisemann)
https://research.nvidia.com/sites/default/files/publications/GIVoxels-pg2011-authors.pdf



