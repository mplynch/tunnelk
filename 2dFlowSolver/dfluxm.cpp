#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;
 
// Subroutine dfluxm returns Flux Jacobian for f_minus
vector< vector<double> > dfluxm(vector<double> Q,double nx, double ny, double len)
{
  int i;
  double gamma = 1.4;
  double fluxm1 ,fluxm2 ,fluxm3, fluxm4;
  double rho, u, v, e, p, c, Ubar;
  double nx_hat, ny_hat;                     //Components of the unit normal vector 
  double rho_Q1, rho_Q2, rho_Q3, rho_Q4;     //rho derivatives
  double u_Q1, u_Q2, u_Q3, u_Q4;             //u derivatives
  double v_Q1, v_Q2, v_Q3, v_Q4;             //v derivatives
  double e_Q1, e_Q2, e_Q3, e_Q4;             //e derivatives
  double p_Q1, p_Q2, p_Q3, p_Q4;             //pressure derivatives
  double Ubar_Q1, Ubar_Q2, Ubar_Q3, Ubar_Q4; //Ubar derivatives
  double spd2;                               //speed squared
  double spd2_Q1, spd2_Q2, spd2_Q3, spd2_Q4; //speed^2 derivatives
  double c_Q1, c_Q2, c_Q3, c_Q4;             //speed of sound derivatives
  
  vector<double> vec;
  vector <vector<double> > dfminus;

  //--- Inialize Flux Jacobian Vector ---
  for(i=0;i<4;i++)
    vec.push_back(0.0); 
  for(i=0;i<4;i++)
    dfminus.push_back(vec); 

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

  //--- speed of sound ---
  c = sqrt(gamma*p/rho);
  c_Q1 = gamma/2.0 / (sqrt(gamma*p/rho)) * (1.0/rho*p_Q1-p/pow(rho,2)*rho_Q1);
  c_Q2 = gamma/2.0 / (sqrt(gamma*p/rho)) * (1.0/rho*p_Q2-p/pow(rho,2)*rho_Q2);
  c_Q3 = gamma/2.0 / (sqrt(gamma*p/rho)) * (1.0/rho*p_Q3-p/pow(rho,2)*rho_Q3);
  c_Q4 = gamma/2.0 / (sqrt(gamma*p/rho)) * (1.0/rho*p_Q4-p/pow(rho,2)*rho_Q4);

  //--- components of the unit normal vector ---
  nx_hat = nx/len;
  ny_hat = ny/len;

  //--- Ubar ---
  Ubar = nx_hat*u + ny_hat*v;
  Ubar_Q1 = nx_hat*u_Q1 + ny_hat*v_Q1;
  Ubar_Q2 = nx_hat*u_Q2 + ny_hat*v_Q2;
  Ubar_Q3 = nx_hat*u_Q3 + ny_hat*v_Q3;
  Ubar_Q4 = nx_hat*u_Q4 + ny_hat*v_Q4;
 
  //--- fluxes --- 
  fluxm1 = -len*(0.25*rho*c*pow(Ubar/c-1.0,2));
  fluxm2 = nx_hat/gamma*(-Ubar-2.0*c)+u;
  fluxm3 = ny_hat/gamma*(-Ubar-2.0*c)+v;
  fluxm4 = (-(gamma-1.0)*pow(Ubar,2) - 2.0*(gamma-1.0)*Ubar*c + 2.0*pow(c,2))/(pow(gamma,2) - 1.0)
            + (pow(u,2)+pow(v,2))/2.0;

  //--- flux jacobians ---
  if(Ubar < -c)
  {  
    dfminus[0][0] = len * (rho*Ubar_Q1 + Ubar*rho_Q1);  
    dfminus[0][1] = len * (rho*Ubar_Q2 + Ubar*rho_Q2);  
    dfminus[0][2] = len * (rho*Ubar_Q3 + Ubar*rho_Q3);  
    dfminus[0][3] = len * (rho*Ubar_Q4 + Ubar*rho_Q4);  

    dfminus[1][0] = len * (rho_Q1*(u*Ubar)+u_Q1*(rho*Ubar)+Ubar_Q1*(rho*u) + nx_hat*p_Q1);
    dfminus[1][1] = len * (rho_Q2*(u*Ubar)+u_Q2*(rho*Ubar)+Ubar_Q2*(rho*u) + nx_hat*p_Q2);
    dfminus[1][2] = len * (rho_Q3*(u*Ubar)+u_Q3*(rho*Ubar)+Ubar_Q3*(rho*u) + nx_hat*p_Q3);
    dfminus[1][3] = len * (rho_Q4*(u*Ubar)+u_Q4*(rho*Ubar)+Ubar_Q4*(rho*u) + nx_hat*p_Q4);

    dfminus[2][0] = len * (rho_Q1*(v*Ubar)+v_Q1*(rho*Ubar)+Ubar_Q1*(rho*v) + ny_hat*p_Q1);
    dfminus[2][1] = len * (rho_Q2*(v*Ubar)+v_Q2*(rho*Ubar)+Ubar_Q2*(rho*v) + ny_hat*p_Q2);
    dfminus[2][2] = len * (rho_Q3*(v*Ubar)+v_Q3*(rho*Ubar)+Ubar_Q3*(rho*v) + ny_hat*p_Q3);
    dfminus[2][3] = len * (rho_Q4*(v*Ubar)+v_Q4*(rho*Ubar)+Ubar_Q4*(rho*v) + ny_hat*p_Q4);

    dfminus[3][0] = len * (Ubar*e_Q1+e*Ubar_Q1 + p*Ubar_Q1+Ubar*p_Q1);
    dfminus[3][1] = len * (Ubar*e_Q2+e*Ubar_Q2 + p*Ubar_Q2+Ubar*p_Q2);
    dfminus[3][2] = len * (Ubar*e_Q3+e*Ubar_Q3 + p*Ubar_Q3+Ubar*p_Q3);
    dfminus[3][3] = len * (Ubar*e_Q4+e*Ubar_Q4 + p*Ubar_Q4+Ubar*p_Q4);
  }
  else if(Ubar > c)
  {
    dfminus[0][0] = 0.0; dfminus[0][1] = 0.0; dfminus[0][2] = 0.0; dfminus[0][3] = 0.0;
    dfminus[1][0] = 0.0; dfminus[1][1] = 0.0; dfminus[1][2] = 0.0; dfminus[1][3] = 0.0;
    dfminus[2][0] = 0.0; dfminus[2][1] = 0.0; dfminus[2][2] = 0.0; dfminus[2][3] = 0.0;
    dfminus[3][0] = 0.0; dfminus[3][1] = 0.0; dfminus[3][2] = 0.0; dfminus[3][3] = 0.0;
  }
  else 
  {
    dfminus[0][0] = -len/4.0*(rho*(2.0*c*(Ubar/c-1.0)*(1.0/c*Ubar_Q1-Ubar/pow(c,2)*c_Q1)+pow(Ubar/c-1.0,2)*c_Q1)
                    +rho_Q1*c*pow(Ubar/c-1.0,2));
    dfminus[0][1] = -len/4.0*(rho*(2.0*c*(Ubar/c-1.0)*(1.0/c*Ubar_Q2-Ubar/pow(c,2)*c_Q2)+pow(Ubar/c-1.0,2)*c_Q2)
                    +rho_Q2*c*pow(Ubar/c-1.0,2));
    dfminus[0][2] = -len/4.0*(rho*(2.0*c*(Ubar/c-1.0)*(1.0/c*Ubar_Q3-Ubar/pow(c,2)*c_Q3)+pow(Ubar/c-1.0,2)*c_Q3)
                    +rho_Q3*c*pow(Ubar/c-1.0,2));
    dfminus[0][3] = -len/4.0*(rho*(2.0*c*(Ubar/c-1.0)*(1.0/c*Ubar_Q4-Ubar/pow(c,2)*c_Q4)+pow(Ubar/c-1.0,2)*c_Q4)
                    +rho_Q4*c*pow(Ubar/c-1.0,2));
  
    dfminus[1][0] = dfminus[0][0]*fluxm2 + fluxm1*(nx_hat/gamma*(-Ubar_Q1-2.0*c_Q1)+u_Q1);
    dfminus[1][1] = dfminus[0][1]*fluxm2 + fluxm1*(nx_hat/gamma*(-Ubar_Q2-2.0*c_Q2)+u_Q2);
    dfminus[1][2] = dfminus[0][2]*fluxm2 + fluxm1*(nx_hat/gamma*(-Ubar_Q3-2.0*c_Q3)+u_Q3);
    dfminus[1][3] = dfminus[0][3]*fluxm2 + fluxm1*(nx_hat/gamma*(-Ubar_Q4-2.0*c_Q4)+u_Q4);
   
    dfminus[2][0] = dfminus[0][0]*fluxm3 + fluxm1*(ny_hat/gamma*(-Ubar_Q1-2.0*c_Q1)+v_Q1);
    dfminus[2][1] = dfminus[0][1]*fluxm3 + fluxm1*(ny_hat/gamma*(-Ubar_Q2-2.0*c_Q2)+v_Q2);
    dfminus[2][2] = dfminus[0][2]*fluxm3 + fluxm1*(ny_hat/gamma*(-Ubar_Q3-2.0*c_Q3)+v_Q3);
    dfminus[2][3] = dfminus[0][3]*fluxm3 + fluxm1*(ny_hat/gamma*(-Ubar_Q4-2.0*c_Q4)+v_Q4);
   
    dfminus[3][0] = dfminus[0][0]*fluxm4 + fluxm1 * ( 1.0/(pow(gamma,2)-1.0) *
                    (-(gamma-1.0)*2.0*Ubar*Ubar_Q1 - 2.0*(gamma-1.0)*(Ubar*c_Q1+c*Ubar_Q1) + 4.0*c*c_Q1)
                    + spd2_Q1/2.0);
    dfminus[3][1] = dfminus[0][1]*fluxm4 + fluxm1 * ( 1.0/(pow(gamma,2)-1.0) *
                    (-(gamma-1.0)*2.0*Ubar*Ubar_Q2 - 2.0*(gamma-1.0)*(Ubar*c_Q2+c*Ubar_Q2) + 4.0*c*c_Q2)
                    + spd2_Q2/2.0);
    dfminus[3][2] = dfminus[0][2]*fluxm4 + fluxm1 * ( 1.0/(pow(gamma,2)-1.0) *
                    (-(gamma-1.0)*2.0*Ubar*Ubar_Q3 - 2.0*(gamma-1.0)*(Ubar*c_Q3+c*Ubar_Q3) + 4.0*c*c_Q3)
                    + spd2_Q3/2.0);
    dfminus[3][3] = dfminus[0][3]*fluxm4 + fluxm1 * ( 1.0/(pow(gamma,2)-1.0) *
                    (-(gamma-1.0)*2.0*Ubar*Ubar_Q4 - 2.0*(gamma-1.0)*(Ubar*c_Q4+c*Ubar_Q4) + 4.0*c*c_Q4)
                    + spd2_Q4/2.0);
  }

  return dfminus;
}
