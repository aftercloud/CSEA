#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"

#define EPSILON 0.001

//IMPLEMENT THESE FUNCTIONS
//These function definitions are mere suggestions. Change them as you like.

// 镜面反射
Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{
	Vector3f R = incoming + 2 * normal * Vector3f::dot(-normal, incoming);
	R.normalize();
	return R;
}

// 折射
bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
	float n, float nt, Vector3f& transmitted)
{
	float dN = Vector3f::dot(incoming, normal);

	float gh = 1 - n*n / nt/nt * (1 - dN*dN);

	if(gh <= 0){
		return false;
	}

	transmitted = n/nt * (incoming - normal*dN) - normal*sqrt(gh);
	transmitted.normalize();

	return true;
}

RayTracer::RayTracer( SceneParser * scene, int max_bounces 
	//more arguments if you need...
	) :
m_scene(scene)

{
	g=scene->getGroup();
	m_maxBounces = max_bounces;
}

RayTracer::~RayTracer()
{
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,
	float refr_index, Hit& hit )
{
	if(this->g->intersect(ray, hit, EPSILON)){

		Vector3f pixelColor;
		Vector3f mirrorColor;
		Vector3f transmittedColor;

		pixelColor = this->m_scene->getAmbientLight();
		for(int k=0;k<this->m_scene->getNumLights();k++){
			float distanceToLight;
			Vector3f dirToLight;
			Vector3f lightColor;
			Vector3f point = ray.pointAtParameter(hit.getT());
			this->m_scene->getLight(k)->getIllumination(point, dirToLight, lightColor, distanceToLight);

			Ray shadowRay = Ray(point, dirToLight);
			Hit shadowHit = Hit();
			this->g->intersect(shadowRay, shadowHit, EPSILON);
			if(shadowHit.getT() < distanceToLight){
				continue;
			}
			pixelColor += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
		}

		pixelColor = pixelColor * hit.getMaterial()->getDiffuseColor();

		// 二次光线权重
		float right = 1;

		if(this->m_maxBounces > bounces){

			// 当前光线交点
			Vector3f point = ray.pointAtParameter(hit.getT());

			// 反射
			Vector3f R = mirrorDirection(hit.getNormal(), ray.getDirection());
			Ray r = Ray(point, R);
			Hit rh = Hit();
			mirrorColor = this->traceRay(r, EPSILON, bounces+1, refr_index, rh);

			// 折射
			if(hit.getMaterial()->getRefractionIndex()>0){
				float n = refr_index;
				float nt = hit.getMaterial()->getRefractionIndex();
				Vector3f nn = hit.getNormal();
				if(Vector3f::dot(nn, ray.getDirection())>0){
					nn *= -1;
					nt = 1;
				}

				Vector3f transmitted;
				if(transmittedDirection(nn, ray.getDirection(), n, nt, transmitted)){
					Ray t = Ray(point, transmitted);
					Hit th = Hit();
					transmittedColor = this->traceRay(t, EPSILON, bounces+1, refr_index, th);
					float c = n > nt ? abs(Vector3f::dot(transmitted, nn)) : abs(Vector3f::dot(ray.getDirection(), nn));
					float r0 = pow((nt-n)/(nt+n), 2);
					right = r0 + (1-r0) * pow((1-c), 5);
				}
			}
		}
		return pixelColor + hit.getMaterial()->getSpecularColor() * (right * mirrorColor + (1-right) * transmittedColor);
	}
	return this->m_scene->getBackgroundColor(ray.getDirection());
}
