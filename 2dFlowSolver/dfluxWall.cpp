#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;
 
// Subroutine dfluxWall returns Flux Jacobian for edges on an inviscid surface
vector< vector<double> > dfluxWall(vector<double> Q, double nx, double ny, double len)
{
  int i;
  double gamma = 1.4;
  double rho, u, v, e, p;
  double nx_hat, ny_hat;                     //Components of the unit normal vector 
  double rho_Q1, rho_Q2, rho_Q3, rho_Q4;     //rho derivatives
  double u_Q1, u_Q2, u_Q3, u_Q4;             //u derivatives
  double v_Q1, v_Q2, v_Q3, v_Q4;             //v derivatives
  double e_Q1, e_Q2, e_Q3, e_Q4;             //e derivatives
  double p_Q1, p_Q2, p_Q3, p_Q4;             //pressure derivatives
  double spd2;                               //speed squared
  double spd2_Q1, spd2_Q2, spd2_Q3, spd2_Q4; //speed^2 derivatives
  
  vector<double> vec;
  vector <vector<double> > dfwall;

  //--- Inialize Flux Jacobian Vector ---
  for(i=0;i<4;i++)
    vec.push_back(0.0); 
  for(i=0;i<4;i++)
    dfwall.push_back(vec); 

  //--- density ---
  rho = Q[0];
  rho_Q1 = 1.0;
  rho_Q2 = 0.0;
  rho_Q3 = 0.0;
  rho_Q4 = 0.0;

  //--- velicity in the x direction ---
  u = Q[1]/Q[0];
  u_Q1 = -Q[1]/pow(Q[0],2);
  u_Q2 = 1.0/Q[0];
  u_Q3 = 0.0;
  u_Q4 = 0.0;

  //--- velicity in the y direction ---
  v = Q[2]/Q[0];
  v_Q1 = -Q[2]/pow(Q[0],2);
  v_Q2 = 0.0;
  v_Q3 = 1.0/Q[0];
  v_Q4 = 0.0;

  //--- energy ---
  e = Q[3]; 
  e_Q1 = 0.0;
  e_Q2 = 0.0;
  e_Q3 = 0.0;
  e_Q4 = 1.0;

  //--- speed squared ---
  spd2 = (pow(Q[1],2) + pow(Q[2],2))/pow(Q[0],2);
  spd2_Q1 = -2.0*(pow(Q[1],2) + pow(Q[2],2))/pow(Q[0],3); 
  spd2_Q2 = 2.0*Q[1]/pow(Q[0],2); 
  spd2_Q3 = 2.0*Q[2]/pow(Q[0],2); 
  spd2_Q4 = 0.0;

  //--- pressure ---
  p = (gamma-1.0)*(e-0.5*rho*spd2);
  p_Q1 = (gamma-1.0)*(e_Q1-0.5*rho*spd2_Q1-0.5*spd2*rho_Q1);
  p_Q2 = (gamma-1.0)*(e_Q2-0.5*rho*spd2_Q2-0.5*spd2*rho_Q2);
  p_Q3 = (gamma-1.0)*(e_Q3-0.5*rho*spd2_Q3-0.5*spd2*rho_Q3);
  p_Q4 = (gamma-1.0)*(e_Q4-0.5*rho*spd2_Q4-0.5*spd2*rho_Q4);

  //--- components of the unit normal vector ---
  nx_hat = nx/len;
  ny_hat = ny/len;
 
  //--- flux jacobians ---
  dfwall[0][0] = 0.0;
  dfwall[0][1] = 0.0;
  dfwall[0][2] = 0.0;
  dfwall[0][3] = 0.0;

  dfwall[1][0] = len * nx_hat*p_Q1;
  dfwall[1][1] = len * nx_hat*p_Q2;
  dfwall[1][2] = len * nx_hat*p_Q3;
  dfwall[1][3] = len * nx_hat*p_Q4;

  dfwall[2][0] = len * ny_hat*p_Q1;
  dfwall[2][1] = len * ny_hat*p_Q2;
  dfwall[2][2] = len * ny_hat*p_Q3;
  dfwall[2][3] = len * ny_hat*p_Q4;

  dfwall[3][0] = 0.0;
  dfwall[3][1] = 0.0;
  dfwall[3][2] = 0.0;
  dfwall[3][3] = 0.0;

  return dfwall;
}
