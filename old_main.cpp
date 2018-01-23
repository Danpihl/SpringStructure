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

#include <IL/il.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

#include "colors.h"
#include "functions.h"
#include "SpringStructure.h"
#include "opengl_stuff.h"

#define DEFAULT_WIDTH  640
#define DEFAULT_HEIGHT 480
 
int width  = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;

void processNormalKeys(unsigned char key, int x, int y);
void processNormalKeysRelease(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);

void renderScene(void);
void draw_icosphere();
int LoadImage(char *filename);

using namespace std;

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
  
  //conn_file_name = base_path + "conn_rand.txt";
  //pos_file_name = base_path + "pos_rand.txt";

  //conn_file_name = base_path + "conn_sphere_163.txt";
  //pos_file_name = base_path + "pos_sphere_163.txt";

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
  float c = 3.0f;     // Exterior dampening
  float ca = 0.0f;      // Air resistance
  float m = 1.0f;       // Exterior mass
  float x0 = 0.0f;
  float y0 = 14.0f;
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
  
	// OpenGL init
	glEnable(GL_DEPTH_TEST);

  glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);

  //set length of one complete row in destination data (doesn't need to equal img.cols)
  glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

  ilInit();
  GLuint texid;
  ILuint image = LoadImage("../floor2.jpg");
  if ( image == -1 )
  {
      cout << "Can't load picture file by DevIL \n";
      return -1;
  }

  
  /*glGenTextures(1, &texid);
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 
  0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData()); */

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}


void processNormalKeysRelease(unsigned char key, int x, int y) {
  
  if((key == 'i') || (key == 'k')) {
    dy = 0.0f;
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
    ss.update();
    //start_record();
    
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
    v0[0] = 3.0f;
    v0[1] = 0.0f;
    v0[2] = 1.0f;
    ss.set_velocity(v0, 20.0f);
  }
  else if(key == 'c') {
    //reset velocities and positions
     ss.reset_structure();
  }
  else if(key == 'i') {
    dy = dy + 0.01f;
  }
  else if(key == 'k') {
    dy = dy - 0.01f;
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
    if(lim_ < 20) {
      for(i = 1; i < lim_+1; i++) {
        cidx = ss.conn[k*ss.max_length + i];
        if(cidx != 162) {
          glBegin(GL_LINES);

          glVertex3f(ss.P[k], ss.P[ss.N+k], ss.P[2*ss.N+k]);
          glVertex3f(ss.P[cidx], ss.P[ss.N+cidx], ss.P[2*ss.N+cidx]);

          glEnd();
        }
      }
    }

  }
  
}



void renderScene(void) {

  #define world_edge 20.0f
  draw_world(world_edge);

  glPushMatrix();
  glTranslatef(0.0, 0.0, 0.0);

  draw_icosphere();
  glPopMatrix();
  
  glutSwapBuffers();

  /*if(is_recording) {
    glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);
    cv::imwrite("/Users/danielpihlquist/programming_projects/bouncy_ball/images/from_simulation/image_" + to_string(img_idx) + ".png", img);
    img_idx = img_idx + 1;
  }*/

  //int w_ = glutGet(GLUT_WINDOW_WIDTH);
  //int h_ = glutGet(GLUT_WINDOW_HEIGHT);

  //cout << "Width: " << w_ << ", height: " << h_ << endl;

  
}

int LoadImage(char *filename)
{
    ILboolean success; 
     ILuint image; 
 
    ilGenImages(1, &image); /* Generation of one image name */
     ilBindImage(image); /* Binding of image name */
     success = ilLoadImage(filename); /* Loading of the image filename by DevIL */
 
    if (success) /* If no error occured: */
    {
        /* Convert every colour component into unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
           success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); 
 
        if (!success)
           {
                 return -1;
           }
    }
    else
        return -1;
 
    return image;
}