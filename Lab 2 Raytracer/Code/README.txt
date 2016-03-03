--------------------------- Version 2.0 --------------------------- 

The program now supports shadows (from multiple light sources), reflections,
anti-aliasing and extended camera model, producing better and more flexible images. 
These features are implemented by introducing new rendering options which can be 
defined in the scene files, in a similar way as the render modes in version 1.0 
If no render options are defined, the images are rendered using phong lighting 
without any of the new features.

New render options in this version are:

Shadows
Shadows are cast if a light hits another object before it hits the object that will 
be displayed in the image that is rendered. The implementation supports multiple
light sources. Shadows can be either set to true or false.
Example of implementation:
Shadows: true

Reflections
When this mode is enabled rays are continued after their collision with an object,
depending on the specular coefficient of the object. This mode uses a recursive 
function, this recursion depth can be set in the .yaml file.
Example of implementation:
MaxRecursionDepth: 2

Anti-aliasing or super sampling
This render option allows rendering better quality images. The resolution of images 
remain the same, but more rays are fired at one pixel, the color value for said 
pixel becomes the average of the rays fired. The number of rays fired can be 
defined in the .yaml file, ideally the square root of this number should be an 
integer. If not, the number of rays fired will be the nearest lower number for 
which an integer square root value exists.
Example of implementation:
SuperSampling:
  factor: 4

(Extended) Camera model
This render option allows the user to define resolutions of images. 
(rather than the default 400x400) It also supports eye positions, a 
reference point in the scene and an up-vector. If no camera is defined 
a camera object is still created, but with parameters that correspond to 
the default render mode from version 1.0.
Example of implementation:
Camera:
    eye: [200,200,1000]
    center: [200,200,0]
    up: [0,0.75,0]
    viewSize: [800,400]

--------------------------- Version 1.0 --------------------------- 

This program renders images from .yaml files. 
The programm supports spheres, triangles and planes
Images can be rendered in three modes "phong", "zbuffer" and "normal".
This mode should be defined in the .yaml file, example: RenderMode: "zbuffer"
If no rendermode is defined, the images are redered using Phongs model by default.
Images can be generated from the scene (.yaml) files by running the program (in the
terminal:  “./ray scene01.yaml”) with the desired .yaml file.

Rendermodes:

phong
The scene is rendered using Phongs model, including a diffuse term and 
ambient and specular terms of phongs equation.

zbuffer
The scene is rendered using a z-buffer model. Figures in scenes are represented in 
grayscale according to their distance from the observation point (‘eye’). 
The closed element viewable is represented as white, the viewable element the furthest 
away from the eye is represented as pure black.

normal
Instead of the material color and shading that shows up in images rendered in the phong mode,
 the three components of the normal are mapped using rgb values. 
The range of values of the normal (-1 .. 1) are scaled back to the color range (0 .. 1).
