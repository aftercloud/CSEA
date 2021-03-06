// gl_1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<Windows.h>
#include<gl/freeglut.h>
#include<time.h>
#include<iostream>
using namespace std;

// 光源位置
GLfloat position[] = { 1.0, 1.0, 5.0, 0.0 };

// 茶壶颜色
GLfloat mat[4];

// 变色的 RGB
// xi 表示差值
// Xi 表示初始值
double x1, x2, x3;
double X1, X2, X3;

// 渐变限制
const int L = 720;

// 渐变过渡
int limit = L;

// 旋转控制
bool ro = false;

// 旋转角度
double angle = 1.0;

// 旋转轴
int xyz[] = { 0, 1, 0 };


void init(void)
{
	// 光源的属性
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat local_view[] = { 0.0 };

	// 设置光源的属性
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	// 使光源生效
	glFrontFace(GL_CW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);

	// 茶壶颜色
	mat[0] = 0.5;
	mat[1] = 0.5;
	mat[2] = 0.9;

}


void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(4, 8, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glRotatef(angle, xyz[0], xyz[1], xyz[2]);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);

	glutSolidTeapot(3.0);

	glPopMatrix();

	glFlush(); 
	
	glutSwapBuffers();
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(0.0, 16.0, 0.0, 16.0*(GLfloat)h / (GLfloat)w,
			-10.0, 10.0);
	else
		glOrtho(0.0, 16.0*(GLfloat)w / (GLfloat)h, 0.0, 16.0,
			-10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
}


double rand_zero_one()
{
	return (rand() % 100) * 0.01;
}


void keyboard(unsigned char key, int x, int y)
{
	x1 = rand_zero_one() - mat[0];
	x2 = rand_zero_one() - mat[1];
	x3 = rand_zero_one() - mat[2];
	X1 = mat[0];
	X2 = mat[1];
	X3 = mat[2];

	switch (key) {
	case 'x':
		xyz[0] = 1;
		xyz[1] = xyz[2] = 0;
		break;
	case 'y':
		xyz[1] = 1;
		xyz[0] = xyz[2] = 0;
		break;
	case 'z':
		xyz[2] = 1;
		xyz[0] = xyz[1] = 0;
		break;
	case 'r':
		ro = !ro;
		break;
	case 'c':
		limit = 0;
		break;
	default:
		break;
	}
	// 刷新
	glutPostRedisplay();
}


void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		position[1] += 0.5;
		break;
	case GLUT_KEY_DOWN:
		position[1] -= 0.5;
		break;
	case GLUT_KEY_LEFT:
		position[0] -= 0.5;
		break;
	case GLUT_KEY_RIGHT:
		position[0] += 0.5;
		break;
	default:
		break;
	}
	// 刷新
	glutPostRedisplay();
}


void idle()
{
	// 变色
	if (limit++ < L)
	{
		mat[0] = x1 * limit / L + X1;
		mat[1] = x2 * limit / L + X2;
		mat[2] = x3 * limit / L + X3;
		if (!ro) 
		{
			display();
		}
	}
	// 旋转
	if (ro)
	{
		if (angle > 359.0)
		{
			angle = 0.0;
		}
		angle++;
		display();
	}
}


// 主循环
int main(int argc, char **argv)
{
	// 设置随机种子
	srand(time(NULL));

	// 初始化 GLUT
	glutInit(&argc, argv);

	// 设置显示模式
	// GLUT_RGB RGB 颜色显示
	// GLUT_RGBA RGBA 颜色显示
	// GLUT_DEPTH 深度缓冲区
	// GLUT_SINGLE 单缓冲区窗口
	// GLUT_BUFFER 双缓冲区窗口
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// 设置窗口大小
	glutInitWindowSize(400, 400);

	// 设置窗口位置
	glutInitWindowPosition(150, 150);

	// 创建窗口
	glutCreateWindow("gl_1");

	// GLUT 初始化
	init();

	// 图像缩放
	glutReshapeFunc(reshape);

	// 显示茶壶
	glutDisplayFunc(display);

	// 一般键盘事件
	glutKeyboardFunc(keyboard);

	// 特殊键盘事件
	glutSpecialFunc(SpecialKeys);

	// 空闲时调用
	glutIdleFunc(idle);

	// 开启循环
	glutMainLoop();

	return 0;
}
