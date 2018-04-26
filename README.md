Adapting work created by Fredrik Präntare for Mac OS X.  It is still under construction.


Voxel Cone Tracing For Mac OS (OpenGL 4.1)
--------------

Improvements vs previous implementation:
* Geometry is no longer bound to a box of size between -1 and 1 in all 3 axis, world can be arbitarily large.
* Lowered the required OpenGL version from 4.5 to 4.1 ( The voxel paper calls out that OpenGL 4.5 is needed due to extensions used)
* Sturcture of the code has been heavily modified and comparmentalized.
* Reduced the number of cones from 9 ( 10 if specular highlight was being rendered ) to 5 ( 6 if specular highlight needs rendering)

Things left to do:

* Performance is terrible on Mac OS in my MacBook Pro 2015:
    - Full deferred rendering could be used to speed up frame rates.
    - Do cone tracing on a smaller texture, then upsample this texture, and through deferred shading color indirect illumination
    - If the scene doesn't change, we shouldn't be updating the mipmaps
    - Use smaller data type sizes for the indirect illumination (i.e.: fixed point vs. floating point)
    - Ultimately, the algorithm is very expensive, it's likely not possible to make it this run in acceptable frame rates for a game on my MacBook Pro 2015 due to
    inherent issues such as random sampling of 3D textures, this cannot be avoided. 
    
* Fredrik in his original implemenation had different cones such refraction and specular, computational resources permitting these could be added here as well

Screen Shots ( indirect illumination exagerated for illustration)
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



