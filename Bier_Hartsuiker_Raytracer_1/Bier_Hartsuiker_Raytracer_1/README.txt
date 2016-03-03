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
