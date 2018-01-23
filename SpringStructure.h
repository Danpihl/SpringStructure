#ifndef SPRING_STRUCTUREL_H
#define SPRING_STRUCTUREL_H

#include <iostream>
#include <string>

class SpringStructure {

public:

  float m;
  float C;
  float K;
  float g;
  float h;

  float *iP;
  float *P;
  float *V;
  float *A;
  float *Vp;
  float F;
  float R;
  float *Rs;

  float *Ls;    // Length between connections

  int *conn;
  int N;

  // Center point:
  
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

  //int i, k;

  float friction_threshold;
  float speed_threshold;
  float mu;     // Friction
  float Nf;
  float v_speed;
  float v_0, v_2;

  int Ne;   // Number of edges

  float r0, r1, r2;
  float vproj[3];
  float vproj_center[3];
  int max_length;

  int *edge_conn;
  int *point_to_edge_index;
  int max_connections;
  float xx, yy, zz;

  void update();
  void set_velocity(float dir[3], float amp);
  void calculate_radiis();
  void calculate_edges(int *conn);
  SpringStructure(std::string conn_file_name, std::string pos_file_name, 
                                 float h_, float C_, float K_, float m_, float Ca_,
                                 float x0, float y0, float z0);
  void print_connectivity_graph(int *conn);
  void reset_structure(void);
  SpringStructure();

};


#endif