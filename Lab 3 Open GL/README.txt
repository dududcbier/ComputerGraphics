--------------------------------------- Version 2.0 ---------------------------------------------

This program modified in such a way that it can render objects with textures.
Running the file give an animation of a 'solar system'; textured spherical objects both rotating 
around a spherical object in the center and around themselves. The program is compatible with 
the implementation of one texture, a png image, this texture should should be located in the 
textures folder and should be called texture.png.

For rendering the animated scene of the solar system the function renderAnimatedScene is used.
This function loops through the objects that the scene contains and renders each object using 
renderPlanet. This function determined the orientation of each object in the scene using the 
centerDistance, speedArroundSelf, speedAroundCenter, position, size and time t. In each call to
renderAnimatedScene the variable t (time) is updated. The function then calls itself again with 
the updated value of t using renderLater(). This process continues indefinitely until the user
terminates the program.

--------------------------------------- Version 1.0 ---------------------------------------------

This program renders images from model objects. This version of the program supports spheres. 
Spheres are build with triangles. 

The material colour, lightposition, lightcolour and position of the spere are used to place
speres properly in an rendered image. Unfortunately there is a bug in this part of the program.
Making us unable to continue an implement the phong shading that should be in this program.