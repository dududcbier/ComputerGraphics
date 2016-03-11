#include "plane.h"
#include <iostream>
#include <math.h>

/************************** plane **********************************/

Hit Plane::intersect(const Ray &ray)
{
	/****************************************************
	* RT1.2: NORMAL CALCULATION
	****************************************************/
	Vector N = v1.cross(v2).normalized(); // The normal is obtained by doing the cross product between the two vectors that generate the plane
	
	/****************************************************
	* RT1.1: INTERSECTION CALCULATION
	****************************************************/
	
	double denominator = ray.D.dot(N);
	if (denominator == 0)
		return Hit::NO_HIT(); // no intersection
		
	double t = N.dot(p - ray.O) / denominator; 
	if (t == 0) // if t equals 0, then the ray is contained in the plane and therefore shouldn't be seen
		return Hit::NO_HIT();

	return Hit(t,N);
}

Color Plane::textureColor(Point p) {
	return Object::textureColor(p);
}
