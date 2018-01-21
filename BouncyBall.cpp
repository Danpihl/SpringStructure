#include "BouncyBall.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

void BouncyBall::update() {
  

  Ac[0] = 0.0f;
  Ac[1] = 0.0f;
  Ac[2] = 0.0f;
  
  Vpc[0] = 0.0f;
  Vpc[1] = 0.0f;
  Vpc[2] = 0.0f;
  for(k = 0; k < N; k++) {
    idx_0 = k;
    idx_1 = N + k;
    idx_2 = 2*N + k;

    vp[0] = P[idx_0];
    vp[1] = P[idx_1];
    vp[2] = P[idx_2];

    if(conn[5*N + k] == -1) {    // 5 connections
      lim = 5;
    }
    else {                       // 6 connections
      lim = 6;
    }
    a[0] = 0.0f;
    a[1] = 0.0f;
    a[2] = 0.0f;

    Vp[idx_0] = 0.0f;
    Vp[idx_1] = 0.0f;
    Vp[idx_2] = 0.0f;

    for(i = 0; i < lim; i++) {

      cidx = conn[i*N + k]-1;

      v[0] = vp[0] - P[cidx];    // difference vector x
      v[1] = vp[1] - P[N + cidx];    // difference vector y
      v[2] = vp[2] - P[2*N + cidx];    // difference vector z
      r0 = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
      R = sqrt(r0);
      v[0] = v[0]/R;
      v[1] = v[1]/R;
      v[2] = v[2]/R;

      F = K*(Ls[i*N + k] - R);
      
      a[0] = a[0] + F*v[0];
      a[1] = a[1] + F*v[1];
      a[2] = a[2] + F*v[2];
      
      //vp = ((vv'*vc)/(vc'*vc))*vc;
      // Project velocities onto connection vector
      
      r1 = (V[idx_0]*v[0] + V[idx_1]*v[1] + V[idx_2]*v[2])/r0;
      r2 = (V[cidx]*v[0] + V[N + cidx]*v[1] + V[2*N + cidx]*v[2])/r0;
      r0 = r1-r2;
      vproj[0] = r0*v[0];
      vproj[1] = r0*v[1];
      vproj[2] = r0*v[2];

      Vp[idx_0] = Vp[idx_0] + vproj[0];
      Vp[idx_1] = Vp[idx_1] + vproj[1];
      Vp[idx_2] = Vp[idx_2] + vproj[2];

    }

    // Vector from point to center
    vc[0] = vp[0] - Pc[0];
    vc[1] = vp[1] - Pc[1];
    vc[2] = vp[2] - Pc[2];
    
    r0 = vc[0]*vc[0] + vc[1]*vc[1] + vc[2]*vc[2];  // Squared distance

    // vc is vector from center to point
    // Project outer point velocity and center point velocity onto vc

    // Center point velocity projected
    
    r1 = (Vc[0]*vc[0] + Vc[1]*vc[1] + Vc[2]*vc[2])/r0;
    r2 = (V[idx_0]*vc[0] + V[idx_1]*vc[1] + V[idx_2]*vc[2])/r0;
    r2 = r1-r2;
    vproj_center[0] = r2*vc[0];
    vproj_center[1] = r2*vc[1];
    vproj_center[2] = r2*vc[2];

    Vpc[0] = Vpc[0] + vproj_center[0];
    Vpc[1] = Vpc[1] + vproj_center[1];
    Vpc[2] = Vpc[2] + vproj_center[2];
    
    Rc = sqrt(vc[0]*vc[0] + vc[1]*vc[1] + vc[2]*vc[2]);

    vc[0] = vc[0]/Rc;
    vc[1] = vc[1]/Rc;
    vc[2] = vc[2]/Rc;
    
    Ft = Kc*(Lc - Rc);

    Fc[0] = Ft*vc[0];
    Fc[1] = Ft*vc[1];
    Fc[2] = Ft*vc[2];
    
    A[idx_0] = a[0] + Fc[0] + vproj_center[0]*Cc;
    A[idx_1] = a[1] + Fc[1] + vproj_center[1]*Cc;
    A[idx_2] = a[2] + Fc[2] + vproj_center[2]*Cc;

    Ac[0] = Ac[0] - Fc[0];
    Ac[1] = Ac[1] - Fc[1];
    Ac[2] = Ac[2] - Fc[2];

  }   // Looping over k

  Ac[0] = (Ac[0] - Ca*Vc[0] - Vpc[0]*Cc)/mc;
  Ac[1] = (Ac[1] - Ca*Vc[1] - Vpc[1]*Cc)/mc - g;
  Ac[2] = (Ac[2] - Ca*Vc[2] - Vpc[2]*Cc)/mc;

  Vc[0] = Vc[0] + h*Ac[0];
  Vc[1] = Vc[1] + h*Ac[1];
  Vc[2] = Vc[2] + h*Ac[2];

  Pc[0] = Pc[0] + h*Vc[0];
  Pc[1] = Pc[1] + h*Vc[1];
  Pc[2] = Pc[2] + h*Vc[2];

  // ---------------- FRICTION ----------------
  
  for(k = 0; k < N; k++) {
    
    idx_0 = k;
    idx_1 = N + k;
    idx_2 = 2*N + k;

    if(P[idx_1] < friction_threshold) {

      v_speed = sqrt(V[idx_0]*V[idx_0] + V[idx_2]*V[idx_2]);
      if(v_speed > speed_threshold) {
        vp[0] = P[idx_0];
        vp[1] = P[idx_1];
        vp[2] = P[idx_2];
        if(conn[5*N + k] == -1)    // 5 connections
          lim = 5;
        else                       // 6 connections
          lim = 6;

        Nf = 0.0f;
        for(i = 0; i < lim; i++) {

          cidx = conn[i*N + k]-1;

          v[0] = vp[0] - P[cidx];          // difference vector x
          v[1] = vp[1] - P[N + cidx];      // difference vector y
          v[2] = vp[2] - P[2*N + cidx];    // difference vector z
          r0 = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
          R = sqrt(r0);
          v[1] = v[1]/R;

          F = K*(Ls[i*N + k] - R);
          
          Nf = Nf + F*v[1];

        }
        Nf = a[1];
        
        if(Nf > 0.0f) {
          v_0 = V[idx_0]/v_speed;
          v_2 = V[idx_2]/v_speed;
          
          A[idx_0] = A[idx_0] - v_0*mu*Nf;
          A[idx_2] = A[idx_2] - v_2*mu*Nf;  
        }
      }
    }   // If P[idx_1] < friction_threshold

  } // Looping over k for friction

  


  // ---------------- FRICTION ----------------
  


  if(Pc[1] < 0.0f) {
    Pc[1] = 0.0f;
    Vc[1] = -Vc[1];
  }

  for(int k = 0; k < N; k++) {
    idx_0 = k;
    idx_1 = N + k;
    idx_2 = 2*N + k;
    A[idx_0] = (A[idx_0] - Ca*V[idx_0] - C*Vp[idx_0])/m;
    A[idx_1] = (A[idx_1] - Ca*V[idx_1] - C*Vp[idx_1])/m;
    A[idx_2] = (A[idx_2] - Ca*V[idx_2] - C*Vp[idx_2])/m;

    A[idx_1] = A[idx_1] - g;

    V[idx_0] = V[idx_0] + h*A[idx_0];
    V[idx_1] = V[idx_1] + h*A[idx_1];
    V[idx_2] = V[idx_2] + h*A[idx_2];

    P[idx_0] = P[idx_0] + h*V[idx_0];
    P[idx_1] = P[idx_1] + h*V[idx_1];
    P[idx_2] = P[idx_2] + h*V[idx_2];

    if(P[idx_1] < 0.0f) {
      P[idx_1] = 0.0f;
      V[idx_1] = -V[idx_1];
    }

    #define room_limit 10.0f

    if(P[idx_0] < -room_limit) {
      P[idx_0] = -room_limit;
      V[idx_0] = -V[idx_0];
    }
    else if(P[idx_0] > room_limit) {
      P[idx_0] = room_limit;
      V[idx_0] = -V[idx_0];
    }
    if(P[idx_2] < -room_limit) {
      P[idx_2] = -room_limit;
      V[idx_2] = -V[idx_2];
    }
    else if(P[idx_2] > room_limit) {
      P[idx_2] = room_limit;
      V[idx_2] = -V[idx_2];
    }
  }


}

void BouncyBall::set_velocity(float dir[3], float amp) {

  float r_ = sqrt(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]);
  dir[0] = amp*dir[0]/r_;
  dir[1] = amp*dir[1]/r_;
  dir[2] = amp*dir[2]/r_;

  for(int c = 0; c < N; c++) {

    V[c]       = V[c] + dir[0];
    V[N + c]   = V[N + c] + dir[1];
    V[2*N + c] = V[2*N + c] + dir[2];

  }

}

BouncyBall::BouncyBall() {

}

BouncyBall::BouncyBall(int *conn_, float *P_, int N_, float m_,
                       float C_, float K_, float h_,
                       float Kc_, float Cc_, float mc_, float Lc_, float Ca_,
                       float x0, float y0, float z0) {

  this->m = m_;
  this->C = C_;
  this->K = K_;
  this->h = h_;

  this->N = N_;

  this->P = (float*)malloc(sizeof(float)*3*N_);
  this->A = (float*)malloc(sizeof(float)*3*N_);

  this->conn = (int*)malloc(sizeof(int)*6*N_);

  this->Pc = (float*)malloc(sizeof(float)*3);
  this->Vc = (float*)malloc(sizeof(float)*3);
  this->Ac = (float*)malloc(sizeof(float)*3);
  this->Vp = (float*)malloc(sizeof(float)*3*N_);
  this->Vpc = (float*)malloc(sizeof(float)*3);

  this->Vc[0] = 0.0f;
  this->Vc[1] = 0.0f;
  this->Vc[2] = 0.0f;

  this->Pc[0] = 0.0f;
  this->Pc[1] = 4.0f;
  this->Pc[2] = 0.0f;
  
  for(int c = 0; c < N; c++) {
    this->P[0*N + c] = P_[0*N + c] + x0;
    this->P[1*N + c] = P_[1*N + c] + y0;
    this->P[2*N + c] = P_[2*N + c] + z0;
  }

  for(int k = 0; k < 6*N_; k++)
    this->conn[k] = conn_[k];

  this->V = (float*)calloc(3*N_, sizeof(float));
  
  /*for(int r = 0; r < 3; r++) {
    for(int c = 0; c < N_; c++) {
      if(r == 0)
        this->V[r*N_ + c] = -1.0f;
      else
        this->V[r*N_ + c] = 0.0f;
    }
  }*/

  this->Ca = Ca_;

  // Lengths of connection, at least 5, at most 6 per point
  this->Ls = (float*)malloc(sizeof(float)*6*N_);
  
  for(int k = 0; k < N_; k++) {
    idx_0 = k;
    idx_1 = N + k;
    idx_2 = 2*N + k;
    vp[0] = P[idx_0];
    vp[1] = P[idx_1];
    vp[2] = P[idx_2];

    if(conn[5*N_ + k] == -1)
      lim = 5;
    else
      lim = 6;
    for(int i = 0; i < lim; i++) {
      cidx = conn[i*N + k]-1;
      v[0] = vp[0] - P[0*N + cidx];    // difference vector x
      v[1] = vp[1] - P[1*N + cidx];    // difference vector y
      v[2] = vp[2] - P[2*N + cidx];    // difference vector z
      Ls[i*N_ + k] = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);    
    }
  }

  this->g = 9.82*10.0f;
  
  this->Kc = Kc_;   // Spring constant
  this->Cc = Cc_;   // Dampening constant
  this->mc = mc_;   // Mass
  this->Lc = Lc_;   // Length

  this->friction_threshold = 0.1f;
  this->speed_threshold = 0.01f;
  this->mu = 2.0f;
  this->Nf = 0.0f;

}