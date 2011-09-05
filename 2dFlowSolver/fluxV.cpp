#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
 
using namespace std;
 
#include "area.h"

// Subroutine fluxV returns the viscous flux
vector<double> fluxV(int n1, int n2, int n3,
                     vector< vector<double> > &node,
                     vector< vector<double> > &Q,
                     vector<double> &Qfree,
                     double mu_inf,
                     double M_inf,
                     double nx_face, double ny_face, 
                     double len)
{
  double nx_hat, ny_hat;   // unit normals 
  double u, v;             // velocities
  double tau_xx, tau_yy;   // normal stresses
  double tau_xy;           // shear stress tau_xy = tau_yx
  double q_x, q_y;         // heat transfer
  double A;                // triangle area
  double u1, u2, u3;       // x velocity at the vertices
  double v1, v2, v3;       // y velocity at the vertices
  double p1, p2, p3;       // Pressure at the vertices
  double a2_1, a2_2, a2_3; // Speed of sound squared at the vertices
  double u_x, u_y;         // du/dx, du/dy   x velocity gradients
  double v_x, v_y;         // dv/dx, dv/dy   y velocity gradients
  double a2_x, a2_y;       // da2/dx, da2/dy sound-speed-squared gradients
  double mu    = mu_inf;   // viscosity
  double Re    = 5000.0;   // Reynolds Number
  double Pr    = 0.72;     // Prandtl Number
  double gamma = 1.4;      // ratio of specific heats

  vector<double> fvisc;
  vector<double> nx;     // cell normals in the x direction
  vector<double> ny;     // cell normals in the y direction
 
  //--- Calculate Cell Gradients --------------------------------------------80
  A = fabs( area(node[n1][0],node[n1][1],node[n2][0],node[n2][1],
                 node[n3][0],node[n3][1]) );

  nx.push_back(  node[n3][1] - node[n2][1] );
  nx.push_back(  node[n1][1] - node[n3][1] );
  nx.push_back(  node[n2][1] - node[n1][1] );
  ny.push_back( -(node[n3][0] - node[n2][0]) );
  ny.push_back( -(node[n1][0] - node[n3][0]) );
  ny.push_back( -(node[n2][0] - node[n1][0]) );

  u1 = Q[n1][1]/Q[n1][0]; v1 = Q[n1][2]/Q[n1][0];
  u2 = Q[n2][1]/Q[n2][0]; v2 = Q[n2][2]/Q[n2][0];
  u3 = Q[n3][1]/Q[n3][0]; v3 = Q[n3][2]/Q[n3][0];
  p1 = (Q[n1][3]-0.5*Q[n1][0]*(u1*u1 + v1*v1))*(gamma-1.0); 
  p2 = (Q[n2][3]-0.5*Q[n2][0]*(u2*u2 + v2*v2))*(gamma-1.0); 
  p3 = (Q[n3][3]-0.5*Q[n3][0]*(u3*u3 + v3*v3))*(gamma-1.0); 
  a2_1 = gamma*p1/Q[n1][0];
  a2_2 = gamma*p2/Q[n2][0];
  a2_3 = gamma*p3/Q[n3][0];

  u_x  = 1.0/(2.0*A) * (u1*(nx[1]+nx[2]) + u2*(nx[0]+nx[2]) + u3*(nx[1]+nx[0]));
  u_y  = 1.0/(2.0*A) * (u1*(ny[1]+ny[2]) + u2*(ny[0]+ny[2]) + u3*(ny[1]+ny[0]));
  v_x  = 1.0/(2.0*A) * (v1*(nx[1]+nx[2]) + v2*(nx[0]+nx[2]) + v3*(nx[1]+nx[0]));
  v_y  = 1.0/(2.0*A) * (v1*(ny[1]+ny[2]) + v2*(ny[0]+ny[2]) + v3*(ny[1]+ny[0]));
  a2_x = 1.0/(2.0*A) * (a2_1*(nx[1]+nx[2]) + a2_2*(nx[0]+nx[2]) + 
                        a2_3*(nx[1]+nx[0]));
  a2_y = 1.0/(2.0*A) * (a2_1*(ny[1]+ny[2]) + a2_2*(ny[0]+ny[2]) + 
                        a2_3*(ny[1]+ny[0]));

  //--- Calculate Stresses --------------------------------------------------80
  tau_xx = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x - v_y);
  tau_yy = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y - u_x);
  tau_xy = mu * M_inf/Re * (u_y + v_x);

  //--- Calculate Heat Transfer ---------------------------------------------80
  q_x = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x;
  q_y = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y;

  //--- Calculate Viscous Flux ----------------------------------------------80
  nx_hat = nx_face/len; // x component of the unit normal vector for the face
  ny_hat = ny_face/len; // y component of the unit normal vector for the face

  u = (u1 + u2 + u3)/3.0; // x-velocity at the centroid
  v = (v1 + v2 + v3)/3.0; // y-velocity at the centroid

  fvisc.push_back( 0.0 );
  fvisc.push_back( len * (tau_xx*nx_hat + tau_xy*ny_hat) );
  fvisc.push_back( len * (tau_xy*nx_hat + tau_yy*ny_hat) );
  fvisc.push_back( len * ((u*tau_xx + v*tau_xy - q_x)*nx_hat +
                          (u*tau_xy + v*tau_yy - q_y)*ny_hat) ); 

  return fvisc;
}
