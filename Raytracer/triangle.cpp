#include "triangle.h"
#include <iostream>
#include <math.h>

/************************** Triangle **********************************/

Hit Triangle::intersect(const Ray &ray)
{
	/****************************************************
	* RT1.1: INTERSECTION CALCULATION
	****************************************************/
	
	double a, b, c, d, e, f, g, h, i, j, k, l; // These are the variables used as described in the book to calculate the intersection. We're using them to make the code more readable.
	
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
	
	// Basically what we do is solve a system of 3 equations, using Cramer's Rule, to determine gamma, beta and t

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
