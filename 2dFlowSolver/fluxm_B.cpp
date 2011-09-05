#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
 
using namespace std;

// Subroutine fluxm_B returns the derivative of f_minus w.r.t. Beta (a design variable)
// This is the partial derivative [dflux/dB with Q fixed (i.e. there is no Q_B)
vector<double> fluxm_B(vector<double> Q,  
                       double nx,   double ny,   double len,
                       double nx_B, double ny_B, double len_B)
{
  //--- Variables ------------------------------------------------------------80
  int i,j;               // counters
  double gamma = 1.4;    // ratio of specific heats
  double rho;            // density
  double u, v;           // velocities in the x and y directions
  double p;              // pressure
  double e;              // internal energy
  double c;              // speed of sound
  double spd2;           // speed squared

  double nx_hat;         // x component of the unit normal vector
  double ny_hat;         // y component of the unit normal vector
  double Ubar;           // velocity normal to the face 

  //--- Derivatives of variables w.r.t Beta ---------------------------------80
  double rho_B  = 0.0;
  double u_B    = 0.0; 
  double v_B    = 0.0;
  double p_B    = 0.0;
  double e_B    = 0.0;
  double c_B    = 0.0;
  double spd2_B = 0.0;

  double nx_hat_B, ny_hat_B;
  double Ubar_B;

  vector<double> fminus_B(4);
 
  //--- Calculate Values ----------------------------------------------------80
  //flow Variables
  rho  = Q[0];                      
  u    = Q[1]/Q[0];                
  v    = Q[2]/Q[0];               
  e    = Q[3];                   
  spd2 = u*u + v*v;
  p    = (gamma-1.0)*(e-0.5*rho*spd2);
  c    = sqrt(gamma*p/rho);          
  
  nx_hat = nx/len;
  ny_hat = ny/len;
  Ubar = nx_hat*u + ny_hat*v;

  //--- Calculate Derivatives -----------------------------------------------80
  nx_hat_B = nx_B/len - nx*len_B/(len*len);
  ny_hat_B = ny_B/len - ny*len_B/(len*len);
  Ubar_B   = (nx_hat_B*u + nx_hat*u_B) + (ny_hat_B*v + ny_hat*v_B);


#if 0
// recaculate flux to double check values
  vector<double> fminus(4);
  if(Ubar > c)
  {  
    fminus[0] =  0.0;
    fminus[1] =  0.0;
    fminus[2] =  0.0;
    fminus[3] =  0.0;
  }
  else if(Ubar < -c)
  {
    fminus[0] = len * rho * Ubar;  
    fminus[1] = len * (rho*u*Ubar + nx_hat*p);
    fminus[2] = len * (rho*v*Ubar + ny_hat*p);
    fminus[3] = len * ((e+p)*Ubar);
  }
  else
  {  
    fminus[0] = len*(-0.25*rho*c*pow(Ubar/c-1.0,2));
    fminus[1] = fminus[0] * (nx_hat/gamma*(-Ubar-2.0*c)+u);
    fminus[2] = fminus[0] * (ny_hat/gamma*(-Ubar-2.0*c)+v);
    fminus[3] = fminus[0] * ( (-(gamma-1.0)*pow(Ubar,2) - 2.0*(gamma-1.0)*Ubar*c + 2*pow(c,2))
                             /(pow(gamma,2) - 1.0)
                            + (pow(u,2)+pow(v,2))/2.0 );
  }
  for(i=0; i<4; i++)
    printf("\n  fminus[i] = %19.10e",fminus[i]);
  printf("\n");
#endif

  if(Ubar > c)
  {  
    fminus_B[0] =  0.0;
    fminus_B[1] =  0.0;
    fminus_B[2] =  0.0;
    fminus_B[3] =  0.0;
  }
  else if(Ubar < -c)
  {
    fminus_B[0] = len_B*rho*Ubar + len*rho_B*Ubar + len*rho*Ubar_B;  
    fminus_B[1] = len_B*(rho*u*Ubar + nx_hat*p) + len*(rho*u*Ubar_B + nx_hat_B*p);
    fminus_B[2] = len_B*(rho*v*Ubar + ny_hat*p) + len*(rho*v*Ubar_B + ny_hat_B*p);
    fminus_B[3] = len_B*((e+p)*Ubar)            + len*((e+p)*Ubar_B);
  }
  else
  {  
    double fminus0 = len *   -rho*c/4.0 * pow(Ubar/c-1.0,2);
    //printf("\n  fminus0 = %19.10e\n",fminus0);

    fminus_B[0] = len_B * -rho*c/4.0 * pow(Ubar/c-1.0,2)
                + len   * -rho*c/4.0 * 2.0*(Ubar/c - 1.0)*(Ubar_B/c);

    fminus_B[1] = fminus_B[0] * (nx_hat/gamma  *(-Ubar-2.0*c) + u )
                + fminus0     * (nx_hat_B/gamma*(-Ubar-2.0*c) + 
                                nx_hat/gamma  *(-Ubar_B-2.0*c_B));
    fminus_B[2] = fminus_B[0] * (ny_hat/gamma  *(-Ubar-2.0*c) + v)
                + fminus0     * (ny_hat_B/gamma*(-Ubar-2.0*c) + 
                                ny_hat/gamma  *(-Ubar_B-2.0*c_B));
    fminus_B[3] = fminus_B[0] * (((1.0-gamma)*Ubar*Ubar  - 2.0*(gamma-1.0)*Ubar*c + 2*c*c)
                                /(gamma*gamma-1.0) + spd2/2.0 )
                + fminus0     * (((1.0-gamma)*2.0*Ubar*Ubar_B - 2.0*(gamma-1.0)*Ubar_B*c)
                                /(gamma*gamma-1.0));
  }

  return fminus_B;
}
