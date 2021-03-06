// gl_2.cpp: 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include<Windows.h>
#include<gl/freeglut.h>
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;

vector<vector<double>>f;
vector<vector<double>>v;

void init()
{
	ifstream file;
	file.open("bunny_1k.obj");
	char c;
	double x, y, z;
	int cc = 0;
	while (!file.eof())
	{
		vector<double>temp;
		file >> c;
		file >> x >> y >> z;
		temp.push_back(x);
		temp.push_back(y);
		temp.push_back(z);
		if (c == 'f')
		{
			f.push_back(temp);
		}
		else {
			v.push_back(temp);
		}
	}
	file.close();

}

int begin_x = 0;
int begin_y = 0;
int x_now = 0;
int y_now = 0;
int sw = 0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(1.2, 1.2, 1.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if (sw == 0)
	{
		glRotated(x_now - begin_x, 0, 1, 0);
		glRotated(y_now - begin_y, 1, 0, 0);
	}
	else {
		glScaled(x_now - begin_x, x_now - begin_x, x_now - begin_x);
	}

	glColor3f(0.5, 0.5, 0.9);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < f.size(); i++)
	{
		glVertex3f(v[f[i][0] - 1][0], v[f[i][0] - 1][1], v[f[i][0] - 1][2]);
		glVertex3f(v[f[i][1] - 1][0], v[f[i][1] - 1][1], v[f[i][1] - 1][2]);
		glVertex3f(v[f[i][2] - 1][0], v[f[i][2] - 1][1], v[f[i][2] - 1][2]);
	}
	glEnd();
	glFlush();
}


void reshape(int w, int h) 
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 10.0);
	glMatrixMode(GL_MODELVIEW);

}


void mouseClick(int button, int state, int x, int y)
{
	begin_x = x;
	begin_y = y;
	if(button== GLUT_LEFT_BUTTON)
	{
		sw = 0;
	}
	else {
		sw = 1;
	}
	
}

void mouseMove(int x, int y)
{
	x_now = x;
	y_now = y;
	display();
}


int main(int argc, char *argv[]) 
{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow("pl_2");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape); 
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutMainLoop();
	return 0;
}