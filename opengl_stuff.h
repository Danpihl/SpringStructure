#ifndef OPENGL_STUFF_H
#define OPENGL_STUFF_H

extern float angle;
extern float lx;
extern float lz;
extern float x;
extern float z;
extern float deltaAngle;
extern float deltaMove;

void pressKey(int key, int xx, int yy);
void releaseKey(int key, int x, int y);
void computeDir(float deltaAngle);
void computePos(float deltaMove);
void changeSize(int w, int h);
void draw_world(int world_edge);

#endif