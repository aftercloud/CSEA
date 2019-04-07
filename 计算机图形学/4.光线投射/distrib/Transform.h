#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"
///TODO implement this class
///So that the intersect function first transforms the ray
///Add more fields as necessary
class Transform: public Object3D
{
public: 
	Transform(){}
	Transform( const Matrix4f& m, Object3D* obj ):o(obj){
		this->m = m.inverse();
	}
	~Transform(){
	}
	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		Vector3f new_origin = (m * Vector4f(r.getOrigin(), 1)).xyz();
		Vector3f new_direction = (m * Vector4f(r.getDirection(), 0)).xyz();
		if(o->intersect( Ray(new_origin, new_direction) , h , tmin)){
			Matrix4f tm = this->m;
			tm.transpose();
			Vector3f new_normal = (tm * Vector4f(h.getNormal(), 1)).xyz();
			new_normal.normalize();
			h.setNormal(new_normal);
			return true;
		}
		return false;
	}

protected:
	Object3D* o; //un-transformed object	
	// Äæ±ä»»¾ØÕó
	Matrix4f m;
};

#endif //TRANSFORM_H
