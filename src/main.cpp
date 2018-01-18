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

Game game;
float viewx = 0;
int frameh = FRAME_HEIGHT, framew = FRAME_WIDTH;


void init(void) {
    glShadeModel(GL_FLAT);                                // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                   // Black Background
    glClearDepth(1.0f);                                     // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                                // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                                 // The Type Of Depth Testing To Do
}


void drawString(const char* str) {
    static int isFirstCall = 1;
    static GLuint lists;

    if( isFirstCall ) { // ����ǵ�һ�ε��ã�ִ�г�ʼ��
                        // Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
        isFirstCall = 0;

        // ����MAX_CHAR����������ʾ�б���
        lists = glGenLists(MAX_CHAR);

        // ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
        wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
    }
    // ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
    for(; *str!='\0'; ++str)
        glCallList(lists + *str);
}

void display(void) {
    float K = 1;
    Pos2d a = game.mallet1Pos();
    Pos2d c = game.mallet2Pos();
    Pos2d b = game.puckPos();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear Screen And Depth Buffer

	glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(-0.35f, 0.0f, 2.0f);
	char score[50]; 
	char difficulty[50]; 
	sprintf(score, "Player  %d : %d   AI", game.score1(), game.score2()); 
	drawString(score);
    glRasterPos3f(-0.35f, 0.0f, 1.8f);
	sprintf(difficulty, "Difficulty : %s", game.difficulty().c_str()); 
	drawString(difficulty);


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
        case 'r':
            game.reset();
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
            break;
        case GLUT_KEY_DOWN:
            game.difficulty_down();
            break;
   }
}

void mouse(int x, int y) {
    // translate screen coordinates to local coordinates in game
    //����ѡ����
	GLfloat  winX, winY;
	
	//�任Ҫ��ͼ�������˳��һ������������ת�����������
    GLint    viewport[4];   
    GLdouble modelview[16];   
    GLdouble projection[16];   
  
    glGetIntegerv(GL_VIEWPORT, viewport); // �õ��������һ�������ӿڵĲ���  
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