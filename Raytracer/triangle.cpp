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

#include "triangle.h"
#include <iostream>
#include <math.h>

/************************** Triangle **********************************/

Hit Triangle::intersect(const Ray &ray)
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


	double a, b, c, d, e, f, g, h, i, j, k, l;

	a = p1.x - p2.x;
	b = p1.y - p2.y;
	c = p1.z - p2.z;

	d = p1.x - p3.x;
	e = p1.y - p3.y;
	f = p1.z - p3.z;
	
	g = ray.D.x;
	h = ray.D.y;
	i = ray.D.z;

	j = p1.x - ray.O.x;
	k = p1.y - ray.O.y;
	l = p1.z - ray.O.z;

	double ei_hf = e * i - h * f;
	double gf_di = g * f - d * i;
	double dh_eg = d * h - e * g;
	double ak_jb = a * k - j * b;
	double jc_al = j * c - a * l;
	double bl_kc = b * l - k * c;

	double M     = a * (ei_hf) + b * (gf_di) + c * (dh_eg);

	double gamma = (i * (ak_jb) + h * (jc_al) + g * (bl_kc)) / M;

	if (gamma < 0 || gamma > 1) {
		return Hit::NO_HIT();
	}

	double beta  = (j * (ei_hf) + k * (gf_di) + l * (dh_eg)) / M;

	if (beta < 0 || beta > 1 - gamma){
		return Hit::NO_HIT();
	}

	double t   = - (f * (ak_jb) + e * (jc_al) + d * (bl_kc)) / M;

	/****************************************************
	* RT1.2: NORMAL CALCULATION
	****************************************************/

	Vector N = (p2 - p1).cross(p3 - p1).normalized();

	return Hit(t,N);
}
