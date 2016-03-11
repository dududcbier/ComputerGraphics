//
//  Framework for a raytracer
//  File: sphere.h
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

#ifndef PLANE_H
#define PLANE_H

#include "object.h"

class Plane : public Object
{
public:
    Plane(Point p, Vector v1, Vector v2) : p(p), v1(v1), v2(v2) { }

    virtual Hit intersect(const Ray &ray);
    virtual Color textureColor(Point p);

    const Point p;
    const Vector v1;
    const Vector v2;
};

#endif /* end of include guard: PLANE_H */
