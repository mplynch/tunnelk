//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 09Sept09
// Discription:
//  Code for testing flux derivatives
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

#include "flux.h"

int main(int argcs, char* pArgs[])
{ 
  printf(" --- Starting fluxdrive.exe --- \n");

  //--- Variables ------------------------------------------------------------80
  vector<double> Q_R(4);
  vector<double> fminus_original(4);
  vector<double> fminus_perturbed(4);
  vector<double> fplus_original(4);
  vector<double> fplus_perturbed(4);
  vector<double> fminus_B(4); // dfluxm / dBeta using direct differentiation
  vector<double> fplus_B(4);  // dfluxp / dBeta using direct differentiation
 
  int i;
  double nx0,ny0,len0;
  double nx,ny,len;
  double delta_B = 1.0e-7;
  double nx_B;
  double ny_B;
  double len_B;

  //--- Q values are held fixed ----------------------------------------------80
  Q_R[0] = 7.8937570432e-01;
  Q_R[1] = 7.7939985833e-01;
  Q_R[2] = 3.2199828798e-03;
  Q_R[3] = 1.7040071856e+00;

  //--- Original Mesh values
  nx0  = -0.0079543878;
  ny0  = -0.0090621349;
  len0 =  0.0120579673;
  fminus_original = fluxm(Q_R, nx0, ny0, len0);
  fplus_original  = fluxp(Q_R, nx0, ny0, len0);

  //--- Perturbed Mesh values
  nx  = -0.0079543878;
  ny  = -0.0090621373;
  len =  0.0120579691;
  fminus_perturbed = fluxm(Q_R, nx, ny, len);
  fplus_perturbed  = fluxp(Q_R, nx, ny, len);

  for(i=0; i<4; i++)
    printf("\n  fminus_original[i] = %19.10e",fminus_original[i]);
  printf("\n");

  //--- derivative ---
  for(i=0; i<4; i++)
  {
    fminus_B[i] = (fminus_perturbed[i] - fminus_original[i]) / delta_B;
    fplus_B[i]  = (fplus_perturbed[i]  - fplus_original[i]) / delta_B;
  }

  //--- Compare Resuluts for minus flux --------------------------------------80
  //--- finite difference ---
  printf("\n Derivative of fluxm using finite difference");
  for(i=0; i<4; i++)
    printf("\n  fluxm_B[i] = %19.10e",fminus_B[i]);
  printf("\n");

  //--- Forward mode differentiation ---
  printf("\n Derivative using forward mode diferentiation");
  //nx_B = (nx - nx0)/delta_B;
  //ny_B = (ny - ny0)/delta_B;
  //len_B = (len - len0)/delta_B;
  nx_B=-1.994808e-04;
  ny_B=-2.446576e-02;
  len_B=1.919320e-02;

  fminus_B = fluxm_B(Q_R, nx0, ny0, len0, nx_B, ny_B, len_B);
  for(i=0; i<4; i++)
    printf("\n  fluxm_B[i] = %19.10e",fminus_B[i]);
  printf("\n");

  //--- Compare Resuluts for plus flux ---------------------------------------80
  //--- finite difference ---
  printf("\n Derivative of fluxp using finite difference");
  for(i=0; i<4; i++)
    printf("\n  fluxp_B[i] = %19.10e",fplus_B[i]);
  printf("\n");

  //--- Forward mode differentiation ---
  printf("\n Derivative using forward mode diferentiation");
  fplus_B = fluxp_B(Q_R, nx0, ny0, len0, nx_B, ny_B, len_B);
  for(i=0; i<4; i++)
    printf("\n  fluxp_B[i] = %19.10e",fplus_B[i]);
  printf("\n");
}






