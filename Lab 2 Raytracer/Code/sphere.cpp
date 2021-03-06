//
//  Framework for a raytracer
//  File: sphere.cpp
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

#include "sphere.h"
#include <iostream>
#include <math.h>

#define PI 3.1415926535897

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
	/****************************************************
	* RT1.1: INTERSECTION CALCULATION
	*
	* Given: ray, position, r
	* Sought: intersects? if true: *t
	* 
	* Insert calculation of ray/sphere intersection here. 
	*
	* You have the sphere's center (C) and radius (r) as well as
	* the ray's origin (ray.O) and direction (ray.D).
	*
	* If the ray does not intersect the sphere, return false.
	* Otherwise, return true and place the distance of the
	* intersection point from the ray origin in *t (see example).
	****************************************************/

	// place holder for actual intersection calculation

	Vector OC = ray.O - position;
	double t;

	double A = ray.D.dot(ray.D);
	double B = 2 * ray.D.dot(OC);
	double C = OC.dot(OC) - r * r;
	
	double D = B * B - 4 * A * C;
	
	if (D < 0.0) {
		return Hit::NO_HIT();
	}

	t = std::min(((- B + sqrt(D)) / (2 * A)), ((- B - sqrt(D)) / (2 * A)));

	/****************************************************
	* RT1.2: NORMAL CALCULATION
	*
	* Given: t, C, r
	* Sought: N
	* 
	* Insert calculation of the sphere's normal at the intersection point.
	****************************************************/
	Triple p = ray.O + t * ray.D;

	Vector N = (p - position).normalized();
	
	return Hit(t,N);
		
}
