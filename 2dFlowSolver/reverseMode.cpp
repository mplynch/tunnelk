//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 20Nov
// Discription:
//  Calculates the design derivatives using reverse mode differentiation
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

double reverseMode(double                   I_X,
                   vector< vector<double> > RHS_B,
                   vector< vector<double> > lambda)
{
  //printf("\n Calculating Design Derivatives using Reverse Mode");
  int i,j,k,n; // counters
  int nn;      // number of nodes
  int nDV;     // number of design variables      

  nn = RHS_B.size();

  double I_B; // Design derivatives dI/dB, This is what is returned

  I_B = I_X;

  for(n=0; n<nn; n++)
    for(i=0; i<4; i++)
      I_B += lambda[n][i]*RHS_B[n][i];
  //printf("\n Reverse Mode I_B = %17.10e",I_B);

  return I_B;
}
