#include "quad.h"
#include <iostream>
#include <math.h>

/************************** quad **********************************/

Hit Quad::intersect(const Ray &ray)
{
	/****************************************************
	* RT1.2: NORMAL CALCULATION
	* Assuming the points are given in order 
	* (p1 connected to p2, p2 to p3, p3 to p4, p4 to p1)
	****************************************************/
	Vector v1 = p2 - p1;
	Vector v2 = p4 - p1;
	Vector N = v1.cross(v2).normalized(); // The normal is obtained by doing the cross product between the two vectors that generate the plane
	
	/****************************************************
	* RT1.1: INTERSECTION CALCULATION
	****************************************************/
	
	// Intersection with the plane of the quad
	double denominator = ray.D.dot(N);
	if (denominator > 0)
		return Hit::NO_HIT(); // no intersection
		
	double t = N.dot(p1 - ray.O) / denominator; 
	if (t == 0) // if t equals 0, then the ray is contained in the plane and therefore shouldn't be seen
		return Hit::NO_HIT();

	Point hit = ray.O + t * ray.D;

	// Projection 
	double u = (hit - p1).dot(v1);
	double v = (hit - p1).dot(v2);

	if (u >= 0 && u <= v1.length_2() && v >= 0 && v <= v2.length_2())
		return Hit(t,N);

	return Hit::NO_HIT();
}

Color Quad::textureColor(Point p) {
	return Object::textureColor(p);
}
