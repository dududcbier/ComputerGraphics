#include "plane.h"
#include <iostream>
#include <math.h>

/************************** plane **********************************/

Hit Plane::intersect(const Ray &ray)
{
	/****************************************************
	* RT1.2: NORMAL CALCULATION
	****************************************************/
	Vector N = v1.cross(v2).normalized();
	
	/****************************************************
	* RT1.1: INTERSECTION CALCULATION
	****************************************************/
	
	double denominator = ray.D.dot(N);
	if (denominator == 0)
		return Hit::NO_HIT();
		
	double t = N.dot(p - ray.O) / denominator;
	if (t == 0)
		return Hit::NO_HIT();

	return Hit(t,N);
}
