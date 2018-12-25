/*
This source code is accompanying the Cloth Tutorial at the cg.alexandra.dk blog.

You may use the code in any way you see fit. Please leave a comment on the blog
or send me an email if the code or tutorial was somehow helpful.

Everything needed is defined in this file, it is probably best read from the
bottom and up, since dependancy is from the bottom and up

A short overview of this file is;
* includes
* physics constant

* class Vec3
* class object (with position Vec3)
* class Constraint (of two objects)
* class Cloth (with objects and constraints)

* Cloth object and ball (instance of Cloth Class)

* OpenGL/Glut methods, including display() and main() (calling methods on Cloth object)

Jesper Mosegaard, clothTutorial@jespermosegaard.dk

Tested on: Windows Vista / Visual Studio 2005
Linux (Red Hat) / GCC 4.1.2

History:
2 Jun 2009 - Initial version
6 Jan 2010 - Typo corrected in call of glutInitDisplayMode to enable depth-buffer (Thanks Martijn The)

*/

#include "../core/common.h"
#include "../math/vec3.h"
#include "../object/object.h"
#include "../constrain/constrain.h"
#include "../SoftBody/cloth.h"

using namespace ClothSimulation;

// Just below are three global variables holding the actual animated stuff; Cloth and Ball
int change = 0;
int mo = 0;
Cloth cloth1(4, 4, 50, 50, mo); // one Cloth object of the Cloth class
Vec3 ball_pos(2, -4.5, 2); // the center of our one ball
float ball_radiusa = 0.4; // the radius of our one ball
float ball_radiusb = 0.5; // the radius of our one ball

Vec3 cube_center(1, -30, 1);
float cube_edge = 50;

//a few objects that will not move
Vec3 ball_pos1(2.5f, -4.5, 1.5);
float ball_radius1 = 0.4;
Vec3 ball_pos2(1, -4.5, 2);
float ball_radius2 = 0.4;
Vec3 cube_center1(2.5, -4.5, 3);
float cube_edge1 = 0.5;
Vec3 cube_center2(1, -4.5, 3);
float cube_edge2 = 0.5;

Vec3 wind(0, 0, 0);
GLuint tex1; GLuint tex2; GLuint tex3;
GLfloat delta = 0.0f;
float lastFrame = 0.0f;
/***** Below are functions Init(), display(), reshape(), keyboard(), arrow_keys(), main() *****/

/* This is where all the standard Glut/OpenGL stuff is, and where the methods of Cloth are called;
addForce(), windForce(), timeStep(), ballCollision(), and drawShaded()*/
GLuint LoadTexture(const char * filename, int width, int height) {
	GLuint texture;
	unsigned char * data;
	FILE * file;
	// read image
	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &texture); //generate the texture with the loaded data
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data); //free the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

void init(GLvoid)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.2f, 0.2f, 0.4f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightPos[4] = { -1.0,1.0,0.5,0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&lightPos);

	glEnable(GL_LIGHT1);

	GLfloat lightAmbient1[4] = { 0.0,0.0,0.0,0.0 };
	GLfloat lightPos1[4] = { 1.0,0.0,-0.2,0.0 };
	GLfloat lightDiffuse1[4] = { 0.5,0.5,0.3,0.0 };

	glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat *)&lightPos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat *)&lightAmbient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat *)&lightDiffuse1);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glEnable(GL_TEXTURE_2D);

	tex1 = LoadTexture("tex1.bmp", 256, 256);
	tex2 = LoadTexture("ground.bmp", 256, 128);
	tex3 = LoadTexture("pink.bmp", 1024, 682);
	
}


void drawPlane() {
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-60.0f, -25.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(60.0f, -25.0f, -20.0f);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(60.0f, 25.0f, -20.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-60.0f, 25.0f, -20.0f);
}

void drawPlane1() {
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-24.0f, -5.1f, -24.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(26.0f, -5.1f, -24.0f);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(26.0f, -5.1f, 26.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-24.0f, -5.1f, 26.0f);
}

/* display method called each frame*/
void display(void)
{
	// calculating positions
	GLfloat currentFrame = glutGet(GLUT_ELAPSED_TIME);
	delta = currentFrame - lastFrame;
	lastFrame = currentFrame;
	if (mo == 0) {
		if (change == 0) {
			cloth1.addForce(Vec3(0, -0.5, 0)*TIME_STEPSIZE2); // add gravity each frame, pointing down									  //cloth1.windForce(Vec3(0.5,0,0.2)*TIME_STEPSIZE2); // generate some wind each frame
															  //cloth1.windForce(wind * 5 * TIME_STEPSIZE2);
			cloth1.timeStep(); // calculate the object positions of the next frame
			cloth1.ballCollision(ball_pos, ball_radiusa); // resolve collision with the ball
			cloth1.ballCollision(ball_pos1, ball_radius1);
			cloth1.ballCollision(ball_pos2, ball_radius2);

			cloth1.cubeCollision(cube_center1, cube_edge1, 0);
			cloth1.cubeCollision(cube_center2, cube_edge2, 0);
			cloth1.selfCollision();
			cloth1.cubeCollision(cube_center, cube_edge, 1);
		}
		else {
			cloth1.addForce(Vec3(0, -0.5, 0)*TIME_STEPSIZE2); // add gravity each frame, pointing down									  //cloth1.windForce(Vec3(0.5,0,0.2)*TIME_STEPSIZE2); // generate some wind each frame
															  //cloth1.windForce(wind * 5 * TIME_STEPSIZE2);
			cloth1.timeStep(); // calculate the object positions of the next frame
			cloth1.selfCollision();
			cloth1.cubeCollision(cube_center, cube_edge, 1);

		}

	}
	else {
		cloth1.addForce(Vec3(0, -0.5, 0)*TIME_STEPSIZE2); // add gravity each frame, pointing down									  //cloth1.windForce(Vec3(0.5,0,0.2)*TIME_STEPSIZE2); // generate some wind each frame
		cloth1.windForce(wind * 5 * TIME_STEPSIZE2);
		cloth1.timeStep(); // calculate the object positions of the next frame
		cloth1.ballCollision(ball_pos, ball_radiusb); // resolve collision with the ball
													  //cloth1.cubeCollision(cube_center, cube_edge);
		cloth1.selfCollision();

	}

	// drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT || GL_TEXTURE_BINDING_2D);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING); // drawing some smooth shaded background - because I like it ;)
							/*glBegin(GL_POLYGON);
							glColor3f(0.8f, 0.8f, 1.0f);
							glVertex3f(-200.0f, -100.0f, -100.0f);
							glVertex3f(200.0f, -100.0f, -100.0f);
							glColor3f(0.4f, 0.4f, 0.8f);
							glVertex3f(200.0f, 100.0f, -100.0f);
							glVertex3f(-200.0f, 100.0f, -100.0f);
							glEnd();*/
	glEnable(GL_LIGHTING);

	glTranslatef(-2.0f, 2.5f, -7.0f); // move camera out and center on the cloth
	glRotatef(0, 0, 0, 0); // rotate a bit to see the cloth from the side
	cloth1.drawShaded(tex1);  // finally draw the cloth with smooth shading

	if (mo == 0) {
		if (change == 0) {
			glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
			glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z); // hence the translation of the sphere onto the ball position
			glColor3f(0.8f, 0.8f, 1.0f);

			glutSolidSphere(ball_radiusa - 0.1, 50, 50); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
			glPopMatrix();


			glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball

			glTranslatef(ball_pos1.x, ball_pos1.y, ball_pos1.z); // hence the translation of the sphere onto the ball position
			glColor3f(0.4f, 0.4f, 0.8f);

			glutSolidSphere(ball_radius1 - 0.1, 50, 50); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
			glPopMatrix();


			glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball

			glTranslatef(ball_pos2.x, ball_pos2.y, ball_pos2.z); // hence the translation of the sphere onto the ball position
			glColor3f(0.4f, 0.4f, 0.8f);

			glutSolidSphere(ball_radius2 - 0.1, 50, 50); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
			glPopMatrix();


			glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
			glTranslatef(cube_center1.x, cube_center1.y, cube_center1.z); // hence the translation of the sphere onto the ball position
			glColor3f(0.4f, 0.4f, 0.8f);
			glutSolidCube(cube_edge1 - 0.1); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
			glPopMatrix();

			glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
			glTranslatef(cube_center2.x, cube_center2.y, cube_center2.z); // hence the translation of the sphere onto the ball position
			glColor3f(0.8f, 0.8f, 1.0f);
			glutSolidCube(cube_edge2 - 0.1); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
			glPopMatrix();

			
		

			glPushMatrix(); // to draw the ball we use glutsolidsphere, and need to draw the sphere at the position of the ball
			glTranslatef(cube_center.x, cube_center.y, cube_center.z); // hence the translation of the sphere onto the ball position
		//	glColor3f(1.0f, 1.0f, 1.0f);
			glutSolidCube(cube_edge - 15); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
			glPopMatrix();
		
			
		glColor3f(1.0f, 1.0f, 1.0f);
			glDisable(GL_COLOR_MATERIAL);
			glEnable(GL_COLOR_MATERIAL);
			glPushMatrix();

			glBindTexture(GL_TEXTURE_2D, tex3); //bind texture
												//glColor3f(0.3f, 0.2f, 1.0f);
			glBegin(GL_QUADS);
			drawPlane();
			glEnd();
			glPopMatrix();

		}
		else {
			glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
			glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z); // hence the translation of the sphere onto the ball position
			glBindTexture(GL_TEXTURE_2D, tex3);

			glutSolidSphere(ball_radiusa - ball_radiusa, 50, 50); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
			glPopMatrix();
			glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
			glTranslatef(cube_center.x, cube_center.y, cube_center.z); // hence the translation of the sphere onto the ball position
			glBindTexture(GL_TEXTURE_2D, tex3);
			/*glBegin(GL_QUADS);
			drawPlane1();
			glEnd();*/
			glutSolidCube(cube_edge - 15); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
			glPopMatrix();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, tex3); //bind texture
												//glColor3f(0.3f, 0.2f, 1.0f);
			glBegin(GL_QUADS);
			drawPlane();
			glEnd();
			glPopMatrix();
		}

	}
	else {
		//glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
		//glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z); // hence the translation of the sphere onto the ball position
		////glColor3f(1.0f, 1.0f, 1.0f);
		//glutSolidSphere(ball_radius - 0.1, 50, 50); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
		//glPopMatrix();
		//glPushMatrix();
		//glBindTexture(GL_TEXTURE_2D, tex2); //bind texture
		//									//glColor3f(0.3f, 0.2f, 1.0f);
		//glBegin(GL_QUADS);
		//drawPlane();
		//glEnd();
		//glPopMatrix();
		glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
		glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z); // hence the translation of the sphere onto the ball position
														  //glColor3f(1.0f, 1.0f, 1.0f);
		glutSolidSphere(ball_radiusb - 0.1, 50, 50); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
		glPopMatrix();
		glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
		glTranslatef(cube_center.x, cube_center.y, cube_center.z); // hence the translation of the sphere onto the ball position
																   //glColor3f(1.0f, 1.0f, 1.0f);
		glutSolidCube(cube_edge - cube_edge); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
		glPopMatrix();
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, tex3); //bind texture
											//glColor3f(0.3f, 0.2f, 1.0f);
		glBegin(GL_QUADS);
		drawPlane();
		glEnd();
		glPopMatrix();
	}


	//
	//drawPlane();

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h == 0)
		gluPerspective(80, (float)w, 1.0, 5000.0);
	else
		gluPerspective(80, (float)w / (float)h, 1.0, 5000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	GLfloat speed = 0.02f *delta;
	if (key == 'a') {
		ball_pos = ball_pos + Vec3(-0.05, 0, 0)*speed;
	}
	if (key == 'd') {
		ball_pos = ball_pos + Vec3(0.05, 0, 0)*speed;
	}
	if (key == 'w') {
		ball_pos = ball_pos + Vec3(0, 0, -0.05)*speed;
	}
	if (key == 's') {
		ball_pos = ball_pos + Vec3(0, 0, 0.05)*speed;
	}
	if (key == '0') {
		wind = Vec3(0, 0, 0);
	}
	if (key == '1') {
		wind = Vec3(1, 0, 1);
	}
	if (key == '2') {
		wind = Vec3(5, 0, 5);
	}
	if (key == '3') {
		wind = Vec3(10, 0, 10);
	}
	/*if (key == 'q') {
	ball_pos = ball_pos + Vec3(0, 0.05, 0)*speed;
	}
	if (key == 'e') {
	ball_pos = ball_pos + Vec3(0, -0.05, 0)*speed;
	}*/
	if (key == 'j') {
		cube_center = cube_center + Vec3(-0.05, 0, 0)*speed;
	}
	if (key == 'l') {
		cube_center = cube_center + Vec3(0.05, 0, 0)*speed;
	}
	if (key == 'i') {
		cube_center = cube_center + Vec3(0, 0, -0.05)*speed;
	}
	if (key == 'k') {
		cube_center = cube_center + Vec3(0, 0, 0.05)*speed;
	}
	if (key == 'r') {
		cloth1 = Cloth::Cloth(4, 4, 50, 50, mo);
	}
	if (key == 'c') {
		change = 1 - change;
	}
	if (key == 'm') {
		mo = 1 - mo;
		cloth1 = Cloth::Cloth(4, 4, 50, 50, mo);
	}
	switch (key) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void arrow_keys(int a_keys, int x, int y)
{
	GLfloat speed = 0.02f *delta;
	switch (a_keys) {
	case GLUT_KEY_UP:
		//glutFullScreen();
		ball_pos = ball_pos + Vec3(0, 0.05, 0)*speed;
		cube_center = cube_center + Vec3(0, 0.05, 0)*speed;
		break;
	case GLUT_KEY_DOWN:
		//glutReshapeWindow(1280, 720);
		ball_pos = ball_pos + Vec3(0, -0.05, 0)*speed;
		cube_center = cube_center - Vec3(0, 0.05, 0)*speed;
		break;
	case GLUT_KEY_F1:
		glutFullScreen();
		break;
	case GLUT_KEY_F2:
		glutReshapeWindow(1280, 720);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);


	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);

	glutCreateWindow("Cloth Tutorial from cg.alexandra.dk");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);

	glutMainLoop();
}