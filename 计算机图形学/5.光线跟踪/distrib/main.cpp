#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include "RayTracer.h"
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
	int max_bounces = atol(argv[10]);

	Image image(w, h);

	RayTracer rayTracer = RayTracer(&parser, max_bounces);

	for(int i=0;i<w;i++){
		for(int j=0;j<h;j++){
			Vector3f pixelColor;
			for(int m=0;m<3;m++){
				for(int n=0;n<3;n++){
					float ri = (rand()%100)/100.0-0.5;
					float rj = (rand()%100)/100.0-0.5;
					Ray ray = camera->generateRay(Vector2f(i-w/2-1+m+ri, j-h/2-1+n+rj));
					Hit hit = Hit();
					pixelColor += rayTracer.traceRay(ray, 0, 0, 1, hit);
				}
			}
			image.SetPixel(i, j, Vector3f(pixelColor[0]/9, pixelColor[1]/9, pixelColor[2]/9));
		}
	}

	Image image2(w, h);

	float K[] = {0.1201,0.2339,0.2931, 0.2339, 0.1201};

	for(int i=2;i<w-2;i++){
		for(int j=2;j<h-2;j++){
			Vector3f pixelColor;
			for(int m=0;m<5;m++){
				pixelColor += image.GetPixel(i, j-2+m) * K[m];
			}
			for(int m=0;m<5;m++){
				pixelColor += image.GetPixel(i-2+m, j) * K[m];
			}
			image2.SetPixel(i, j, Vector3f(pixelColor[0]/2, pixelColor[1]/2, pixelColor[2]/2));
		}
	}

	for(int i=0;i<w;i++){
		for(int j=0;j<h;j++){
			if(i<2||i>w-3||j<2||j>h-3){
				image2.SetPixel(i, j, image.GetPixel(i, j));
			}
		}
	}

	Image image3(w, h);

	for(int i=1;i<w-1;i++){
		for(int j=1;j<h-1;j++){
			Vector3f pixelColor;
			for(int m=0;m<3;m++){
				pixelColor += image2.GetPixel(i, j-1+m);
			}
			for(int m=0;m<3;m++){
				pixelColor += image2.GetPixel(i-1+m, j);
			}
			image3.SetPixel(i, j, Vector3f(pixelColor[0]/9, pixelColor[1]/9, pixelColor[2]/9));
		}
	}

	for(int i=0;i<w;i++){
		for(int j=0;j<h;j++){
			if(i<1||i>w-2||j<1||j>h-2){
				image3.SetPixel(i, j, image2.GetPixel(i, j));
			}
		}
	}

	image3.SaveImage(argv[7]);

	return 0;
}

