#include "SpringStructure.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include "functions.h"

using namespace std;



void SpringStructure::update() {
  
  //calculate_radiis();
  /*int idx0;
  int idx1;
  
  for(int k = 0; k < Ne; k++) {   // Looping over edges
    
    idx0 = edge_conn[k*2 + 0];
    idx1 = edge_conn[k*2 + 1];
  
    xx = P[idx0] - P[idx1];
    yy = P[idx0 + N] - P[idx1 + N];
    zz = P[idx0 + N*2] - P[idx1 + N*2];

    Rs[k] = sqrt(xx*xx + yy*yy + zz*zz);

  }*/

  for(int k = 0; k < N; k++) {
    idx_0 = k;
    idx_1 = N + k;
    idx_2 = 2*N + k;

    vp[0] = P[idx_0];
    vp[1] = P[idx_1];
    vp[2] = P[idx_2];

    lim = conn[k*max_length];
    
    a[0] = 0.0f;
    a[1] = 0.0f;
    a[2] = 0.0f;

    A[idx_0] = 0.0f;
    A[idx_1] = 0.0f;
    A[idx_2] = 0.0f;

    Vp[idx_0] = 0.0f;
    Vp[idx_1] = 0.0f;
    Vp[idx_2] = 0.0f;

    //conn = [N x max_length] matrix
    //P = [3 x N] matrix

    for(int i = 0; i < lim; i++) {
      
      //cidx = conn[i*N + k];
      cidx = conn[k*max_length + i + 1];

      v[0] = vp[0] - P[cidx];          // difference vector x
      v[1] = vp[1] - P[N + cidx];      // difference vector y
      v[2] = vp[2] - P[2*N + cidx];    // difference vector z
      r0 = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
      
      R = sqrt(r0);
      //R = Rs[point_to_edge_index[k*max_connections + i]];
      
      v[0] = v[0]/R;
      v[1] = v[1]/R;
      v[2] = v[2]/R;

      F = K*(Ls[i*N + k] - R);
      
      a[0] = a[0] + F*v[0];
      a[1] = a[1] + F*v[1];
      a[2] = a[2] + F*v[2];

      A[idx_0] = A[idx_0] + F*v[0];
      A[idx_1] = A[idx_1] + F*v[1];
      A[idx_2] = A[idx_2] + F*v[2];
      
      // vp = ((vv'*vc)/(vc'*vc))*vc;
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
    //A[idx_0] = a[0];
    //A[idx_1] = a[1];
    //A[idx_2] = a[2];
    
  }   // Looping over k

  // ---------------- FRICTION ----------------
  
  for(int k = 0; k < N; k++) {
    
    idx_0 = k;
    idx_1 = N + k;
    idx_2 = 2*N + k;

    if(P[idx_1] < friction_threshold) {

      v_speed = sqrt(V[idx_0]*V[idx_0] + V[idx_2]*V[idx_2]);
      if(v_speed > speed_threshold) {
        vp[0] = P[idx_0];
        vp[1] = P[idx_1];
        vp[2] = P[idx_2];
        
        lim = conn[k*max_length];

        Nf = 0.0f;

        for(int i = 0; i < lim; i++) {

          cidx = conn[k*max_length + i+1];

          v[0] = vp[0] - P[cidx];          // difference vector x
          v[1] = vp[1] - P[N + cidx];      // difference vector y
          v[2] = vp[2] - P[2*N + cidx];    // difference vector z

          r0 = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
      
          R = sqrt(r0);

          //R = Rs[point_to_edge_index[k*max_connections + i]];
          v[0] = v[0]/R;
          v[1] = v[1]/R;
          v[2] = v[2]/R;

          F = K*(Ls[i*N + k] - R);
          
          Nf = Nf + F*v[1];

        }
        
        //Nf = Nf + m; // Keep?

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
  

  for(int k = 0; k < N; k++) {
    idx_0 = k;
    idx_1 = N + k;
    idx_2 = 2*N + k;

    //if(k != (N-1)) {

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

    //}
    // Floor
    if(P[idx_1] < 0.0f) {
      P[idx_1] = 0.0f;
      V[idx_1] = -V[idx_1];
    }

    #define room_limit 15.0f

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

void SpringStructure::reset_structure(void) {

  for(int c_ = 0; c_ < N; c_++) {
    V[c_]       = 0.0f;
    V[N + c_]   = 0.0f;
    V[2*N + c_] = 0.0f;
    P[c_] = iP[c_];
    P[N + c_] = iP[N + c_];
    P[2*N + c_] = iP[2*N + c_];

  }

}

void SpringStructure::set_velocity(float dir[3], float amp) {

  float r_ = sqrt(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]);
  dir[0] = amp*dir[0]/r_;
  dir[1] = amp*dir[1]/r_;
  dir[2] = amp*dir[2]/r_;

  for(int c_ = 0; c_ < N; c_++) {
    if(c_ != (N-1)) {
      V[c_]       = V[c_] + dir[0];
      V[N + c_]   = V[N + c_] + dir[1];
      V[2*N + c_] = V[2*N + c_] + dir[2];
    }

  }

}

void SpringStructure::calculate_radiis() {

  int idx0;
  int idx1;
  
  for(int k = 0; k < Ne; k++) {   // Looping over edges
    
    idx0 = edge_conn[k*2 + 0];
    idx1 = edge_conn[k*2 + 1];
  
    xx = P[idx0] - P[idx1];
    yy = P[idx0 + N] - P[idx1 + N];
    zz = P[idx0 + N*2] - P[idx1 + N*2];

    Rs[k] = sqrt(xx*xx + yy*yy + zz*zz);

  }

}

void SpringStructure::calculate_edges(int *conn) {
  
  int n;
  bool e = true;
  
  vector< vector<int> > ecg;
  vector< vector<int> > p2e_idx;

  int conn_idx;
  
  // Looping over all points
  for(int k = 0; k < N; k++) {
    
    // Number of connected points for current point
    n = conn[k*max_length];
    
    // Loop over every point that's connected to point k
    for(int i = 1; i < n+1; i++) {
      
      Ne = ecg.size();

      e = true;
      conn_idx = conn[k*max_length + i];

      for(int j = 0; j < Ne; j++) {
        
        vector<int> ce = ecg[j];

        if(  ( (ce[0] == k) && (ce[1] == conn_idx) ) || 
             ( (ce[1] == k) && (ce[0] == conn_idx) ) )
          e = false;
      }
      
      if(e) {
        
        vector<int> tv;
        tv.clear();
        tv.push_back(k);
        tv.push_back(conn_idx);
        ecg.push_back(tv);

      }
      
    }
    //p2e_idx.push_back(temp_edges);

  }


  edge_conn = (int*)malloc(sizeof(int)*2*ecg.size());
  
  for(int k = 0; k < ecg.size(); k++) {
    edge_conn[k*2 + 0] = ecg[k][0];
    edge_conn[k*2 + 1] = ecg[k][1];
  }

  Ne = ecg.size();

  for(int k = 0; k < N; k++) {
    
    vector < int > temp_edges;

    for(int i = 0; i < Ne; i++) {

      if (edge_conn[i*2 + 0] == k || edge_conn[i*2 + 1] == k) {
        temp_edges.push_back(i);
      }
    }
    p2e_idx.push_back(temp_edges);
  }

  max_connections = 0;

  for(int k = 0; k < p2e_idx.size(); k++) {
    
    if(p2e_idx[k].size() > max_connections)
      max_connections = p2e_idx[k].size();

    /*for(int i = 0; i < p2e_idx[k].size(); i++) {
      cout << p2e_idx[k][i] << ":";
    }
    cout << endl;*/
  }

  point_to_edge_index = (int*)malloc(sizeof(int)*N*max_connections);

  for(int k = 0; k < p2e_idx.size(); k++) {
    for(int i = 0; i < p2e_idx[k].size(); i++) {
      point_to_edge_index[k*max_connections + i] = p2e_idx[k][i];
    }
  }

  /*for(int k = 0; k < ecg.size(); k++) {
    cout << edge_conn[k*2 + 0] << ":" << edge_conn[k*2 + 1] << endl;
  }*/

  


}



SpringStructure::SpringStructure(std::string conn_file_name, std::string pos_file_name, 
                                 float h_, float C_, float K_, float m_, float Ca_,
                                 float x0, float y0, float z0) {

  std::vector< std::vector<int> > intdata = read_connectivity_graph(conn_file_name);

  max_length = 0;
  this->N = intdata.size();

  for(int k = 0; k < intdata.size(); k++) {
    if(intdata[k][0] > max_length)
      max_length = intdata[k][0];
  }
  
  max_length = max_length + 1;
  
  conn = (int*)malloc(sizeof(int)*N*max_length);
  
  for(int k = 0; k < this->N; k++) {
    for(int i = 0; i < intdata[k][0]+1; i++) {
      
      //conn = [N x max_length] matrix
      conn[k*max_length + i] = intdata[k][i];
      
    }
  }
  calculate_edges(conn);

  

  std::vector< vector<float> > positions = read_positions(pos_file_name);
  this->P = (float*)malloc(sizeof(float)*3*N);
  this->iP = (float*)malloc(sizeof(float)*3*N);
  this->V = (float*)calloc(3*N, sizeof(float));
  this->A = (float*)malloc(sizeof(float)*3*N);

  this->Vp = (float*)malloc(sizeof(float)*3*N);

  for(int k = 0; k < positions.size(); k++) {
    
    //P = [3 x N] matrix
  
    P[k] = positions[k][0];
    P[N + k] = positions[k][1];
    P[2*N + k] = positions[k][2];
    
  }

  Rs = (float*)malloc(sizeof(float)*Ne);

  this->m = m_;
  this->C = C_;
  this->K = K_;
  this->h = h_;
  this->Ca = Ca_;
  
  for(int k = 0; k < N; k++) {
    P[k] = P[k] + x0;
    P[N + k] = P[N + k] + y0;
    P[2*N + k] = P[2*N + k] + z0;
    
    iP[k] = P[k];
    iP[N + k] = P[N + k];
    iP[2*N + k] = P[2*N + k];

    V[k] = 0.0f;
    V[N + k] = 0.0f;
    V[2*N + k] = 0.0f;

  }

  this->g = 9.82*10.0f;
  //P[0] = P[0] + 0.01f;
  //P[N] = P[N] + 0.01f;
  //P[2*N] = P[2*N] + 0.01f;

  this->friction_threshold = 0.1f;
  this->speed_threshold = 0.001f;
  this->mu = 3.0f;
  this->Nf = 0.0f;
  // Need indexing vectors:
  // 1:
  // From point index to edge index
  // 2:
  // From edge index to point index

  //print_connectivity_graph(conn);

  this->Ls = (float*)malloc(sizeof(float)*(max_length-1)*N);
  
  for(int k = 0; k < N; k++) {
    idx_0 = k;
    idx_1 = N + k;
    idx_2 = 2*N + k;

    vp[0] = P[idx_0];
    vp[1] = P[idx_1];
    vp[2] = P[idx_2];

    lim = conn[k*max_length];
    
    //conn = [N x max_length] matrix

    for(int i = 1; i < lim+1; i++) {
      //cidx = conn[i*N + k];
      cidx = conn[k*max_length + i];
      v[0] = vp[0] - P[0*N + cidx];    // difference vector x
      v[1] = vp[1] - P[1*N + cidx];    // difference vector y
      v[2] = vp[2] - P[2*N + cidx];    // difference vector z
      Ls[(i-1)*N + k] = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
      //Ls[i*N + k] = 0.25f;
    }
  }

  

}

void SpringStructure::print_connectivity_graph(int *conn) {

  for(int k = 0; k < N; k++) {  
    for(int i = 0; i < conn[k*max_length]+1; i++) {
      std::cout << conn[k*max_length + i] << ":";
    }
    std::cout << std::endl;
  }

}

SpringStructure::SpringStructure() {

}