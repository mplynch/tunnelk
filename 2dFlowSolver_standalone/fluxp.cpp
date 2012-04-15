#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

// Subroutine flux preturns f_plus
vector<double> fluxp(vector<double> Q,double nx, double ny, double len)
{
  double rho, u, v, e, p, gamma, c, Ubar;
  double nx_hat, ny_hat;
  vector<double> fplus;
 
  //initiealize fplus vector
  for(int i=0;i<4;i++)
    fplus.push_back(0.0);

  rho = Q[0];    //density
  u = Q[1]/Q[0]; //velicity in the x directin
  v = Q[2]/Q[0]; //velicity in the y directin
  e = Q[3];      //energy
  
  gamma = 1.4;                               //ratio of specific heats
  p = (Q[3]-0.5*Q[0]*(u*u + v*v))*(gamma-1); //pressure
  c = sqrt(gamma*p/rho);                     //speed of sound

  nx_hat = nx/len; //x component of the unit normal vector
  ny_hat = ny/len; //y component of the unit normal vector
  Ubar = nx_hat*u + ny_hat*v;

  if(Ubar > c)
  {  
    fplus[0] = len * rho * Ubar;  
    fplus[1] = len * (rho*u*Ubar + nx_hat*p);
    fplus[2] = len * (rho*v*Ubar + ny_hat*p);
    fplus[3] = len * ((e+p)*Ubar);
  }
  else if(Ubar < -c)
  {
    fplus[0] =  0.0;
    fplus[1] =  0.0;
    fplus[2] =  0.0;
    fplus[3] =  0.0;
  }
  else 
  {
    fplus[0] = len*(0.25*rho*c*pow(Ubar/c+1.0,2));
    fplus[1] = fplus[0] * (nx_hat/gamma*(-Ubar+2.0*c)+u);
    fplus[2] = fplus[0] * (ny_hat/gamma*(-Ubar+2.0*c)+v);
    fplus[3] = fplus[0] * ( (-(gamma-1.0)*pow(Ubar,2) + 2.0*(gamma-1.0)*Ubar*c + 2*pow(c,2))
                             /(pow(gamma,2) - 1.0)
                            + (pow(u,2)+pow(v,2))/2.0 );
  }
  
  return fplus;
}
