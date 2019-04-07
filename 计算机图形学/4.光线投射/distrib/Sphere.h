#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:
	Sphere(){ 
		//unit ball at the center

		this->center = Vector3f();
		this->radius = 1;
		this->material = 0;
	}

	Sphere( Vector3f center , float radius , Material* material ):Object3D(material){
		this->center = center;
		this->radius = radius;
	}


	~Sphere(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		Vector3f origin = r.getOrigin();
		origin -= this->center;
		Vector3f direction = r.getDirection();
		float t = h.getT();
		float a, b, c, x, y, z;
		a = origin[0];
		b = origin[1];
		c = origin[2];
		x = direction[0];
		y = direction[1];
		z = direction[2];
		float calc_a, calc_b, calc_c;
		calc_a = x*x + y*y + z*z;
		calc_b = 2*(a*x + b*y + c*z);
		calc_c = a*a + b*b + c*c - this->radius * this->radius;
		float d = calc_b*calc_b - 4*calc_a*calc_c;
		d = sqrt(d);
		if(d > 0){
			float result = (-calc_b-d)/2/calc_a;
			if(result >= tmin && result < t){
				Vector3f point = r.getOrigin() + result * r.getDirection();
				Vector3f normal = point - this->center;
				normal.normalize();
				h.set(result, this->material, normal);
				return true;
			}
		}
		return false;
	}

protected:
	Vector3f center;
	float radius;

};


#endif
