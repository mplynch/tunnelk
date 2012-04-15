//----------------------------------------------------------------------------80
// Subroutine dfluxV returns the viscous flux Jacobians with respect to the 
// three verticies of the triangle (dvisc1, dvisc2, dvisc3)
//----------------------------------------------------------------------------80

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
 
using namespace std;
 
#include "area.h"

void dfluxV(int n1, int n2, int n3,
            vector< vector<double> > &node,
            vector< vector<double> > &Q,
            vector<double> &Qfree,
            double mu_inf,
            double M_inf,
            double nx_face, double ny_face, 
            double len,
            vector< vector<double> > &dfvisc1,
            vector< vector<double> > &dfvisc2,
            vector< vector<double> > &dfvisc3)
{
  double nx_hat, ny_hat;   // unit normals for the face
  double tau_xx, tau_yy;   // normal stresses
  double tau_xy;           // shear stress tau_xy = tau_yx
  double q_x, q_y;         // heat transfer
  double mu;               // viscosity
  double A;                // triangle area
  double rho1, rho2, rho3; // density at the vertices
  double u1, u2, u3;       // x velocity at the vertices
  double v1, v2, v3;       // y velocity at the vertices
  double u, v;             // x and y velocities at the cell centroid
  double e1, e2, e3;       // energy at the vertices
  double p1, p2, p3;       // Pressure at the vertices
  double spd1, spd2, spd3; // Speed-squared at the vertices
  double a2_1, a2_2, a2_3; // Speed of sound squared at the vertices
  double u_x, u_y;         // du/dx, du/dy   x velocity gradients
  double v_x, v_y;         // dv/dx, dv/dy   y velocity gradients
  double a2_x, a2_y;       // da2/dx, da2/dy sound-speed-squared gradients
  double Pr    = 0.72;     // Prandtl Number
  double gamma = 1.4;      // ratio of specific heats
  double Re = 5000;        // Reynolds Number

  double rho1_Q11, rho1_Q21, rho1_Q31, rho1_Q41;           //density derivatives
  double rho2_Q11, rho2_Q21, rho2_Q31, rho2_Q41;
  double rho3_Q11, rho3_Q21, rho3_Q31, rho3_Q41;
  double rho1_Q12, rho1_Q22, rho1_Q32, rho1_Q42;
  double rho2_Q12, rho2_Q22, rho2_Q32, rho2_Q42;
  double rho3_Q12, rho3_Q22, rho3_Q32, rho3_Q42;
  double rho1_Q13, rho1_Q23, rho1_Q33, rho1_Q43;
  double rho2_Q13, rho2_Q23, rho2_Q33, rho2_Q43;
  double rho3_Q13, rho3_Q23, rho3_Q33, rho3_Q43;
  double u_Q11, u_Q21, u_Q31, u_Q41;                       //u derivatives
  double u_Q12, u_Q22, u_Q32, u_Q42;
  double u_Q13, u_Q23, u_Q33, u_Q43;
  double u1_Q11, u1_Q21, u1_Q31, u1_Q41;
  double u2_Q11, u2_Q21, u2_Q31, u2_Q41;
  double u3_Q11, u3_Q21, u3_Q31, u3_Q41;
  double u1_Q12, u1_Q22, u1_Q32, u1_Q42;
  double u2_Q12, u2_Q22, u2_Q32, u2_Q42;
  double u3_Q12, u3_Q22, u3_Q32, u3_Q42;
  double u1_Q13, u1_Q23, u1_Q33, u1_Q43;
  double u2_Q13, u2_Q23, u2_Q33, u2_Q43;
  double u3_Q13, u3_Q23, u3_Q33, u3_Q43;
  double v_Q11, v_Q21, v_Q31, v_Q41;                       //v derivatives
  double v_Q12, v_Q22, v_Q32, v_Q42;
  double v_Q13, v_Q23, v_Q33, v_Q43;
  double v1_Q11, v1_Q21, v1_Q31, v1_Q41;
  double v2_Q11, v2_Q21, v2_Q31, v2_Q41;
  double v3_Q11, v3_Q21, v3_Q31, v3_Q41;
  double v1_Q12, v1_Q22, v1_Q32, v1_Q42;
  double v2_Q12, v2_Q22, v2_Q32, v2_Q42;
  double v3_Q12, v3_Q22, v3_Q32, v3_Q42;
  double v1_Q13, v1_Q23, v1_Q33, v1_Q43;
  double v2_Q13, v2_Q23, v2_Q33, v2_Q43;
  double v3_Q13, v3_Q23, v3_Q33, v3_Q43;
  double e1_Q11, e1_Q21, e1_Q31, e1_Q41;                   //energy derivatives
  double e2_Q11, e2_Q21, e2_Q31, e2_Q41;
  double e3_Q11, e3_Q21, e3_Q31, e3_Q41;
  double e1_Q12, e1_Q22, e1_Q32, e1_Q42;
  double e2_Q12, e2_Q22, e2_Q32, e2_Q42;
  double e3_Q12, e3_Q22, e3_Q32, e3_Q42;
  double e1_Q13, e1_Q23, e1_Q33, e1_Q43;
  double e2_Q13, e2_Q23, e2_Q33, e2_Q43;
  double e3_Q13, e3_Q23, e3_Q33, e3_Q43;
  double spd1_Q11, spd1_Q21, spd1_Q31, spd1_Q41;           //speed-squared derivatives
  double spd2_Q11, spd2_Q21, spd2_Q31, spd2_Q41;
  double spd3_Q11, spd3_Q21, spd3_Q31, spd3_Q41;
  double spd1_Q12, spd1_Q22, spd1_Q32, spd1_Q42;
  double spd2_Q12, spd2_Q22, spd2_Q32, spd2_Q42;
  double spd3_Q12, spd3_Q22, spd3_Q32, spd3_Q42;
  double spd1_Q13, spd1_Q23, spd1_Q33, spd1_Q43;
  double spd2_Q13, spd2_Q23, spd2_Q33, spd2_Q43;
  double spd3_Q13, spd3_Q23, spd3_Q33, spd3_Q43;
  double p1_Q11, p1_Q21, p1_Q31, p1_Q41;                   //pressure derivatives
  double p2_Q11, p2_Q21, p2_Q31, p2_Q41;
  double p3_Q11, p3_Q21, p3_Q31, p3_Q41;
  double p1_Q12, p1_Q22, p1_Q32, p1_Q42;
  double p2_Q12, p2_Q22, p2_Q32, p2_Q42;
  double p3_Q12, p3_Q22, p3_Q32, p3_Q42;
  double p1_Q13, p1_Q23, p1_Q33, p1_Q43;
  double p2_Q13, p2_Q23, p2_Q33, p2_Q43;
  double p3_Q13, p3_Q23, p3_Q33, p3_Q43;
  double a2_1_Q11, a2_1_Q21, a2_1_Q31, a2_1_Q41;           //speed of sound squared derivatives
  double a2_2_Q11, a2_2_Q21, a2_2_Q31, a2_2_Q41;
  double a2_3_Q11, a2_3_Q21, a2_3_Q31, a2_3_Q41;
  double a2_1_Q12, a2_1_Q22, a2_1_Q32, a2_1_Q42;
  double a2_2_Q12, a2_2_Q22, a2_2_Q32, a2_2_Q42;
  double a2_3_Q12, a2_3_Q22, a2_3_Q32, a2_3_Q42;
  double a2_1_Q13, a2_1_Q23, a2_1_Q33, a2_1_Q43;
  double a2_2_Q13, a2_2_Q23, a2_2_Q33, a2_2_Q43;
  double a2_3_Q13, a2_3_Q23, a2_3_Q33, a2_3_Q43;
  double u_x_Q11, u_x_Q21, u_x_Q31, u_x_Q41;               //velocity spatial derivative
  double u_y_Q11, u_y_Q21, u_y_Q31, u_y_Q41;               //derivatives
  double v_x_Q11, v_x_Q21, v_x_Q31, v_x_Q41;
  double v_y_Q11, v_y_Q21, v_y_Q31, v_y_Q41;
  double u_x_Q12, u_x_Q22, u_x_Q32, u_x_Q42;
  double u_y_Q12, u_y_Q22, u_y_Q32, u_y_Q42;
  double v_x_Q12, v_x_Q22, v_x_Q32, v_x_Q42;
  double v_y_Q12, v_y_Q22, v_y_Q32, v_y_Q42;
  double u_x_Q13, u_x_Q23, u_x_Q33, u_x_Q43;
  double u_y_Q13, u_y_Q23, u_y_Q33, u_y_Q43;
  double v_x_Q13, v_x_Q23, v_x_Q33, v_x_Q43;
  double v_y_Q13, v_y_Q23, v_y_Q33, v_y_Q43;
  double a2_x_Q11, a2_x_Q21, a2_x_Q31, a2_x_Q41;           //speed-of-sound-squared
  double a2_y_Q11, a2_y_Q21, a2_y_Q31, a2_y_Q41;           //derivative derivatives
  double a2_x_Q12, a2_x_Q22, a2_x_Q32, a2_x_Q42;
  double a2_y_Q12, a2_y_Q22, a2_y_Q32, a2_y_Q42;
  double a2_x_Q13, a2_x_Q23, a2_x_Q33, a2_x_Q43;
  double a2_y_Q13, a2_y_Q23, a2_y_Q33, a2_y_Q43;
  double tau_xx_Q11, tau_xx_Q21, tau_xx_Q31, tau_xx_Q41;   //stress derivatives
  double tau_yy_Q11, tau_yy_Q21, tau_yy_Q31, tau_yy_Q41;
  double tau_xy_Q11, tau_xy_Q21, tau_xy_Q31, tau_xy_Q41;
  double tau_xx_Q12, tau_xx_Q22, tau_xx_Q32, tau_xx_Q42;
  double tau_yy_Q12, tau_yy_Q22, tau_yy_Q32, tau_yy_Q42;
  double tau_xy_Q12, tau_xy_Q22, tau_xy_Q32, tau_xy_Q42;
  double tau_xx_Q13, tau_xx_Q23, tau_xx_Q33, tau_xx_Q43;
  double tau_yy_Q13, tau_yy_Q23, tau_yy_Q33, tau_yy_Q43;
  double tau_xy_Q13, tau_xy_Q23, tau_xy_Q33, tau_xy_Q43;
  double q_x_Q11, q_x_Q21, q_x_Q31, q_x_Q41;               //heat transfer derivatives
  double q_y_Q11, q_y_Q21, q_y_Q31, q_y_Q41;           
  double q_x_Q12, q_x_Q22, q_x_Q32, q_x_Q42;
  double q_y_Q12, q_y_Q22, q_y_Q32, q_y_Q42;           
  double q_x_Q13, q_x_Q23, q_x_Q33, q_x_Q43;
  double q_y_Q13, q_y_Q23, q_y_Q33, q_y_Q43;           

  vector<double> nx; // cell normal in the x direction
  vector<double> ny; // cell normal in the y direction
  
  //--- Calculate Normals ----------------------------------------------------80
  nx.push_back(  node[n3][1] - node[n2][1] );
  nx.push_back(  node[n1][1] - node[n3][1] );
  nx.push_back(  node[n2][1] - node[n1][1] );
  ny.push_back( -(node[n3][0] - node[n2][0]) );
  ny.push_back( -(node[n1][0] - node[n3][0]) );
  ny.push_back( -(node[n2][0] - node[n1][0]) );

  //--- Density derivatives at the verticies ---------------------------------80
  rho1     = Q[n1][0]; 
  rho1_Q11 = 1.0;      rho1_Q12 = 0.0;   rho1_Q13 = 0.0;     
  rho1_Q21 = 0.0;      rho1_Q22 = 0.0;   rho1_Q23 = 0.0;     
  rho1_Q31 = 0.0;      rho1_Q32 = 0.0;   rho1_Q33 = 0.0;     
  rho1_Q41 = 0.0;      rho1_Q42 = 0.0;   rho1_Q43 = 0.0;     
  rho2     = Q[n2][0];
  rho2_Q11 = 0.0;      rho2_Q12 = 1.0;   rho2_Q13 = 0.0;     
  rho2_Q21 = 0.0;      rho2_Q22 = 0.0;   rho2_Q23 = 0.0;     
  rho2_Q31 = 0.0;      rho2_Q32 = 0.0;   rho2_Q33 = 0.0;     
  rho2_Q41 = 0.0;      rho2_Q42 = 0.0;   rho2_Q43 = 0.0;     
  rho3     = Q[n3][0];
  rho3_Q11 = 0.0;      rho3_Q12 = 0.0;   rho3_Q13 = 1.0;     
  rho3_Q21 = 0.0;      rho3_Q22 = 0.0;   rho3_Q23 = 0.0;     
  rho3_Q31 = 0.0;      rho3_Q32 = 0.0;   rho3_Q33 = 0.0;     
  rho3_Q41 = 0.0;      rho3_Q42 = 0.0;   rho3_Q43 = 0.0;     

  //--- x-velocity derivatives at the verticies -------------------------------80
  u1     = Q[n1][1]/Q[n1][0];     
  u1_Q11 = -Q[n1][1]/pow(Q[n1][0],2);  u1_Q12 = 0.;  u1_Q13 = 0.;
  u1_Q21 = 1.0/Q[n1][0];               u1_Q22 = 0.;  u1_Q23 = 0.;
  u1_Q31 = 0.0;                        u1_Q32 = 0.;  u1_Q33 = 0.;
  u1_Q41 = 0.0;                        u1_Q42 = 0.;  u1_Q43 = 0.;
  u2     = Q[n2][1]/Q[n2][0];      
  u2_Q12 = -Q[n2][1]/pow(Q[n2][0],2);  u2_Q11 = 0.;  u2_Q13 = 0.;
  u2_Q22 = 1.0/Q[n2][0];               u2_Q21 = 0.;  u2_Q23 = 0.;
  u2_Q32 = 0.0;                        u2_Q31 = 0.;  u2_Q33 = 0.;
  u2_Q42 = 0.0;                        u2_Q41 = 0.;  u2_Q43 = 0.;
  u3     = Q[n3][1]/Q[n3][0];       
  u3_Q13 = -Q[n3][1]/pow(Q[n3][0],2);  u3_Q12 = 0.;  u3_Q11 = 0.;
  u3_Q23 = 1.0/Q[n3][0];               u3_Q22 = 0.;  u3_Q21 = 0.;
  u3_Q33 = 0.0;                        u3_Q32 = 0.;  u3_Q31 = 0.;
  u3_Q43 = 0.0;                        u3_Q42 = 0.;  u3_Q41 = 0.;

  //--- y-velocity derivatives at the verticies ------------------------------80
  v1     = Q[n1][2]/Q[n1][0];          
  v1_Q11 = -Q[n1][2]/pow(Q[n1][0],2);  v1_Q12 = 0.;  v1_Q13 = 0.;
  v1_Q21 = 0.0;                        v1_Q22 = 0.;  v1_Q23 = 0.;
  v1_Q31 = 1.0/Q[n1][0];               v1_Q32 = 0.;  v1_Q33 = 0.;
  v1_Q41 = 0.0;                        v1_Q42 = 0.;  v1_Q43 = 0.;
  v2     = Q[n2][2]/Q[n2][0];         
  v2_Q12 = -Q[n2][2]/pow(Q[n2][0],2);  v2_Q11 = 0.;  v2_Q13 = 0.;
  v2_Q22 = 0.0;                        v2_Q21 = 0.;  v2_Q23 = 0.;
  v2_Q32 = 1.0/Q[n2][0];               v2_Q31 = 0.;  v2_Q33 = 0.;
  v2_Q42 = 0.0;                        v2_Q41 = 0.;  v2_Q43 = 0.;
  v3     = Q[n3][2]/Q[n3][0];        
  v3_Q13 = -Q[n3][2]/pow(Q[n3][0],2);  v3_Q12 = 0.;  v3_Q11 = 0.;
  v3_Q23 = 0.0;                        v3_Q22 = 0.;  v3_Q21 = 0.;
  v3_Q33 = 1.0/Q[n3][0];               v3_Q32 = 0.;  v3_Q31 = 0.;
  v3_Q43 = 0.0;                        v3_Q42 = 0.;  v3_Q41 = 0.;

  //--- energy derivatives at the verticies ----------------------------------80
  e1    = Q[n1][3]; 
  e1_Q11 = 0.0;                        e1_Q12 = 0.0;  e1_Q13 = 0.0;
  e1_Q21 = 0.0;                        e1_Q22 = 0.0;  e1_Q23 = 0.0;
  e1_Q31 = 0.0;                        e1_Q32 = 0.0;  e1_Q33 = 0.0;
  e1_Q41 = 1.0;                        e1_Q42 = 0.0;  e1_Q43 = 0.0;
  e2     = Q[n2][3];
  e2_Q12 = 0.0;                        e2_Q11 = 0.0;  e2_Q13 = 0.0;
  e2_Q22 = 0.0;                        e2_Q21 = 0.0;  e2_Q23 = 0.0;
  e2_Q32 = 0.0;                        e2_Q31 = 0.0;  e2_Q33 = 0.0;
  e2_Q42 = 1.0;                        e2_Q41 = 0.0;  e2_Q43 = 0.0;
  e3     = Q[n3][3];
  e3_Q13 = 0.0;                        e3_Q11 = 0.0;  e3_Q12 = 0.0;
  e3_Q23 = 0.0;                        e3_Q21 = 0.0;  e3_Q22 = 0.0;
  e3_Q33 = 0.0;                        e3_Q31 = 0.0;  e3_Q32 = 0.0;
  e3_Q43 = 1.0;                        e3_Q41 = 0.0;  e3_Q42 = 0.0;      

  //--- Speed squared at the vertices ----------------------------------------80
  spd1     = (pow(Q[n1][1],2) + pow(Q[n1][2],2))/pow(Q[n1][0],2);
  spd1_Q11 = -2.0*(pow(Q[n1][1],2) + pow(Q[n1][2],2))/pow(Q[n1][0],3); 
  spd1_Q21 = 2.0*Q[n1][1]/pow(Q[n1][0],2); 
  spd1_Q31 = 2.0*Q[n1][2]/pow(Q[n1][0],2); 
  spd1_Q41 = 0.0;
  spd2     = (pow(Q[n2][1],2) + pow(Q[n2][2],2))/pow(Q[n2][0],2);
  spd2_Q12 = -2.0*(pow(Q[n2][1],2) + pow(Q[n2][2],2))/pow(Q[n2][0],3); 
  spd2_Q22 = 2.0*Q[n2][1]/pow(Q[n2][0],2); 
  spd2_Q32 = 2.0*Q[n2][2]/pow(Q[n2][0],2); 
  spd2_Q42 = 0.0;
  spd3     = (pow(Q[n3][1],2) + pow(Q[n3][2],2))/pow(Q[n3][0],2);
  spd3_Q13 = -2.0*(pow(Q[n3][1],2) + pow(Q[n3][2],2))/pow(Q[n3][0],3); 
  spd3_Q23 = 2.0*Q[n3][1]/pow(Q[n3][0],2); 
  spd3_Q33 = 2.0*Q[n3][2]/pow(Q[n3][0],2); 
  spd3_Q43 = 0.0;

  spd1_Q12 = 0.0;  spd1_Q13 = 0.0;
  spd1_Q22 = 0.0;  spd1_Q23 = 0.0;
  spd1_Q32 = 0.0;  spd1_Q33 = 0.0;
  spd1_Q42 = 0.0;  spd1_Q43 = 0.0;

  spd2_Q11 = 0.0;  spd2_Q13 = 0.0;
  spd2_Q21 = 0.0;  spd2_Q23 = 0.0;
  spd2_Q31 = 0.0;  spd2_Q33 = 0.0;
  spd2_Q41 = 0.0;  spd2_Q43 = 0.0;

  spd3_Q11 = 0.0;  spd3_Q12 = 0.0;
  spd3_Q21 = 0.0;  spd3_Q22 = 0.0;
  spd3_Q31 = 0.0;  spd3_Q32 = 0.0;
  spd3_Q41 = 0.0;  spd3_Q42 = 0.0;

  //--- Pressure at the vertices ---------------------------------------------80
  p1     = (gamma-1.0)*(e1-0.5*rho1*spd1);
  p1_Q11 = (gamma-1.0)*(e1_Q11-0.5*rho1*spd1_Q11-0.5*spd1*rho1_Q11);
  p1_Q21 = (gamma-1.0)*(e1_Q21-0.5*rho1*spd1_Q21-0.5*spd1*rho1_Q21);
  p1_Q31 = (gamma-1.0)*(e1_Q31-0.5*rho1*spd1_Q31-0.5*spd1*rho1_Q31);
  p1_Q41 = (gamma-1.0)*(e1_Q41-0.5*rho1*spd1_Q41-0.5*spd1*rho1_Q41);
  p2     = (gamma-1.0)*(e2-0.5*rho2*spd2);
  p2_Q12 = (gamma-1.0)*(e2_Q12-0.5*rho2*spd2_Q12-0.5*spd2*rho2_Q12);
  p2_Q22 = (gamma-1.0)*(e2_Q22-0.5*rho2*spd2_Q22-0.5*spd2*rho2_Q22);
  p2_Q32 = (gamma-1.0)*(e2_Q32-0.5*rho2*spd2_Q32-0.5*spd2*rho2_Q32);
  p2_Q42 = (gamma-1.0)*(e2_Q42-0.5*rho2*spd2_Q42-0.5*spd2*rho2_Q42);
  p3     = (gamma-1.0)*(e3-0.5*rho3*spd3);
  p3_Q13 = (gamma-1.0)*(e3_Q13-0.5*rho3*spd3_Q13-0.5*spd3*rho3_Q13);
  p3_Q23 = (gamma-1.0)*(e3_Q23-0.5*rho3*spd3_Q23-0.5*spd3*rho3_Q23);
  p3_Q33 = (gamma-1.0)*(e3_Q33-0.5*rho3*spd3_Q33-0.5*spd3*rho3_Q33);
  p3_Q43 = (gamma-1.0)*(e3_Q43-0.5*rho3*spd3_Q43-0.5*spd3*rho3_Q43);

  p1_Q12 = 0.0;  p1_Q13 = 0.0;
  p1_Q22 = 0.0;  p1_Q23 = 0.0;
  p1_Q32 = 0.0;  p1_Q33 = 0.0;
  p1_Q42 = 0.0;  p1_Q43 = 0.0;

  p2_Q11 = 0.0;  p2_Q13 = 0.0;
  p2_Q21 = 0.0;  p2_Q23 = 0.0;
  p2_Q31 = 0.0;  p2_Q33 = 0.0;
  p2_Q41 = 0.0;  p2_Q43 = 0.0;

  p3_Q11 = 0.0;  p3_Q12 = 0.0;
  p3_Q21 = 0.0;  p3_Q22 = 0.0;
  p3_Q31 = 0.0;  p3_Q32 = 0.0;
  p3_Q41 = 0.0;  p3_Q42 = 0.0;

  //--- Speed of Sound squared -----------------------------------------------80
  a2_1     = gamma*p1/rho1;
  a2_1_Q11 = gamma*(1.0/rho1*p1_Q11 - p1/pow(rho1,2)*rho1_Q11);
  a2_1_Q21 = gamma*(1.0/rho1*p1_Q21 - p1/pow(rho1,2)*rho1_Q21);
  a2_1_Q31 = gamma*(1.0/rho1*p1_Q31 - p1/pow(rho1,2)*rho1_Q31);
  a2_1_Q41 = gamma*(1.0/rho1*p1_Q41 - p1/pow(rho1,2)*rho1_Q41);
  a2_2     = gamma*p2/rho2;
  a2_2_Q12 = gamma*(1.0/rho2*p2_Q12 - p2/pow(rho2,2)*rho2_Q12);
  a2_2_Q22 = gamma*(1.0/rho2*p2_Q22 - p2/pow(rho2,2)*rho2_Q22);
  a2_2_Q32 = gamma*(1.0/rho2*p2_Q32 - p2/pow(rho2,2)*rho2_Q32);
  a2_2_Q42 = gamma*(1.0/rho2*p2_Q42 - p2/pow(rho2,2)*rho2_Q42);
  a2_3     = gamma*p3/rho3;
  a2_3_Q13 = gamma*(1.0/rho3*p3_Q13 - p3/pow(rho3,2)*rho3_Q13);
  a2_3_Q23 = gamma*(1.0/rho3*p3_Q23 - p3/pow(rho3,2)*rho3_Q23);
  a2_3_Q33 = gamma*(1.0/rho3*p3_Q33 - p3/pow(rho3,2)*rho3_Q33);
  a2_3_Q43 = gamma*(1.0/rho3*p3_Q43 - p3/pow(rho3,2)*rho3_Q43);

  a2_1_Q12 = 0.0;  a2_1_Q13 = 0.0;
  a2_1_Q22 = 0.0;  a2_1_Q23 = 0.0;
  a2_1_Q32 = 0.0;  a2_1_Q33 = 0.0;
  a2_1_Q42 = 0.0;  a2_1_Q43 = 0.0;

  a2_2_Q11 = 0.0;  a2_2_Q13 = 0.0;
  a2_2_Q21 = 0.0;  a2_2_Q23 = 0.0;
  a2_2_Q31 = 0.0;  a2_2_Q33 = 0.0;
  a2_2_Q41 = 0.0;  a2_2_Q43 = 0.0;

  a2_3_Q11 = 0.0;  a2_3_Q12 = 0.0;
  a2_3_Q21 = 0.0;  a2_3_Q22 = 0.0;
  a2_3_Q31 = 0.0;  a2_3_Q32 = 0.0;
  a2_3_Q41 = 0.0;  a2_3_Q42 = 0.0;

  //--- Spatial Derivatives --------------------------------------------------80
  A = fabs(area(node[n1][0],node[n1][1],node[n2][0],node[n2][1],
                node[n3][0],node[n3][1]));

  //--- Velocity (at the centroid) -------------------------------------------80
  u = (u1 + u2 + u3)/3.0;
  v = (v1 + v2 + v3)/3.0;
  
  u_Q11 = (u1_Q11 + u2_Q11 + u3_Q11)/3.0;
  u_Q21 = (u1_Q21 + u2_Q21 + u3_Q21)/3.0;
  u_Q31 = (u1_Q31 + u2_Q31 + u3_Q31)/3.0;
  u_Q41 = (u1_Q41 + u2_Q41 + u3_Q41)/3.0;
  u_Q12 = (u1_Q12 + u2_Q12 + u3_Q12)/3.0;
  u_Q22 = (u1_Q22 + u2_Q22 + u3_Q22)/3.0;
  u_Q32 = (u1_Q32 + u2_Q32 + u3_Q32)/3.0;
  u_Q42 = (u1_Q42 + u2_Q42 + u3_Q42)/3.0;
  u_Q13 = (u1_Q13 + u2_Q13 + u3_Q13)/3.0;
  u_Q23 = (u1_Q23 + u2_Q23 + u3_Q23)/3.0;
  u_Q33 = (u1_Q33 + u2_Q33 + u3_Q33)/3.0;
  u_Q43 = (u1_Q43 + u2_Q43 + u3_Q43)/3.0;

  v_Q11 = (v1_Q11 + v2_Q11 + v3_Q11)/3.0;
  v_Q21 = (v1_Q21 + v2_Q21 + v3_Q21)/3.0;
  v_Q31 = (v1_Q31 + v2_Q31 + v3_Q31)/3.0;
  v_Q41 = (v1_Q41 + v2_Q41 + v3_Q41)/3.0;
  v_Q12 = (v1_Q12 + v2_Q12 + v3_Q12)/3.0;
  v_Q22 = (v1_Q22 + v2_Q22 + v3_Q22)/3.0;
  v_Q32 = (v1_Q32 + v2_Q32 + v3_Q32)/3.0;
  v_Q42 = (v1_Q42 + v2_Q42 + v3_Q42)/3.0;
  v_Q13 = (v1_Q13 + v2_Q13 + v3_Q13)/3.0;
  v_Q23 = (v1_Q23 + v2_Q23 + v3_Q23)/3.0;
  v_Q33 = (v1_Q33 + v2_Q33 + v3_Q33)/3.0;
  v_Q43 = (v1_Q43 + v2_Q43 + v3_Q43)/3.0;

  u_x      = (u1*(nx[1]+nx[2]) + u2*(nx[0]+nx[2]) + u3*(nx[1]+nx[0]))/2.0/A;
  u_x_Q11  = (u1_Q11*(nx[1]+nx[2]) + u2_Q11*(nx[0]+nx[2]) + u3_Q11*(nx[1]+nx[0]))/2./A;
  u_x_Q21  = (u1_Q21*(nx[1]+nx[2]) + u2_Q21*(nx[0]+nx[2]) + u3_Q21*(nx[1]+nx[0]))/2./A;
  u_x_Q31  = (u1_Q31*(nx[1]+nx[2]) + u2_Q31*(nx[0]+nx[2]) + u3_Q31*(nx[1]+nx[0]))/2./A;
  u_x_Q41  = (u1_Q41*(nx[1]+nx[2]) + u2_Q41*(nx[0]+nx[2]) + u3_Q41*(nx[1]+nx[0]))/2./A;
  u_x_Q12  = (u1_Q12*(nx[1]+nx[2]) + u2_Q12*(nx[0]+nx[2]) + u3_Q12*(nx[1]+nx[0]))/2./A;
  u_x_Q22  = (u1_Q22*(nx[1]+nx[2]) + u2_Q22*(nx[0]+nx[2]) + u3_Q22*(nx[1]+nx[0]))/2./A;
  u_x_Q32  = (u1_Q32*(nx[1]+nx[2]) + u2_Q32*(nx[0]+nx[2]) + u3_Q32*(nx[1]+nx[0]))/2./A;
  u_x_Q42  = (u1_Q42*(nx[1]+nx[2]) + u2_Q42*(nx[0]+nx[2]) + u3_Q42*(nx[1]+nx[0]))/2./A;
  u_x_Q13  = (u1_Q13*(nx[1]+nx[2]) + u2_Q13*(nx[0]+nx[2]) + u3_Q13*(nx[1]+nx[0]))/2./A;
  u_x_Q23  = (u1_Q23*(nx[1]+nx[2]) + u2_Q23*(nx[0]+nx[2]) + u3_Q23*(nx[1]+nx[0]))/2./A;
  u_x_Q33  = (u1_Q33*(nx[1]+nx[2]) + u2_Q33*(nx[0]+nx[2]) + u3_Q33*(nx[1]+nx[0]))/2./A;
  u_x_Q43  = (u1_Q43*(nx[1]+nx[2]) + u2_Q43*(nx[0]+nx[2]) + u3_Q43*(nx[1]+nx[0]))/2./A;

  u_y      = (u1*(ny[1]+ny[2]) + u2*(ny[0]+ny[2]) + u3*(ny[1]+ny[0]))/2.0/A;
  u_y_Q11  = (u1_Q11*(ny[1]+ny[2]) + u2_Q11*(ny[0]+ny[2]) + u3_Q11*(ny[1]+ny[0]))/2./A;
  u_y_Q21  = (u1_Q21*(ny[1]+ny[2]) + u2_Q21*(ny[0]+ny[2]) + u3_Q21*(ny[1]+ny[0]))/2./A;
  u_y_Q31  = (u1_Q31*(ny[1]+ny[2]) + u2_Q31*(ny[0]+ny[2]) + u3_Q31*(ny[1]+ny[0]))/2./A;
  u_y_Q41  = (u1_Q41*(ny[1]+ny[2]) + u2_Q41*(ny[0]+ny[2]) + u3_Q41*(ny[1]+ny[0]))/2./A;
  u_y_Q12  = (u1_Q12*(ny[1]+ny[2]) + u2_Q12*(ny[0]+ny[2]) + u3_Q12*(ny[1]+ny[0]))/2./A;
  u_y_Q22  = (u1_Q22*(ny[1]+ny[2]) + u2_Q22*(ny[0]+ny[2]) + u3_Q22*(ny[1]+ny[0]))/2./A;
  u_y_Q32  = (u1_Q32*(ny[1]+ny[2]) + u2_Q32*(ny[0]+ny[2]) + u3_Q32*(ny[1]+ny[0]))/2./A;
  u_y_Q42  = (u1_Q42*(ny[1]+ny[2]) + u2_Q42*(ny[0]+ny[2]) + u3_Q42*(ny[1]+ny[0]))/2./A;
  u_y_Q13  = (u1_Q13*(ny[1]+ny[2]) + u2_Q13*(ny[0]+ny[2]) + u3_Q13*(ny[1]+ny[0]))/2./A;
  u_y_Q23  = (u1_Q23*(ny[1]+ny[2]) + u2_Q23*(ny[0]+ny[2]) + u3_Q23*(ny[1]+ny[0]))/2./A;
  u_y_Q33  = (u1_Q33*(ny[1]+ny[2]) + u2_Q33*(ny[0]+ny[2]) + u3_Q33*(ny[1]+ny[0]))/2./A;
  u_y_Q43  = (u1_Q43*(ny[1]+ny[2]) + u2_Q43*(ny[0]+ny[2]) + u3_Q43*(ny[1]+ny[0]))/2./A;

  v_x      = (v1*(nx[1]+nx[2]) + v2*(nx[0]+nx[2]) + v3*(nx[1]+nx[0]))/2.0/A;
  v_x_Q11  = (v1_Q11*(nx[1]+nx[2]) + v2_Q11*(nx[0]+nx[2]) + v3_Q11*(nx[1]+nx[0]))/2./A;
  v_x_Q21  = (v1_Q21*(nx[1]+nx[2]) + v2_Q21*(nx[0]+nx[2]) + v3_Q21*(nx[1]+nx[0]))/2./A;
  v_x_Q31  = (v1_Q31*(nx[1]+nx[2]) + v2_Q31*(nx[0]+nx[2]) + v3_Q31*(nx[1]+nx[0]))/2./A;
  v_x_Q41  = (v1_Q41*(nx[1]+nx[2]) + v2_Q41*(nx[0]+nx[2]) + v3_Q41*(nx[1]+nx[0]))/2./A;
  v_x_Q12  = (v1_Q12*(nx[1]+nx[2]) + v2_Q12*(nx[0]+nx[2]) + v3_Q12*(nx[1]+nx[0]))/2./A;
  v_x_Q22  = (v1_Q22*(nx[1]+nx[2]) + v2_Q22*(nx[0]+nx[2]) + v3_Q22*(nx[1]+nx[0]))/2./A;
  v_x_Q32  = (v1_Q32*(nx[1]+nx[2]) + v2_Q32*(nx[0]+nx[2]) + v3_Q32*(nx[1]+nx[0]))/2./A;
  v_x_Q42  = (v1_Q42*(nx[1]+nx[2]) + v2_Q42*(nx[0]+nx[2]) + v3_Q42*(nx[1]+nx[0]))/2./A;
  v_x_Q13  = (v1_Q13*(nx[1]+nx[2]) + v2_Q13*(nx[0]+nx[2]) + v3_Q13*(nx[1]+nx[0]))/2./A;
  v_x_Q23  = (v1_Q23*(nx[1]+nx[2]) + v2_Q23*(nx[0]+nx[2]) + v3_Q23*(nx[1]+nx[0]))/2./A;
  v_x_Q33  = (v1_Q33*(nx[1]+nx[2]) + v2_Q33*(nx[0]+nx[2]) + v3_Q33*(nx[1]+nx[0]))/2./A;
  v_x_Q43  = (v1_Q43*(nx[1]+nx[2]) + v2_Q43*(nx[0]+nx[2]) + v3_Q43*(nx[1]+nx[0]))/2./A;

  v_y      = (v1*(ny[1]+ny[2]) + v2*(ny[0]+ny[2]) + v3*(ny[1]+ny[0]))/2.0/A;
  v_y_Q11  = (v1_Q11*(ny[1]+ny[2]) + v2_Q11*(ny[0]+ny[2]) + v3_Q11*(ny[1]+ny[0]))/2./A;
  v_y_Q21  = (v1_Q21*(ny[1]+ny[2]) + v2_Q21*(ny[0]+ny[2]) + v3_Q21*(ny[1]+ny[0]))/2./A;
  v_y_Q31  = (v1_Q31*(ny[1]+ny[2]) + v2_Q31*(ny[0]+ny[2]) + v3_Q31*(ny[1]+ny[0]))/2./A;
  v_y_Q41  = (v1_Q41*(ny[1]+ny[2]) + v2_Q41*(ny[0]+ny[2]) + v3_Q41*(ny[1]+ny[0]))/2./A;
  v_y_Q12  = (v1_Q12*(ny[1]+ny[2]) + v2_Q12*(ny[0]+ny[2]) + v3_Q12*(ny[1]+ny[0]))/2./A;
  v_y_Q22  = (v1_Q22*(ny[1]+ny[2]) + v2_Q22*(ny[0]+ny[2]) + v3_Q22*(ny[1]+ny[0]))/2./A;
  v_y_Q32  = (v1_Q32*(ny[1]+ny[2]) + v2_Q32*(ny[0]+ny[2]) + v3_Q32*(ny[1]+ny[0]))/2./A;
  v_y_Q42  = (v1_Q42*(ny[1]+ny[2]) + v2_Q42*(ny[0]+ny[2]) + v3_Q42*(ny[1]+ny[0]))/2./A;
  v_y_Q13  = (v1_Q13*(ny[1]+ny[2]) + v2_Q13*(ny[0]+ny[2]) + v3_Q13*(ny[1]+ny[0]))/2./A;
  v_y_Q23  = (v1_Q23*(ny[1]+ny[2]) + v2_Q23*(ny[0]+ny[2]) + v3_Q23*(ny[1]+ny[0]))/2./A;
  v_y_Q33  = (v1_Q33*(ny[1]+ny[2]) + v2_Q33*(ny[0]+ny[2]) + v3_Q33*(ny[1]+ny[0]))/2./A;
  v_y_Q43  = (v1_Q43*(ny[1]+ny[2]) + v2_Q43*(ny[0]+ny[2]) + v3_Q43*(ny[1]+ny[0]))/2./A;

  //--- Speed of Sound squared Derivatives -----------------------------------80
  a2_x     = (a2_1*(nx[1]+nx[2]) + a2_2*(nx[0]+nx[2]) + a2_3*(nx[1]+nx[0]))/2.0/A;
  a2_y     = (a2_1*(ny[1]+ny[2]) + a2_2*(ny[0]+ny[2]) + a2_3*(ny[1]+ny[0]))/2.0/A;

  a2_x_Q11 = (a2_1_Q11*(nx[1]+nx[2]) + a2_2_Q11*(nx[0]+nx[2]) + a2_3_Q11*(nx[1]+nx[0]))/2.0/A;
  a2_x_Q21 = (a2_1_Q21*(nx[1]+nx[2]) + a2_2_Q21*(nx[0]+nx[2]) + a2_3_Q21*(nx[1]+nx[0]))/2.0/A;
  a2_x_Q31 = (a2_1_Q31*(nx[1]+nx[2]) + a2_2_Q31*(nx[0]+nx[2]) + a2_3_Q31*(nx[1]+nx[0]))/2.0/A;
  a2_x_Q41 = (a2_1_Q41*(nx[1]+nx[2]) + a2_2_Q41*(nx[0]+nx[2]) + a2_3_Q41*(nx[1]+nx[0]))/2.0/A;
  a2_y_Q11 = (a2_1_Q11*(ny[1]+ny[2]) + a2_2_Q11*(ny[0]+ny[2]) + a2_3_Q11*(ny[1]+ny[0]))/2.0/A;
  a2_y_Q21 = (a2_1_Q21*(ny[1]+ny[2]) + a2_2_Q21*(ny[0]+ny[2]) + a2_3_Q21*(ny[1]+ny[0]))/2.0/A;
  a2_y_Q31 = (a2_1_Q31*(ny[1]+ny[2]) + a2_2_Q31*(ny[0]+ny[2]) + a2_3_Q31*(ny[1]+ny[0]))/2.0/A;
  a2_y_Q41 = (a2_1_Q41*(ny[1]+ny[2]) + a2_2_Q41*(ny[0]+ny[2]) + a2_3_Q41*(ny[1]+ny[0]))/2.0/A;

  a2_x_Q12 = (a2_1_Q12*(nx[1]+nx[2]) + a2_2_Q12*(nx[0]+nx[2]) + a2_3_Q12*(nx[1]+nx[0]))/2.0/A;
  a2_x_Q22 = (a2_1_Q22*(nx[1]+nx[2]) + a2_2_Q22*(nx[0]+nx[2]) + a2_3_Q22*(nx[1]+nx[0]))/2.0/A;
  a2_x_Q32 = (a2_1_Q32*(nx[1]+nx[2]) + a2_2_Q32*(nx[0]+nx[2]) + a2_3_Q32*(nx[1]+nx[0]))/2.0/A;
  a2_x_Q42 = (a2_1_Q42*(nx[1]+nx[2]) + a2_2_Q42*(nx[0]+nx[2]) + a2_3_Q42*(nx[1]+nx[0]))/2.0/A;
  a2_y_Q12 = (a2_1_Q12*(ny[1]+ny[2]) + a2_2_Q12*(ny[0]+ny[2]) + a2_3_Q12*(ny[1]+ny[0]))/2.0/A;
  a2_y_Q22 = (a2_1_Q22*(ny[1]+ny[2]) + a2_2_Q22*(ny[0]+ny[2]) + a2_3_Q22*(ny[1]+ny[0]))/2.0/A;
  a2_y_Q32 = (a2_1_Q32*(ny[1]+ny[2]) + a2_2_Q32*(ny[0]+ny[2]) + a2_3_Q32*(ny[1]+ny[0]))/2.0/A;
  a2_y_Q42 = (a2_1_Q42*(ny[1]+ny[2]) + a2_2_Q42*(ny[0]+ny[2]) + a2_3_Q42*(ny[1]+ny[0]))/2.0/A;

  a2_x_Q13 = (a2_1_Q12*(nx[1]+nx[2]) + a2_2_Q13*(nx[0]+nx[2]) + a2_3_Q13*(nx[1]+nx[0]))/2.0/A;
  a2_x_Q23 = (a2_1_Q22*(nx[1]+nx[2]) + a2_2_Q23*(nx[0]+nx[2]) + a2_3_Q23*(nx[1]+nx[0]))/2.0/A;
  a2_x_Q33 = (a2_1_Q32*(nx[1]+nx[2]) + a2_2_Q33*(nx[0]+nx[2]) + a2_3_Q33*(nx[1]+nx[0]))/2.0/A;
  a2_x_Q43 = (a2_1_Q42*(nx[1]+nx[2]) + a2_2_Q43*(nx[0]+nx[2]) + a2_3_Q43*(nx[1]+nx[0]))/2.0/A;
  a2_y_Q13 = (a2_1_Q12*(ny[1]+ny[2]) + a2_2_Q13*(ny[0]+ny[2]) + a2_3_Q13*(ny[1]+ny[0]))/2.0/A;
  a2_y_Q23 = (a2_1_Q22*(ny[1]+ny[2]) + a2_2_Q23*(ny[0]+ny[2]) + a2_3_Q23*(ny[1]+ny[0]))/2.0/A;
  a2_y_Q33 = (a2_1_Q32*(ny[1]+ny[2]) + a2_2_Q33*(ny[0]+ny[2]) + a2_3_Q33*(ny[1]+ny[0]))/2.0/A;
  a2_y_Q43 = (a2_1_Q42*(ny[1]+ny[2]) + a2_2_Q43*(ny[0]+ny[2]) + a2_3_Q43*(ny[1]+ny[0]))/2.0/A;

  //--- Stresses and Stress Derivatives --------------------------------------80
  mu = mu_inf;

  tau_xx     = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x - v_y);
  tau_xx_Q11 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q11 - v_y_Q11);
  tau_xx_Q21 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q21 - v_y_Q21);
  tau_xx_Q31 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q31 - v_y_Q31);
  tau_xx_Q41 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q41 - v_y_Q41);
  tau_xx_Q12 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q12 - v_y_Q12);
  tau_xx_Q22 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q22 - v_y_Q22);
  tau_xx_Q32 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q32 - v_y_Q32);
  tau_xx_Q42 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q42 - v_y_Q42);
  tau_xx_Q13 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q13 - v_y_Q13);
  tau_xx_Q23 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q23 - v_y_Q23);
  tau_xx_Q33 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q33 - v_y_Q33);
  tau_xx_Q43 = mu * M_inf/Re * 2.0/3.0 * (2.0*u_x_Q43 - v_y_Q43);

  tau_yy     = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y - u_x);
  tau_yy_Q11 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q11 - u_x_Q11);
  tau_yy_Q21 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q21 - u_x_Q21);
  tau_yy_Q31 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q31 - u_x_Q31);
  tau_yy_Q41 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q41 - u_x_Q41);
  tau_yy_Q12 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q12 - u_x_Q12);
  tau_yy_Q22 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q22 - u_x_Q22);
  tau_yy_Q32 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q32 - u_x_Q32);
  tau_yy_Q42 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q42 - u_x_Q42);
  tau_yy_Q13 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q13 - u_x_Q13);
  tau_yy_Q23 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q23 - u_x_Q23);
  tau_yy_Q33 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q33 - u_x_Q33);
  tau_yy_Q43 = mu * M_inf/Re * 2.0/3.0 * (2.0*v_y_Q43 - u_x_Q43);
  
  tau_xy     = mu * M_inf/Re * (u_y + v_x);
  tau_xy_Q11 = mu * M_inf/Re * (u_y_Q11 + v_x_Q11);
  tau_xy_Q21 = mu * M_inf/Re * (u_y_Q21 + v_x_Q21);
  tau_xy_Q31 = mu * M_inf/Re * (u_y_Q31 + v_x_Q31);
  tau_xy_Q41 = mu * M_inf/Re * (u_y_Q41 + v_x_Q41);
  tau_xy_Q12 = mu * M_inf/Re * (u_y_Q12 + v_x_Q12);
  tau_xy_Q22 = mu * M_inf/Re * (u_y_Q22 + v_x_Q22);
  tau_xy_Q32 = mu * M_inf/Re * (u_y_Q32 + v_x_Q32);
  tau_xy_Q42 = mu * M_inf/Re * (u_y_Q42 + v_x_Q42);
  tau_xy_Q13 = mu * M_inf/Re * (u_y_Q13 + v_x_Q13);
  tau_xy_Q23 = mu * M_inf/Re * (u_y_Q23 + v_x_Q23);
  tau_xy_Q33 = mu * M_inf/Re * (u_y_Q33 + v_x_Q33);
  tau_xy_Q43 = mu * M_inf/Re * (u_y_Q43 + v_x_Q43);

  //--- Heat Transfer Derivatives --------------------------------------------80
  q_x = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x;
  q_x_Q11 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q11;
  q_x_Q21 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q21;
  q_x_Q31 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q31;
  q_x_Q41 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q41;
  q_x_Q12 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q12;
  q_x_Q22 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q22;
  q_x_Q32 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q32;
  q_x_Q42 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q42;
  q_x_Q13 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q13;
  q_x_Q23 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q23;
  q_x_Q33 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q33;
  q_x_Q43 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_x_Q43;

  q_y = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y;
  q_y_Q11 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q11;
  q_y_Q21 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q21;
  q_y_Q31 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q31;
  q_y_Q41 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q41;
  q_y_Q12 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q12;
  q_y_Q22 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q22;
  q_y_Q32 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q32;
  q_y_Q42 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q42;
  q_y_Q13 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q13;
  q_y_Q23 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q23;
  q_y_Q33 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q33;
  q_y_Q43 = -M_inf/(Re*(gamma-1.0)) * (mu/Pr) * a2_y_Q43;

  //--- Populate Viscous Flux Jacobian Matrix --------------------------------80
 
  nx_hat = nx_face/len; // x component of the unit normal vector for the face
  ny_hat = ny_face/len; // y component of the unit normal vector for the face

  //--- At Node 1 ---
  dfvisc1[0][0] = 0.0;
  dfvisc1[0][1] = 0.0;
  dfvisc1[0][2] = 0.0;
  dfvisc1[0][3] = 0.0;

  dfvisc1[1][0] = len * (tau_xx_Q11*nx_hat + tau_xy_Q11*ny_hat);
  dfvisc1[1][1] = len * (tau_xx_Q21*nx_hat + tau_xy_Q21*ny_hat);
  dfvisc1[1][2] = len * (tau_xx_Q31*nx_hat + tau_xy_Q31*ny_hat);
  dfvisc1[1][3] = len * (tau_xx_Q41*nx_hat + tau_xy_Q41*ny_hat);
 
  dfvisc1[2][0] = len * (tau_xy_Q11*nx_hat + tau_yy_Q11*ny_hat);
  dfvisc1[2][1] = len * (tau_xy_Q21*nx_hat + tau_yy_Q21*ny_hat);
  dfvisc1[2][2] = len * (tau_xy_Q31*nx_hat + tau_yy_Q31*ny_hat);
  dfvisc1[2][3] = len * (tau_xy_Q41*nx_hat + tau_yy_Q41*ny_hat);

  dfvisc1[3][0] = len * ((u*tau_xx_Q11 + u_Q11*tau_xx + v*tau_xy_Q11 + v_Q11*tau_xy
                         - q_x_Q11)*nx_hat +
                         (u*tau_xy_Q11 + u_Q11*tau_xy + v*tau_yy_Q11 + v_Q11*tau_yy
                         - q_y_Q11)*ny_hat);
  dfvisc1[3][1] = len * ((u*tau_xx_Q21 + u_Q21*tau_xx + v*tau_xy_Q21 + v_Q21*tau_xy
                         - q_x_Q21)*nx_hat +
                         (u*tau_xy_Q21 + u_Q21*tau_xy + v*tau_yy_Q21 + v_Q21*tau_yy
                         - q_y_Q21)*ny_hat);
  dfvisc1[3][2] = len * ((u*tau_xx_Q31 + u_Q31*tau_xx + v*tau_xy_Q31 + v_Q31*tau_xy
                         - q_x_Q31)*nx_hat +
                         (u*tau_xy_Q31 + u_Q31*tau_xy + v*tau_yy_Q31 + v_Q31*tau_yy
                         - q_y_Q31)*ny_hat);
  dfvisc1[3][3] = len * ((u*tau_xx_Q41 + u_Q41*tau_xx + v*tau_xy_Q41 + v_Q41*tau_xy
                         - q_x_Q41)*nx_hat +
                         (u*tau_xy_Q41 + u_Q41*tau_xy + v*tau_yy_Q41 + v_Q41*tau_yy
                         - q_y_Q41)*ny_hat);

  //--- At Node 2 ---
  dfvisc2[0][0] = 0.0;
  dfvisc2[0][1] = 0.0;
  dfvisc2[0][2] = 0.0;
  dfvisc2[0][3] = 0.0;

  dfvisc2[1][0] = len * (tau_xx_Q12*nx_hat + tau_xy_Q12*ny_hat);
  dfvisc2[1][1] = len * (tau_xx_Q22*nx_hat + tau_xy_Q22*ny_hat);
  dfvisc2[1][2] = len * (tau_xx_Q32*nx_hat + tau_xy_Q32*ny_hat);
  dfvisc2[1][3] = len * (tau_xx_Q42*nx_hat + tau_xy_Q42*ny_hat);
 
  dfvisc2[2][0] = len * (tau_xy_Q12*nx_hat + tau_yy_Q12*ny_hat);
  dfvisc2[2][1] = len * (tau_xy_Q22*nx_hat + tau_yy_Q22*ny_hat);
  dfvisc2[2][2] = len * (tau_xy_Q32*nx_hat + tau_yy_Q32*ny_hat);
  dfvisc2[2][3] = len * (tau_xy_Q42*nx_hat + tau_yy_Q42*ny_hat);

  dfvisc2[3][0] = len * ((u*tau_xx_Q12 + u_Q12*tau_xx + v*tau_xy_Q12 + v_Q12*tau_xy
                         - q_x_Q12)*nx_hat +
                         (u*tau_xy_Q12 + u_Q12*tau_xy + v*tau_yy_Q12 + v_Q12*tau_yy
                         - q_y_Q12)*ny_hat);
  dfvisc2[3][1] = len * ((u*tau_xx_Q22 + u_Q22*tau_xx + v*tau_xy_Q22 + v_Q22*tau_xy
                         - q_x_Q22)*nx_hat +
                         (u*tau_xy_Q22 + u_Q22*tau_xy + v*tau_yy_Q22 + v_Q22*tau_yy
                         - q_y_Q22)*ny_hat);
  dfvisc2[3][2] = len * ((u*tau_xx_Q32 + u_Q32*tau_xx + v*tau_xy_Q32 + v_Q32*tau_xy
                         - q_x_Q32)*nx_hat +
                         (u*tau_xy_Q32 + u_Q32*tau_xy + v*tau_yy_Q32 + v_Q32*tau_yy
                         - q_y_Q32)*ny_hat);
  dfvisc2[3][3] = len * ((u*tau_xx_Q42 + u_Q42*tau_xx + v*tau_xy_Q42 + v_Q42*tau_xy
                         - q_x_Q42)*nx_hat +
                         (u*tau_xy_Q42 + u_Q42*tau_xy + v*tau_yy_Q42 + v_Q42*tau_yy
                         - q_y_Q42)*ny_hat);

  //--- At Node 3 ---
  dfvisc3[0][0] = 0.0;
  dfvisc3[0][1] = 0.0;
  dfvisc3[0][2] = 0.0;
  dfvisc3[0][3] = 0.0;

  dfvisc3[1][0] = len * (tau_xx_Q13*nx_hat + tau_xy_Q13*ny_hat);
  dfvisc3[1][1] = len * (tau_xx_Q23*nx_hat + tau_xy_Q23*ny_hat);
  dfvisc3[1][2] = len * (tau_xx_Q33*nx_hat + tau_xy_Q33*ny_hat);
  dfvisc3[1][3] = len * (tau_xx_Q43*nx_hat + tau_xy_Q43*ny_hat);
 
  dfvisc3[2][0] = len * (tau_xy_Q13*nx_hat + tau_yy_Q13*ny_hat);
  dfvisc3[2][1] = len * (tau_xy_Q23*nx_hat + tau_yy_Q23*ny_hat);
  dfvisc3[2][2] = len * (tau_xy_Q33*nx_hat + tau_yy_Q33*ny_hat);
  dfvisc3[2][3] = len * (tau_xy_Q43*nx_hat + tau_yy_Q43*ny_hat);

  dfvisc3[3][0] = len * ((u*tau_xx_Q13 + u_Q13*tau_xx + v*tau_xy_Q13 + v_Q13*tau_xy
                         - q_x_Q13)*nx_hat +
                         (u*tau_xy_Q13 + u_Q13*tau_xy + v*tau_yy_Q13 + v_Q13*tau_yy
                         - q_y_Q13)*ny_hat);
  dfvisc3[3][1] = len * ((u*tau_xx_Q23 + u_Q23*tau_xx + v*tau_xy_Q23 + v_Q23*tau_xy
                         - q_x_Q23)*nx_hat +
                         (u*tau_xy_Q23 + u_Q23*tau_xy + v*tau_yy_Q23 + v_Q23*tau_yy
                         - q_y_Q23)*ny_hat);
  dfvisc3[3][2] = len * ((u*tau_xx_Q33 + u_Q33*tau_xx + v*tau_xy_Q33 + v_Q33*tau_xy
                         - q_x_Q33)*nx_hat +
                         (u*tau_xy_Q33 + u_Q33*tau_xy + v*tau_yy_Q33 + v_Q33*tau_yy
                         - q_y_Q33)*ny_hat);
  dfvisc3[3][3] = len * ((u*tau_xx_Q43 + u_Q43*tau_xx + v*tau_xy_Q43 + v_Q43*tau_xy
                         - q_x_Q43)*nx_hat +
                         (u*tau_xy_Q43 + u_Q43*tau_xy + v*tau_yy_Q43 + v_Q43*tau_yy
                         - q_y_Q43)*ny_hat);
}
