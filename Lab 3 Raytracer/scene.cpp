//
//  Framework for a raytracer
//  File: scene.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "scene.h"
#include "material.h"

void Scene::setMaxDepth(double x){
	maxDepth = x;
}

void Scene::setMinDepth(double x){
	minDepth = x;
}

/* The method below (minMaxDepth()) is used to determine the minimum and maximum depth 
 * that the image needs. Only used for the z-buffer */

void Scene::minMaxDepth(const Ray &ray){
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        
        if (hit.t < min_hit.t)
            min_hit = hit; 
            
        if (min_hit.t < minDepth) // minDepth has to be updated (if needed)
			setMinDepth(min_hit.t);
        
        if (min_hit.t > maxDepth && min_hit.t < 100000) // maxDepth has to be updated (if needed)
			setMaxDepth(min_hit.t);
    }    

}

Color Scene::trace(const Ray &ray)
{

    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t < min_hit.t && hit.t > 0.0) { // If hit.t == 0, then the light source would've hit itself, which is something we don't want, specially for the reflections.
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector


    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector+Vector      vector sum
    *        Vector-Vector      vector difference
    *        Point-Point        yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double*Color        scales each color component (r,g,b)
    *        Color*Color        dito
    *        pow(a,b)           a to the power of b
    ****************************************************/
	
	Color color;
	
	if (material->texture == "")
		color = material->ka * material->color;
		
	else {
		Image *texture = new Image(material->texture.c_str());
		Triple texCoord = obj->toTextureCoordinates(hit);
		color = texture->colorAt(texCoord.x, texCoord.y);
	}
	
    Vector R;
	
    for (unsigned int i = 0; i < lights.size(); i++){ // The effect of each light source has to be determined
        Vector L = lights[i]->position - hit;
        L.normalize();
        
        Ray lightRay(lights[i]->position, L);
        
        Hit hitObj(obj->intersect(lightRay));        
        
        // Implementation of generating shadows in the scene. If the boolean value shadows is set to 1, 
        // this piece of code computes whether or not direct light is cast on the object (the case if there
        // is no earlier intersection). If there is no direct light, said light is not used for further computation.
        int directLight = 1; 
        
		if (shadows) {
			for (unsigned j = 0; j < objects.size(); j++){
				Hit lightHit(objects[j]->intersect(lightRay));
				if (lightHit.t > hitObj.t)
					directLight = 0;
			}
		}
		
		R = 2 * (N.dot(L) * N) - L;
        
		color += (material->kd * material->color * lights[i]->color * std::max(0.0, N.dot(L))  + material->ks * lights[i]->color * pow(std::max(0.0, R.dot(V)), material->n)) * directLight; 

    }

    if (recursiveDepth < maxRecursionDepth && material->ks > 0 ){ // Checks whether or not it should go deeper in the recursion
		
		Vector reflectionVector = 2 * (N.dot(V) * N) - V;

		Ray reflection(hit, reflectionVector); // Shooting a ray from the hit point in the direction of the reflection

        recursiveDepth++;
        color += material->ks * trace(reflection);
        recursiveDepth--;
    }

    return color;
}

Color Scene::traceNormal(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Vector N = min_hit.N;                          //the normal at hit point


    /****************************************************
    * The colors are set according to the normal components.
    * The mapping of the two intervals ([-1, 1] -> [0, 1])
    * is being made by the function f(x) = (x + 1) / 2.
    ****************************************************/

    Color color = Color((N.x + 1) / 2, (N.y + 1) / 2, (N.z + 1) / 2);

    return color;
}

Color Scene::traceZbuffer(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

	int t = min_hit.t;
    
    /****************************************************
    * The colors are set according to the t value. The larger
    * the t, the further away the object is and the darker is
    * it's color.
    ****************************************************/
    Color color = Color((1 - (t - minDepth)/(maxDepth - minDepth)),(1 - (t - minDepth)/(maxDepth - minDepth)),(1-(t - minDepth)/(maxDepth - minDepth)));

    return color;
}

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();
    
    if (renderMode == 1){ // z-buffer
		setMaxDepth(-9999999);
		setMinDepth(9999999);
        for (int j = 0; j < h; j++){
			for (int i = 0; i < w; i++){
				Point pixel2(i+0.5, h-1-j+0.5, 0);
                Ray ray2(camera.getEye(), (pixel2-camera.getEye()).normalized());
                minMaxDepth(ray2);
            }
        }
	}
	
	Triple gaze = camera.getCenter() - camera.getEye(); // The eye looks at the center
	
	Triple right = gaze.cross(camera.getUpVector()); // right vector is perpendicular to the up vector
	
	right.normalize();
	right *= camera.getUpVector().length();
	
	Point topRightCorner = camera.getCenter() + right * camera.getWidth() / 2 + camera.getUpVector() * camera.getHeight() / 2; 
	
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
			Color col;
			
			 // The purpose of these two for loops (below) is so that we can get the SuperSampling done.
			 // On each iteration the color is altered to get better results in the image.
			 
			for (int i = 1; i <= sqrt(ssFactor); i++){
				for (int j = 1; j <= sqrt(ssFactor); j++) {
					
					Point p(w - (x+ i/(sqrt(ssFactor)) + 1), h - (h-1-y+ j/(sqrt(ssFactor) + 1)), 0);
					Point pixel = topRightCorner - camera.getUpVector().length() * p;	// The pixels are scaled according to the length of the up vector	
					
					Ray ray(camera.getEye(), (pixel-camera.getEye()).normalized());

					if (renderMode == 0) // Phong - this is the default render mode
						col += trace(ray) / ssFactor;
			
					if (renderMode == 1) // z-buffer
						col += traceZbuffer(ray) / ssFactor;;
				
					if (renderMode == 2)  
						col += traceNormal(ray) / ssFactor;; // normal
				}
			}
			col.clamp();
			img(x,y) = col;
        }
    }
    
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}

void Scene::setRenderMode(int r)
{
    renderMode = r;
}

void Scene::setShadow(int s)
{
    shadows = s;
}

void Scene::setMaxRecursionDepth(int m){
    maxRecursionDepth = m;
}

void Scene::setSuperSamplingFactor(int ss){
    ssFactor = ss;
}

void Scene::setCamera(Camera cam) {
	camera = cam;
}

