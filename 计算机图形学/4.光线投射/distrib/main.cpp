#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
	// Fill in your implementation here.

	// This loop loops over each of the input arguments.
	// argNum is initialized to 1 because the first
	// "argument" provided to the program is actually the
	// name of the executable (in our case, "a4").
	for( int argNum = 1; argNum < argc; ++argNum )
	{
		std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
	}


	// First, parse the scene using SceneParser.
	// Then loop over each pixel in the image, shooting a ray
	// through that pixel and finding its intersection with
	// the scene.  Write the color at the intersection to that
	// pixel in your output image.

	SceneParser parser = SceneParser(argv[2]);
	Group* group = parser.getGroup();
	Camera* camera = parser.getCamera();

	int w = atol(argv[4]);
	int h = atol(argv[5]);

	Image image(w, h);

	for(int i=0;i<w;i++){
		for(int j=0;j<h;j++){
			Ray ray = camera->generateRay(Vector2f(i-w/2, j-h/2));
			Hit hit = Hit();
			Vector3f pixelColor = parser.getBackgroundColor();

			if(group->intersect(ray, hit, 0)){
				pixelColor = parser.getAmbientLight();
				for(int k=0;k<parser.getNumLights();k++){
					float distanceToLight;
					Vector3f dirToLight;
					Vector3f lightColor;
					parser.getLight(k)->getIllumination(hit.getT()*ray.getDirection()+ray.getOrigin(), dirToLight, lightColor, distanceToLight);
					pixelColor += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
				}
			}

			image.SetPixel(i, j, pixelColor);
		}
	}

	image.SaveImage(argv[7]);


	return 0;
}

