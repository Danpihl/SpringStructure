#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <math.h>

#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>

#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilut.h>
#include <IL/ilu.h>
#include <IL/ilu_region.h>
#include <IL/ilut_config.h>
#include <IL/devil_internal_exports.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

#include "colors.h"
#include "functions.h"
#include "SpringStructure.h"
#include "opengl_stuff.h"

void processNormalKeys(unsigned char key, int x, int y);
void processNormalKeysRelease(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);

void renderScene(void);
void draw_icosphere();

using namespace std;

GLuint texture[1];
ILuint imageID[1];

string record_string;
string write_string;
bool is_recording = false;

SpringStructure ss;

int img_idx = 0;
cv::Mat img;

std::string base_path("/Users/danielpihlquist/programming_projects/SpringStructure/matlab/");

std::string conn_file_name("");
std::string pos_file_name("");

std::vector<SpringStructure> structures;

int main(int argc, char **argv) {

  //conn_file_name = base_path + "conn_sphere_163.txt";
  //pos_file_name = base_path + "pos_sphere_163.txt";

  //conn_file_name = base_path + "conn_rand.txt";
  //pos_file_name = base_path + "pos_rand.txt";

  //conn_file_name = base_path + "conn_cube_125.txt";
  //pos_file_name = base_path + "pos_cube_125.txt";

  //conn_file_name = base_path + "conn_lattice.txt";
  //pos_file_name = base_path + "pos_lattice.txt";

  //conn_file_name = base_path + "conn_snake.txt";
  //pos_file_name = base_path + "pos_snake.txt";

  conn_file_name = base_path + "conn_wheel.txt";
  pos_file_name = base_path + "pos_wheel.txt";

  //conn_file_name = base_path + "conn_ball_chain.txt";
  //pos_file_name = base_path + "pos_ball_chain.txt";

  // Simulation parameters:
  float c = 4.0f;     // Exterior dampening
  float ca = 0.1f;      // Air resistance
  float m = 0.5f;       // Exterior mass
  float x0 = 0.0f;
  float y0 = 5.0f;
  float z0 = 0.0f;
  float K = 100000.0f;  // Exterior spring constant
  float h = 0.001f;     // Time step
  
  ss = SpringStructure(conn_file_name, pos_file_name, h, c, K, m, ca, x0, y0, z0);
  //ss.update();
  //Width: 1440, height: 804
  img.create(804, 1440, CV_8UC3);

  // -------------- INITIALIZE GL STUFF --------------
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(600, 200);
	glutInitWindowSize(1200, 600);
	glutCreateWindow("Spring structure simulation");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutSpecialFunc(pressKey);
  
	// here are the new entries
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKey);
  glutKeyboardFunc(processNormalKeys);
  glutKeyboardUpFunc(processNormalKeysRelease);
  glEnable(GL_SMOOTH);		// Enable (gouraud) shading
	glEnable(GL_DEPTH_TEST);	// Enable the depth testing
	glDepthFunc(GL_LEQUAL);		// Set our depth function to overwrite if new value less than or equal to current value
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Ask for nicest perspective correction
	glEnable(GL_TEXTURE_2D);	// Enable 2D textures

  glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);

  //set length of one complete row in destination data (doesn't need to equal img.cols)
  glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}


void processNormalKeysRelease(unsigned char key, int x, int y) {
  
  if((key == 'i') || (key == 'k')) {
    dy = 0.0f;
  }
  if((key == 'o') || (key == 'l')) {
    de_ = 0.0f;
  }
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
  else if(key == 'j') {
    for(int k = 0; k < ss.N; k++)
      if(k != (ss.N-1))
        ss.V[ss.N + k] = ss.V[ss.N + k] + 10.0f;
    
  }
  else if(key == 'r') {
    is_recording = !is_recording;
  }
  else if(key == 'w') {
    float v0[3];
    v0[0] = 1.0f;
    v0[1] = 3.0f;
    v0[2] = 0.0f;
    ss.set_velocity(v0, 20.0f);
  }
  else if(key == 'a') {
    float v0[3];
    v0[0] = -1.0f;
    v0[1] = 3.0f;
    v0[2] = 0.0f;
    ss.set_velocity(v0, 20.0f);
  }
  else if(key == 's') {
    float v0[3];
    v0[0] = 0.0f;
    v0[1] = 3.0f;
    v0[2] = -1.0f;
    ss.set_velocity(v0, 20.0f);
  }
  else if(key == 'd') {
    float v0[3];
    v0[0] = 0.0f;
    v0[1] = 3.0f;
    v0[2] = 1.0f;
    ss.set_velocity(v0, 20.0f);
  }
  else if(key == 'c') {
    //reset velocities and positions
     ss.reset_structure();
  }
  else if(key == 'i') {
    dy = dy - 0.1f;
  }
  else if(key == 'k') {
    dy = dy + 0.1f;
  }
  else if(key == 'o') {
    de_ = de_ + 0.01f;
  }
  else if(key == 'l') {
    de_ = de_ - 0.01f;
  }
  else if(key == 'u') {
    for(int rr = 0; rr < 10; rr++)
      ss.update();
  }
  
}

void draw_icosphere() {

  int k, i;
  
  int cidx;
  for(int rr = 0; rr < 10; rr++)
    ss.update();

  glColor3f(0.3f, 0.5f, 0.75f);
  //glColor3f(1.0f, 0.5f, 0.25f);

  for(k = 0; k < ss.N; k++) {
    glPushMatrix();
    glTranslatef(ss.P[k], ss.P[ss.N+k], 0.0f + ss.P[2*ss.N+k]);
    glutSolidSphere(0.05f, 20, 20);
    glPopMatrix();
  }

  glLineWidth(2.5);
  glColor3f(0.5f, 0.0f, 0.1f);
  int lim_;

  for(k = 0; k < ss.N; k++) {
    
    lim_ = ss.conn[k*ss.max_length];
  
    for(i = 1; i < lim_+1; i++) {
      cidx = ss.conn[k*ss.max_length + i];
      glBegin(GL_LINES);

      glVertex3f(ss.P[k], ss.P[ss.N+k], ss.P[2*ss.N+k]);
      glVertex3f(ss.P[cidx], ss.P[ss.N+cidx], ss.P[2*ss.N+cidx]);

      glEnd();
      
    }
  }
  
}



void renderScene(void) {

  if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);
  if(dy)
    Y = Y + dy;
  if(de_)
    E = E + de_;
  
  // Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset the matrix
	glMatrixMode(GL_MODELVIEW);
	
  #define world_edge 15.0f

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
  glTranslatef(0.0, Y, 0.0);
	// Set the camera
  gluLookAt(x, 1.0f, z,
				    x+lx, 1.0f+E,  z+lz,
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

  //glPushMatrix();
  //glTranslatef(0.0, Y, 0.0);

  draw_icosphere();
  //glPopMatrix();

	
	// ----- Stop Drawing Stuff! ------

	glutSwapBuffers(); // Swap the buffers to display the scene (so we don't have to watch it being drawn!)


  if(is_recording) {
    glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);
    cv::imwrite("/Users/danielpihlquist/programming_projects/SpringStructure/images/curr/image_" + to_string(img_idx) + ".png", img);
    img_idx = img_idx + 1;
  }

  //int w_ = glutGet(GLUT_WINDOW_WIDTH);
  //int h_ = glutGet(GLUT_WINDOW_HEIGHT);

  //cout << "Width: " << w_ << ", height: " << h_ << endl;

  
}
