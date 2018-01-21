#include "opengl_stuff.h"
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#include <math.h>

float angle = 0.0f;
float lx=0.0f;
float lz=-1.0f;
float x=0.0f;
float z=5.0f;
float deltaAngle = 0.0f;
float deltaMove = 0;

void draw_world(int world_edge) {


	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, 1.0f, z,
				    x+lx, 1.0f,  z+lz,
				    0.0f, 1.0f,  0.0f);
  //gluLookAt(camera.x, camera.y, camera.z,  lookat.x, lookat.y, lookat.z, 0, 1, 0)
  
  // Draw ground
  
	
	glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-world_edge, 0.0f, -world_edge);
		glVertex3f(-world_edge, 0.0f,  world_edge);
    glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f( world_edge, 0.0f,  world_edge);
		glVertex3f( world_edge, 0.0f, -world_edge);
	glEnd();

  glColor3f(1.0f, 0.0f, 0.0f);
  glLineWidth(2.5); 
  glBegin(GL_LINES);
    glVertex3f(-10.0f, 0.02f, -10.0f);
    glVertex3f(10.0f, 0.02f, -10.0f);
    glVertex3f(10.0f, 0.02f, 10.0f);
    glVertex3f(-10.0f, 0.02f, 10.0f);
    glVertex3f(-10.0f, 0.02f, -10.0f);

    glVertex3f(-10.0f, 0.02f, -10.0f);
    glVertex3f(-10.0f, 0.02f, 10.0f);
    glVertex3f(10.0f, 0.02f, 10.0f);
    glVertex3f(10.0f, 0.02f, -10.0f);
    glVertex3f(-10.0f, 0.02f, -10.0f);
  glEnd();
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT : deltaAngle = -0.05f; break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.05f; break;
		case GLUT_KEY_UP : deltaMove = 1.5f; break;
		case GLUT_KEY_DOWN : deltaMove = -1.5f; break;
	}
  

}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
		case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

void computeDir(float deltaAngle) {
	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void setup_opengl(void) {


}