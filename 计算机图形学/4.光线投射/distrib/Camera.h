#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vector2f& point ) = 0 ; 

	virtual float getTMin() const = 0 ; 
	virtual ~Camera(){}
protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;

};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle){
		this->center = center;
		this->direction = direction;
		this->up = up;
		this->angle = angle;
	}

	virtual Ray generateRay( const Vector2f& point){
		// ��ֱ������Ļ��λ����
		Vector3f w = this->direction;
		w.normalize();

		// ��������Ļˮƽ���ҵ�λ����
		Vector3f u = Vector3f::cross(w, this->up);
		u.normalize();

		// ��������Ļ��ֱ���ϵ�λ����
		Vector3f v = Vector3f::cross(u, w);
		v.normalize();

		float x, y, z;
		x = point.x();
		y = point.y();

		// ������Ļ 200 x 200
		float a = 100 * sqrt(2.0);
		float b = 100 / sin(this->angle/2);
		z = b * b - a * a;
		z = sqrt(z);

		Vector3f direction = x*u+y*v+z*w;
		direction.normalize();

		return Ray(this->center, direction);
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
	float angle;
};

#endif //CAMERA_H
