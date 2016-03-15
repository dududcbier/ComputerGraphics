//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"
#include "camera.h"

class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
    double maxDepth;
    double minDepth;
    int shadows;
    int recursiveDepth;
    int maxRecursionDepth;
    int ssFactor;
    double b;
    double y;
    double beta;
    double alpha;
    
public:
    Color trace(const Ray &ray);       // tracer corresponds to the rendering in Phong mode
    Color traceNormal(const Ray &ray); // traceNormal corresponds to the rendering in normal mode
    Color traceZbuffer(const Ray &ray);  // traceZbuffer corresponds to the rendering in zbuffer mode
    Color traceGooch(const Ray &ray);  // traceGooch corresponds to the rendering in Gooch mode
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    void setRenderMode(int r);
    void minMaxDepth(const Ray &ray);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
    void setMaxDepth(double x);
    void setMinDepth(double x);
    void setShadow(int s);
    void setMaxRecursionDepth(int m);
    void setSuperSamplingFactor(int ss);
    void setCamera(Camera cam);
    void setY(double x);
    void setB(double x);
    void setBeta(double x);
    void setAlpha(double x);

    Camera camera;
    int renderMode;
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
