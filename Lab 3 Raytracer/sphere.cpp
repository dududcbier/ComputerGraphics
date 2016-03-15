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
#include <assert.h>

#define PI 3.141592653589793

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

Color Sphere::textureColor(Point p) {

	// Triple a = r * rVec.normalized();
	// Triple b = Triple(1, 0, 0);

	// Triple axis = a.cross(b);
	// axis /= axis.length();
	// double ang = acos(a.dot(b) / a.length());

	// double c1 = axis.x;
	// double c2 = axis.y;
	// double c3 = axis.z;

	// double origX = p.x;
	// double origY = p.y;
	// double origZ = p.z;

	// p.x = origX * cos(ang) + (1 - cos(ang)) * (c1 * c1 * origX + c1 * c2 * origY + c1 * c3 * origZ) + (c2 * origZ - c3 * origY) * sin(ang);
	// p.y = origY * cos(ang) + (1 - cos(ang)) * (c2 * c1 * origX + c2 * c2 * origY + c2 * c3 * origZ) + (c3 * origX - c1 * origZ) * sin(ang);
	// p.z = origZ * cos(ang) + (1 - cos(ang)) * (c3 * c1 * origX + c3 * c2 * origY + c3 * c3 * origZ) + (c1 * origY - c2 * origX) * sin(ang);

	// cout << "(" << p.x << ", " << p.y << ", " << p.z << ")" << endl;

	double theta = acos((p.z - position.z) / r);
	//cout << "Theta: " << theta << " [" << (p.z - c3) / r << "]" << endl;
	double phi = atan2(p.y - position.y, p.x - position.x);

	phi -= angle * PI / 180;

	if (phi < 0)
		phi += 2 * PI;
	
	double u = phi / (2 * PI);
	double v = (PI - theta) / PI;

	// cout << "(" << u << ", " << v << ")" << endl; 
	
	assert(u >= 0 && u <= 1);
	assert(v >= 0 && v <= 1);

	return material->texture->colorAt(u, v);
	
}
