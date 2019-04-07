#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;

///TODO: 
///Implement Group
///Add data structure to store a list of Object* 
class Group:public Object3D
{
public:

	Group(){
		group = vector<Object3D*>();
	}

	Group( int num_objects ){
		group = vector<Object3D*>();
		for(int i=0;i<num_objects;i++){
			Object3D* obj = 0;
			group.push_back(obj);
		}
	}

	~Group(){
	}

	virtual bool intersect( const Ray& r , Hit& h , float tmin ) {
		bool res = false;
		bool one = res;
		int size = this->getGroupSize();
		for(int i=0;i<size;i++){
			Object3D* obj = this->group[i];
			res = obj->intersect(r, h, tmin);
			if(res == true){one = res;}
		}
		return one;
	}

	void addObject( int index , Object3D* obj ){
		group[index] = obj;
	}

	int getGroupSize(){ 
		return group.size();
	}

private:
	vector<Object3D*> group;
};

#endif

