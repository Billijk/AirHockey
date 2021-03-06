
/*
* OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
*/

/*
*  cube.c
*  This program demonstrates a single modeling transformation,
*  glScalef() and a single viewing transformation, gluLookAt().
*  A wireframe cube is rendered.
*/
#include "constants.h"
#include "common.h"
#include "game.h"
#include <windows.h>
#include <gl.h>
#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>

// ASCII字符总共只有0到127，一共128种字符
#define MAX_CHAR       128

unsigned char colorBuf[FRAME_WIDTH*FRAME_HEIGHT*3];
GLfloat light_position[] = { 0.0, 0.0, 2.0, 0.0 };

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 150.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0 };


#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLubyte otherImage[checkImageHeight][checkImageWidth][4];

static GLuint texName[2];

Game game;
float viewx = 0;
int frameh = FRAME_HEIGHT, framew = FRAME_WIDTH;


void makeCheckImages(void) {
	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i&0x8)==0)^((j&0x8))==0))*255;
			checkImage[i][j][0] = (GLubyte) c;
			checkImage[i][j][1] = (GLubyte) c;
			checkImage[i][j][2] = (GLubyte) c;
			checkImage[i][j][3] = (GLubyte) 255;
			c = ((((i&0x10)==0)^((j&0x10))==0))*255;
			otherImage[i][j][0] = (GLubyte) c;
			otherImage[i][j][1] = (GLubyte) 0;
			otherImage[i][j][2] = (GLubyte) 0;
			otherImage[i][j][3] = (GLubyte) 255;
		}
	}
}


void init(void) {
	glShadeModel(GL_FLAT);                                // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                   // Black Background
	glClearDepth(1.0f);                                     // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                                // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                                 // The Type Of Depth Testing To Do
}

void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if( isFirstCall ) { // 如果是第一次调用，执行初始化
		// 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for(; *str!='\0'; ++str)
		glCallList(lists + *str);
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear Screen And Depth Buffer
	//glLoadIdentity();                                       // Reset The Current Modelview Matrix
	//glTranslatef(0.0f,0.0f,-6.0f);                          // Move Right 1.5 Units And Into The Screen 6.0

	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);    

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);


	//Draw Text
	char score[50]; 
	char difficulty[50]; 
	glColor3f(1.0f, 1.0f, 1.0f);
	selectFont(18, ANSI_CHARSET, "Comic Sans MS");
	GLfloat  winX, winY;
	GLint    viewport[4];   
	GLdouble modelview[16];   
	GLdouble projection[16];   
	GLdouble npx, npy, npz;    // near point
	GLdouble fpx, fpy, fpz;    // far point
	GLdouble px, py, pz;	//cal the vector of ray
	winX = 10;
	winY = frameh - 15;
	glGetIntegerv(GL_VIEWPORT, viewport); 
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &npx, &npy, &npz);
	gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &fpx, &fpy, &fpz); 
	px = fpx - npx;
	py = fpy - npy;
	pz = fpz - npz;
	float t = -(float)(fpz / pz);
	float lx = fpx + t * px;
	float ly = fpy + t * py;
	glRasterPos3f(lx, ly, -0.1f);
	sprintf(score, "Player  %d : %d   AI", game.score1(), game.score2()); 
	drawString(score);

	winX = 10;
	winY = frameh - 35;
	glGetIntegerv(GL_VIEWPORT, viewport); 
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &npx, &npy, &npz);
	gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &fpx, &fpy, &fpz); 
	px = fpx - npx;
	py = fpy - npy;
	pz = fpz - npz;
	t = -(float)(fpz / pz);
	lx = fpx + t * px;
	ly = fpy + t * py;
	glRasterPos3f(lx, ly, -0.1f);
	sprintf(difficulty, "Difficulty : %s", game.difficulty().c_str()); 
	drawString(difficulty);
	
	winX = framew - 310;
	winY = frameh - 15;
	glGetIntegerv(GL_VIEWPORT, viewport); 
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &npx, &npy, &npz);
	gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &fpx, &fpy, &fpz); 
	px = fpx - npx;
	py = fpy - npy;
	pz = fpz - npz;
	t = -(float)(fpz / pz);
	lx = fpx + t * px;
	ly = fpy + t * py;
	glRasterPos3f(lx, ly, -0.1f);
	drawString("[esc]:EXIT [i]:INIT POSITION [r]:RESET GAME");
	
	winX = framew - 310;
	winY = frameh - 35;
	glGetIntegerv(GL_VIEWPORT, viewport); 
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &npx, &npy, &npz);
	gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &fpx, &fpy, &fpz); 
	px = fpx - npx;
	py = fpy - npy;
	pz = fpz - npz;
	t = -(float)(fpz / pz);
	lx = fpx + t * px;
	ly = fpy + t * py;
	glRasterPos3f(lx, ly, -0.1f);
	drawString("[left]/[right]:VIEW [up]/[down]:DIFFICULTY");

	//Draw Objects
	float K = 1;
	Pos2d a = game.mallet1Pos();
	Pos2d c = game.mallet2Pos();
	Pos2d b = game.puckPos();
	
	//Draw Floor
	glBegin(GL_QUADS);
	glColor3f(0.61f, 0.58f, 0.26f);
	glVertex3f(-K * 100, -K * 100, -2.0);
	glVertex3f(K * 100, -K * 100, -2.0);
	glVertex3f(K * 100, K * 100, -2.0);
	glVertex3f(-K * 100, K * 100, -2.0);
	glEnd();

	//Draw Table
	glBegin(GL_QUAD_STRIP);
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), -2.0);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), -2.0);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), -2.0);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), -2.0);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), -2.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), -2.0);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), -2.0);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), -2.0);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), -2.0);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(0.07f, 0.52f, 0.02f);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glEnd();
	
	//Draw Pillar
	/*glBegin(GL_QUAD_STRIP);
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(-K * (TABLE_WIDTH / 3 - 0.2), -K * (TABLE_LENGTH / 3 - 0.2), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 - 0.2), -K * (TABLE_LENGTH / 3 - 0.2), -4.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 + 0.2), -K * (TABLE_LENGTH / 3 - 0.2), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 + 0.2), -K * (TABLE_LENGTH / 3 - 0.2), -4.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 + 0.2), -K * (TABLE_LENGTH / 3 + 0.2), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 + 0.2), -K * (TABLE_LENGTH / 3 + 0.2), -4.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 - 0.2), -K * (TABLE_LENGTH / 3 + 0.2), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 - 0.2), -K * (TABLE_LENGTH / 3 + 0.2), -4.0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(-K * (TABLE_WIDTH / 3 - 0.2), K * (TABLE_LENGTH / 3 - 0.2), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 - 0.2), K * (TABLE_LENGTH / 3 - 0.2), -4.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 + 0.2), K * (TABLE_LENGTH / 3 - 0.2), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 + 0.2), K * (TABLE_LENGTH / 3 - 0.2), -4.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 + 0.2), K * (TABLE_LENGTH / 3 + 0.2), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 + 0.2), K * (TABLE_LENGTH / 3 + 0.2), -4.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 - 0.2), K * (TABLE_LENGTH / 3 + 0.2), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 3 - 0.2), K * (TABLE_LENGTH / 3 + 0.2), -4.0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(K * (TABLE_WIDTH / 3 - 0.2), K * (TABLE_LENGTH / 3 - 0.2), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 3 - 0.2), K * (TABLE_LENGTH / 3 - 0.2), -4.0);
	glVertex3f(K * (TABLE_WIDTH / 3 + 0.2), K * (TABLE_LENGTH / 3 - 0.2), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 3 + 0.2), K * (TABLE_LENGTH / 3 - 0.2), -4.0);
	glVertex3f(K * (TABLE_WIDTH / 3 + 0.2), K * (TABLE_LENGTH / 3 + 0.2), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 3 + 0.2), K * (TABLE_LENGTH / 3 + 0.2), -4.0);
	glVertex3f(K * (TABLE_WIDTH / 3 - 0.2), K * (TABLE_LENGTH / 3 + 0.2), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 3 - 0.2), K * (TABLE_LENGTH / 3 + 0.2), -4.0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(K * (TABLE_WIDTH / 3 - 0.2), -K * (TABLE_LENGTH / 3 - 0.2), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 3 - 0.2), -K * (TABLE_LENGTH / 3 - 0.2), -4.0);
	glVertex3f(K * (TABLE_WIDTH / 3 + 0.2), -K * (TABLE_LENGTH / 3 - 0.2), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 3 + 0.2), -K * (TABLE_LENGTH / 3 - 0.2), -4.0);
	glVertex3f(K * (TABLE_WIDTH / 3 + 0.2), -K * (TABLE_LENGTH / 3 + 0.2), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 3 + 0.2), -K * (TABLE_LENGTH / 3 + 0.2), -4.0);
	glVertex3f(K * (TABLE_WIDTH / 3 - 0.2), -K * (TABLE_LENGTH / 3 + 0.2), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 3 - 0.2), -K * (TABLE_LENGTH / 3 + 0.2), -4.0);
	glEnd();*/


	//Draw Mid Line
	/*glBegin(GL_QUAD_STRIP);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(K * (TABLE_WIDTH / 2), 0.0, 0.1);
	glVertex3f(K * (TABLE_WIDTH / 2), 0.0, 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 2), 0.0, 0.1);
	glVertex3f(-K * (TABLE_WIDTH / 2), 0.0, 0.0);
	glEnd();*/

	//Draw Wall
	glBegin(GL_QUAD_STRIP);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(K * (TABLE_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(K * (TABLE_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(-K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(-K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(-K * (TABLE_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(-K * (TABLE_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(-K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(-K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glColor3f(0.7f, 0.7f, 0.7f);
	glVertex3f(K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.0);
	glVertex3f(K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.0);
	glVertex3f(K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(K * (TABLE_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(K * (TABLE_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.0);
	glVertex3f(K * (TABLE_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.0);
	glVertex3f(K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.1);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(-K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.0);
	glVertex3f(-K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(-K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(-K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 2 + 0.1), -K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(-K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2 + 0.1), 0.0);
	glVertex3f(-K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2 + 0.1), 0.1);
	glVertex3f(-K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.0);
	glVertex3f(-K * (GOAL_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(-K * (TABLE_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 2), -K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(-K * (TABLE_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.0);
	glVertex3f(-K * (TABLE_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.1);
	glVertex3f(-K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.0);
	glVertex3f(-K * (GOAL_WIDTH / 2), K * (TABLE_LENGTH / 2), 0.1);
	glEnd();


	//Draw Mallet1
	glBegin(GL_QUAD_STRIP);
	glColor3f(0.0f, 1.0f, 0.5f);
	float ax = (a.x - TABLE_WIDTH / 2) * K, ay = (a.y - TABLE_LENGTH / 2) * K;
	for(double i = 0; i < 2 * PI; i += PI / 24)
	{
		glVertex3f(cos(i) * MALLET_DIAMETER * K / 2 + ax, sin(i) * MALLET_DIAMETER * K / 2 + ay, 0.0);
		glVertex3f(cos(i) * MALLET_DIAMETER * K / 2 + ax, sin(i) * MALLET_DIAMETER * K / 2 + ay, 0.1);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 1.0f, 0.5f);
	glVertex3f(ax, ay, 0.0f);
	glVertex3f(ax, ay, 0.1f);
	for (double i = 0; i <= 2 * PI; i += PI / 24)  
	{  
		glVertex3f(cos(i) * MALLET_DIAMETER * K / 2 + ax, sin(i) * MALLET_DIAMETER * K / 2 + ay, 0.0);
		glVertex3f(cos(i) * MALLET_DIAMETER * K / 2 + ax, sin(i) * MALLET_DIAMETER * K / 2 + ay, 0.1);
	}  
	glEnd();

	//Draw Mallet2
	glBegin(GL_QUAD_STRIP);
	glColor3f(0.0f, 0.5f, 1.0f);
	float cx = (c.x - TABLE_WIDTH / 2) * K, cy = (c.y - TABLE_LENGTH / 2) * K;
	for(double i = 0; i <= 2 * PI; i += PI / 24)
	{
		glVertex3f(cos(i) * MALLET_DIAMETER * K / 2 + cx, sin(i) * MALLET_DIAMETER * K / 2 + cy, 0.0);
		glVertex3f(cos(i) * MALLET_DIAMETER * K / 2 + cx, sin(i) * MALLET_DIAMETER * K / 2 + cy, 0.1);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.5f, 1.0f);
	glVertex3f(cx, cy, 0.0f);
	glVertex3f(cx, cy, 0.1f);
	for (double i = 0; i <= 2 * PI; i += PI / 24) 
	{
		glVertex3f(cos(i) * MALLET_DIAMETER * K / 2 + cx, sin(i) * MALLET_DIAMETER * K / 2 + cy, 0.0);
		glVertex3f(cos(i) * MALLET_DIAMETER * K / 2 + cx, sin(i) * MALLET_DIAMETER * K / 2 + cy, 0.1);
	}
	glEnd();

	//Draw Puck
	glBegin(GL_QUAD_STRIP);
	glColor3f(1.0f, 0.2f, 0.3f);
	float bx = (b.x - TABLE_WIDTH / 2) * K, by = (b.y - TABLE_LENGTH / 2) * K;
	for(double i = 0; i <= 2 * PI; i += PI / 24)
	{
		glVertex3f(cos(i) * PUCK_DIAMETER * K / 2 + bx, sin(i) * PUCK_DIAMETER * K / 2 + by, 0.0);
		glVertex3f(cos(i) * PUCK_DIAMETER * K / 2 + bx, sin(i) * PUCK_DIAMETER * K / 2 + by, 0.1);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 0.2f, 0.3f);
	glVertex3f(bx, by, 0.0f);
	glVertex3f(bx, by, 0.1f);
	for (double i = 0; i <= 2 * PI; i += PI / 24) 
	{
		glVertex3f(cos(i) * PUCK_DIAMETER * K / 2 + bx, sin(i) * PUCK_DIAMETER * K / 2 + by, 0.0);
		glVertex3f(cos(i) * PUCK_DIAMETER * K / 2 + bx, sin(i) * PUCK_DIAMETER * K / 2 + by, 0.1);
	}
	glEnd();

	glutSwapBuffers();
}

void reshape (int width, int height) {
	if (height==0) {                                    // Prevent A Divide By Zero By
		height=1;                                        // Making Height Equal One
	}

	frameh = height; framew = width;

	glViewport(0,0,width,height);                        // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
	glLoadIdentity();                                    // Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	// Change Look at
	//	gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	gluLookAt(viewx, -4.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	//	gluOrtho2D(0, width, 0, height); 

	glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'i':
		game.init();
		break;
	case 'r':
		game.clear_score();
		game.init();
		break;
	}
}
void keyboard_special(GLint key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		if (viewx > -1.5f)
			viewx -= VIEW_MOVE_SPEED;
		reshape(framew, frameh);
		break;
	case GLUT_KEY_RIGHT:
		if (viewx < 1.5f)
			viewx += VIEW_MOVE_SPEED;
		reshape(framew, frameh);
		break;
	case GLUT_KEY_UP:
		game.difficulty_up();
		game.init();
		break;
	case GLUT_KEY_DOWN:
		game.difficulty_down();
		game.init();
		break;
	}
}

void mouse(int x, int y) {
	// translate screen coordinates to local coordinates in game
	//射线选择功能
	GLfloat  winX, winY;

	//变换要绘图函数里的顺序一样，否则坐标转换会产生错误
	GLint    viewport[4];   
	GLdouble modelview[16];   
	GLdouble projection[16];   

	glGetIntegerv(GL_VIEWPORT, viewport); // 得到的是最后一个设置视口的参数  
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = x; 
	winY = frameh - y;

	GLdouble npx, npy, npz;    // near point
	gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &npx, &npy, &npz);

	GLdouble fpx, fpy, fpz;    // far point
	gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &fpx, &fpy, &fpz); 

	//cal the vector of ray
	GLdouble px, py, pz;
	px = fpx - npx;
	py = fpy - npy;
	pz = fpz - npz;

	float t = -(float)(fpz / pz);
	float lx = fpx + t * px + TABLE_WIDTH / 2;
	float ly = fpy + t * py + TABLE_LENGTH / 2;
	game.moveMouse(lx, ly);
}

void update(void) {
	game.update();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(FRAME_WIDTH, FRAME_HEIGHT); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	glutPassiveMotionFunc(mouse);
	glutIdleFunc(update);
	glutMainLoop();
	return 0;
}