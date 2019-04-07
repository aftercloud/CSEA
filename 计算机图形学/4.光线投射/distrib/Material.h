#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
///TODO:
///Implement Shade function that uses ambient, diffuse, specular and texture
class Material
{
public:

	Material( const Vector3f& d_color ,const Vector3f& s_color=Vector3f::ZERO, float s=0):
	  diffuseColor( d_color),specularColor(s_color), shininess(s)
	  {

	  }

	  virtual ~Material()
	  {

	  }

	  virtual Vector3f getDiffuseColor() const 
	  { 
		  return  diffuseColor;
	  }


	  Vector3f Shade( const Ray& ray, const Hit& hit,
		  const Vector3f& dirToLight, const Vector3f& lightColor ) 
	  {
		  Vector3f kd = this->getDiffuseColor();
		  if(hit.hasTex){
			  kd = t(hit.texCoord[0], hit.texCoord[1]);
		  }

		  float d = Vector3f::dot(dirToLight, hit.getNormal());
		  d = d < 0 ? 0 : d;

		  Vector3f I = -dirToLight;
		  Vector3f V = 2 * hit.getNormal() * Vector3f::dot(-hit.getNormal(), I);
		  Vector3f R = I + V;
		  V = -ray.getDirection();
		  float v = Vector3f::dot(V, R);
		  v = v < 0 ? 0 : v;

		  return d * lightColor * kd + specularColor * lightColor * pow(v, shininess);
	  }

	  void loadTexture(const char * filename){
		  t.load(filename);
	  }
protected:
	// 漫反射颜色
	Vector3f diffuseColor;
	// 高光颜色
	Vector3f specularColor;
	float shininess;
	// 纹理颜色
	Texture t;
};



#endif // MATERIAL_H
