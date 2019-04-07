#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;
///TODO: implement this class.
///Add more fields as necessary,
///but do not remove hasTex, normals or texCoords
///they are filled in by other components
class Triangle: public Object3D
{
public:
	Triangle();
	///@param a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m):Object3D(m){
		this->a = a;
		this->b = b;
		this->c = c;
		hasTex = false;
	}

	virtual bool intersect( const Ray& ray,  Hit& hit , float tmin){
		Vector3f Rd = ray.getDirection();
		Vector3f Ro = ray.getOrigin();

		Matrix3f A(
			a[0]-b[0], a[0]-c[0], Rd[0],
			a[1]-b[1], a[1]-c[1], Rd[1],
			a[2]-b[2], a[2]-c[2], Rd[2]
		);
		float dA = A.determinant();

		Matrix3f beta(
			a[0]-Ro[0], a[0]-c[0], Rd[0],
			a[1]-Ro[1], a[1]-c[1], Rd[1],
			a[2]-Ro[2], a[2]-c[2], Rd[2]
		);
		float db = beta.determinant() / dA;

		Matrix3f gamma(
			a[0]-b[0], a[0]-Ro[0], Rd[0],
			a[1]-b[1], a[1]-Ro[1], Rd[1],
			a[2]-b[2], a[2]-Ro[2], Rd[2]
		);
		float dg = gamma.determinant() / dA;

		Matrix3f t(
			a[0]-b[0], a[0]-c[0], a[0]-Ro[0],
			a[1]-b[1], a[1]-c[1], a[1]-Ro[1],
			a[2]-b[2], a[2]-c[2], a[2]-Ro[2]
		);
		float dt = t.determinant() / dA;

		float value[] = {1-db-dg, db, dg};

		if(value[0]>0 && value[1]>0 && value[2]>0){
			if(dt >= tmin && dt < hit.getT()){

				Vector3f normal = value[0] * this->normals[0] +
					value[1] * this->normals[1] +
					value[2] * this->normals[2];
				normal.normalize();

				Vector2f texture = value[0] * this->texCoords[0] +
					value[1] * this->texCoords[1] +
					value[2] * this->texCoords[2];

				hit.set(dt, this->material, normal);
				if(this->hasTex){
					hit.hasTex = this->hasTex;
					hit.setTexCoord(texture);
				}

				return true;
			}
		}
		return false;
	}
	bool hasTex;
	Vector3f normals[3];
	Vector2f texCoords[3];
protected:
	Vector3f a;
	Vector3f b;
	Vector3f c;
};

#endif //TRIANGLE_H
