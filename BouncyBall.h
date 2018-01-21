#ifndef BOUNCY_BALL_H
#define BOUNCY_BALL_H

class BouncyBall {

public:

  float m;
  float C;
  float K;
  float g;
  float h;

  float *P;
  float *V;
  float *A;
  float *Vp;
  float *Vpc;
  float F;
  float R;

  float *Ls;    // Length between connections

  int *conn;
  int N;

  // Center point:
  float *Pc;  // Position
  float *Vc;  // Velocity
  float *Ac;  // Acceleration
  float Rc;  // Radii to other points
  
  float vc[3];  // Vectors to other points

  float Fc[3];  // Force to other points
  float Lc;   // Length to center
  float Kc;   // Spring constant
  float Cc;   // Dampening constant center
  float mc;   // Mass
  
  float Ca;   // Air resistance

  float Ft;

  float vp[3];
  float v[3];
  float a[3];
  float ac[3];

  int idx_0, idx_1, idx_2, cidx;
  int lim;
  float r0;

  int i, k;

  float friction_threshold;
  float speed_threshold;
  float mu;
  float Nf;
  float v_speed;
  float v_0, v_2;

  float r1, r2;
  float vproj[3];
  float vproj_center[3];

  void update();
  void set_velocity(float dir[3], float amp);
  BouncyBall(int *conn_, float *P_, int N_, float m_,
             float C_, float K_, float h_,
             float Kc_, float Cc_, float mc_, float Lc_, float Ca_,
             float x0, float y0, float z0);
  BouncyBall();

};



#endif