// glutEx1.cpp : 定义控制台应用程序的入口点。
//
//注意FPS函数的应用

#include <stdlib.h>
#include "gl/glut.h"
#include <stdio.h>
#include <string.h>
//#include <iostream>
#include "stanford_bunny.h"

//using namespace std;

float eye[] = { 0, 6, 9 };
float center[] = { 0, 0, 0 };
float fDistance = 0.2f;
float fRotate = 0;
bool bAnim = false;

bool bDrawList = false;
GLint tableList = 0;
GLint bunnynum = 1;
GLint bunnyscale = 1;

void DrawCube(GLfloat x, GLfloat y, GLfloat z, GLfloat xlength, GLfloat ylength, GLfloat zlength) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(xlength, ylength, zlength);
	glutSolidCube(1);
	glPopMatrix();
}

void DrawLegs(GLfloat location, GLfloat width, GLfloat height) {
	DrawCube(location, 0, location, width, height, width);
	DrawCube(location, 0, -location, width, height, width);
	DrawCube(-location, 0, location, width, height, width);
	DrawCube(-location, 0, -location, width, height, width);
}

void DrawDesktop(GLfloat y, GLfloat width) {
	DrawCube(0, y, 0, width, 0.5, width);
}

void DrawTable(GLfloat height, GLfloat width) {
	GLfloat legheight = height;
	GLfloat legwidth = 0.5;
	GLfloat leglocation = width / 2.0;
	GLfloat desktopwidth = width * 1.4;
	GLfloat desktopy = height / 2.0 + 0.25;

	DrawDesktop(desktopy, desktopwidth);
	DrawLegs(leglocation, legwidth, legheight);
}

void DrawBunny(GLint index) {
	GLint row = (int)index / 4.0;
	GLint column = index % 4;

	glPushMatrix();
	glTranslatef(3 - column * 2, 3.75 + bunnyscale * 0.25, 3 - row * 2);
	glScalef(1 + bunnyscale, 1 + bunnyscale, 1 + bunnyscale);
	DrawBunny();
	//glutSolidCube(1);
	glPopMatrix();
}

GLint GenTableList()
{
	GLint lid = glGenLists(1);

	glNewList(lid, GL_COMPILE);
	for (GLint index = 0; index < bunnynum; index++) {
		DrawBunny(index);
	}

	DrawTable(6, 6);
	glEndList();
	return lid;
}

void Draw_Table_List()
{
	glCallList(tableList);
}

void DrawScene()
{
	for (GLint index = 0; index < bunnynum; index++) {
		DrawBunny(index);
	}

	DrawTable(6, 6);
}

void reshape(int width, int height)
{
	if (height == 0)									// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;
	gluPerspective(45, whRatio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void idle()
{
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	case 'q': {
		exit(0);
		break;
	}
	case 'i': {
		// 兔子数-1
		if (bunnynum > 1) {
			bunnynum--;
			tableList = GenTableList();
		}
		break;
	}
	case 'k': {
		// 兔子数+1
		if (bunnynum < 16) {
			bunnynum++;
			tableList = GenTableList();
		}
		break;
	}
	case 'j': {
		// 兔子变小
		switch (bunnyscale) {
		case 1: {
			bunnyscale = 3;
			tableList = GenTableList();
			break;
		}
		case 2:
		case 3: {
			bunnyscale -= 1;
			tableList = GenTableList();
			break;
		}
		}
		break;
	}
	case 'a':
	{
		eye[0] += fDistance;
		center[0] += fDistance;
		break;
	}
	case 'd':
	{
		eye[0] -= fDistance;
		center[0] -= fDistance;
		break;
	}
	case 'w':
	{
		eye[1] -= fDistance;
		center[1] -= fDistance;
		break;
	}
	case 's':
	{
		eye[1] += fDistance;
		center[1] += fDistance;
		break;
	}
	case 'z':
	{
		//eye[2] *= 0.95;
		eye[2] -= 0.2;
		center[2] -= 0.2;
		break;
	}
	case 'c':
	{
		//eye[2] *= 1.05;
		eye[2] += 0.2;
		center[2] += 0.2;
		break;
	}
	case 'l':
	{
		bDrawList = !bDrawList;	// 切换显示列表和非显示列表绘制方式
		break;
	}
	case ' ':
	{
		bAnim = !bAnim;
		break;
	}
	default: break;
	}
}

void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];

	char mode[64];
	if (bDrawList)
		strcpy(mode, "display list");
	else
		strcpy(mode, "naive");

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(buffer, "FPS:%4.2f %s",
			frame*1000.0 / (time - timebase), mode);
		timebase = time;
		frame = 0;
	}

	char *c;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);	// 选择投影矩阵
	glPushMatrix();					// 保存原矩阵（投影矩阵）
	glLoadIdentity();				// 装入单位矩阵
	glOrtho(0, 480, 0, 480, -1, 1);	// 位置正投影
	glMatrixMode(GL_MODELVIEW);		// 选择Modelview矩阵
	glPushMatrix();					// 保存原矩阵（Modelview矩阵）
	glLoadIdentity();				// 装入单位矩阵
	glRasterPos2f(10, 10);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glPopMatrix();					// 重置为原保存矩阵（Modelview矩阵）
	glMatrixMode(GL_PROJECTION);	// 选择投影矩阵
	glPopMatrix();					// 重置为原保存矩阵（投影矩阵）
	glMatrixMode(GL_MODELVIEW);		// 选择Modelview矩阵

	// 以下为等价方法
	//glMatrixMode(GL_PROJECTION);	// 选择投影矩阵
	//glPopMatrix();				// 重置为原保存矩阵（投影矩阵）
	//glMatrixMode(GL_MODELVIEW);	// 选择Modelview矩阵
	//glPopMatrix();				// 重置为原保存矩阵（Modelview矩阵）

	glEnable(GL_DEPTH_TEST);
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0.5, 0, 1);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// 从（0, 4, 6）看向（0, 0, 0），头顶朝上。持续接近变为从上方观察，持续远离变为平行观察。

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat gray[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat light_pos[] = { 10, 10, 10, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gray);	// 设置环境光为灰色
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);	// 应用灯光位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, gray);			// 应用环境光
	glEnable(GL_LIGHT0);							// 启用灯光（编号0？）

	if (bAnim)
		fRotate += 0.5f;
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis

	glScalef(0.4, 0.4, 0.4);
	if (!bDrawList)
		DrawScene();						// old way
	else
		Draw_Table_List();                  // new way

	getFPS();
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Exercise 4");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	tableList = GenTableList();

	glutMainLoop();
	return 0;
}


