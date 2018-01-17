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

/*    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    makeCheckImages();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(2, texName);
    glBindTexture(GL_TEXTURE_2D, texName[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                checkImage);

    glBindTexture(GL_TEXTURE_2D, texName[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, 
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                otherImage);*/
   
}

void display(void) {
    float K = 1;
    Pos2d a = game.mallet1Pos();
    Pos2d c = game.mallet2Pos();
    Pos2d b = game.puckPos();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear Screen And Depth Buffer
    //glLoadIdentity();                                       // Reset The Current Modelview Matrix
    //glTranslatef(0.0f,0.0f,-6.0f);                          // Move Right 1.5 Units And Into The Screen 6.0

    //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);    
    
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

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


/*    //Draw Circle
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 0.5f);
    float ax = (a.x - TABLE_WIDTH / 2) * K, ay = (a.y - TABLE_LENGTH / 2) * K;
    for(double i = 0; i < 2 * PI; i += PI / 24)
        glVertex3f(cos(i) * MALLET_DIAMETER * K / 2 + ax, sin(i) * MALLET_DIAMETER * K / 2 + ay, 0.0);
    glEnd();
    //Draw Circle
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.5f, 1.0f);
    float cx = (c.x - TABLE_WIDTH / 2) * K, cy = (c.y - TABLE_LENGTH / 2) * K;
    for(double i = 0; i < 2 * PI; i += PI / 24)
        glVertex3f(cos(i) * MALLET_DIAMETER * K / 2 + cx, sin(i) * MALLET_DIAMETER * K / 2 + cy, 0.0);
    glEnd();

    //Draw Puck
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.2f, 0.3f);
    float bx = (b.x - TABLE_WIDTH / 2) * K, by = (b.y - TABLE_LENGTH / 2) * K;
    for(double i = 0; i < 2 * PI; i += PI / 24)
        glVertex3f(cos(i) * PUCK_DIAMETER * K / 2 + bx, sin(i) * PUCK_DIAMETER * K / 2 + by, 0.0);
    glEnd();*/


	//Draw Mallet1
    glBegin(GL_QUAD_STRIP);
    glColor3f(0.0f, 1.0f, 0.0f);
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
    glBegin(GL_POLYGON);
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

    glViewport(0,0,width,height);                        // Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                    // Reset The Projection Matrix
	
    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	// Change Look at
//	gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	gluLookAt(1.2f, -6.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
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
	winY = FRAME_HEIGHT - y;
	
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(FRAME_WIDTH, FRAME_HEIGHT); 
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouse);
    glutIdleFunc(update);
    glutMainLoop();
    return 0;
}