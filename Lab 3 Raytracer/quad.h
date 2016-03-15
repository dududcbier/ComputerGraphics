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

#ifndef QUAD_H
#define QUAD_H

#include "object.h"

class Quad : public Object
{
public:
    Quad(Point p1, Point p2, Point p3, Point p4) : p1(p1), p2(p2), p3(p3), p4(p4) { }

    virtual Hit intersect(const Ray &ray);
    virtual Color textureColor(Point p);

    const Point p1;
    const Point p2;
    const Point p3;
    const Point p4;
};

#endif /* end of include guard: QUAD_H */
