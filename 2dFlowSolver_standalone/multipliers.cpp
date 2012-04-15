//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 20Nov
// Discription:
//  Calculate Lagrange Multipliers for the adjoint Equaition
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

#include "lu.h"

vector< vector<double> > multipliers(vector< vector< vector<double> > > R_Q_T,
                                     vector< vector<double> >           I_Q,
                                     vector<int>                        iau,
                                     vector<int>                        ia,
                                     vector<int>                        ja)
{
  printf("\n Calculating Lagrange Multipliers");
  int i,j,k,n;         // counters
  int nn;              // number of nodes
  int lin_it;          // iteration number of the linear system (inner loop)
  int flag;            // flag for controlling output from lu_solve 
  int maxInIt;         // maximum number of itterations
  double RMSnode;      // The 2-norm of the Residual for a row
  double RMStot;       // The total Residual of the Linear system
  double kickout;      // The convergence criterion

  vector<double> b(4);    // right hand side for solving a given row (node)
  vector<double> offD(4); // off diagonal contribution
  vector<double> Dx(4);   // The diagonal contribution; used to calculate residual
  vector<double> RMS(4);  // The Residual vector RMS = b - [offD]x - [D]x
 
  // lambda is a vector of Lagrange Multipliers, this is what is returned
  vector< vector<double> > lambda(iau.size(),vector<double>(4)); 
    
  //--- Solve Linear System Using Point Implicit Scheme ----------------------80
  //-- Solve Ax=b for a given row (row n) using LU factoriazation ------------80
  //A, 4x4 = diagonal entry of the row: A[ [iau[n] ]
  //b, 1x4 = RHS minus all of the off diagonal entries: b-offD
  //x, 1x4 = delQ[n]

  nn = iau.size();
  lin_it = 1;
  maxInIt = 500;
  kickout = 1.0e-15;
  do
  {
    RMStot = 0.0;
    for(n=0; n<nn; n++)
    {
      RMSnode = 0.0;
      for(i=0; i<4; i++)
      {
        b[i] = -I_Q[n][i];
      }

      for(j=ia[n]; j<ia[n+1]; j++)
      {
        if( j != iau[n] )
        {
          for(i=0; i<4; i++)
          {
            offD[i] = R_Q_T[j][i][0]*lambda[ja[j]][0] +
                      R_Q_T[j][i][1]*lambda[ja[j]][1] +
                      R_Q_T[j][i][2]*lambda[ja[j]][2] +
                      R_Q_T[j][i][3]*lambda[ja[j]][3];
          }

          for(i=0; i<4; i++)
          {
            b[i] -= offD[i];
          }
        }
      }
        
      for(i=0; i<4; i++)
      {
        Dx[i] = R_Q_T[iau[n]][i][0]*lambda[n][0] +
                R_Q_T[iau[n]][i][1]*lambda[n][1] +
                R_Q_T[iau[n]][i][2]*lambda[n][2] +
                R_Q_T[iau[n]][i][3]*lambda[n][3];
      }

      //--- Calculate Residual of Linear System ---
      for(i=0; i<4; i++)
      {
        RMS[i] = b[i] - Dx[i];   
        RMSnode += pow(RMS[i],2);
      }
      RMSnode = sqrt(RMSnode/4.0);

      //--- Solve Row Using LU decomposition ---
      for(i=0; i<4; i++)
      {
        for(j=0; j<4; j++)
        {
          if(isnan(R_Q_T[ iau[n] ][i][j]) == 1)
          {
            printf("\nERROR: Problem with A matrix in adjoint\n"); exit(0);
          }
        }
      }
      flag = 0; // 0:minimize output, 1:print additional output from lu_solve
      lambda[n] = lu_solve(flag, R_Q_T[iau[n]], b);

      RMStot += pow(RMSnode,2);
    }//End of node loop
       
    RMStot = sqrt(RMStot/nn);
    lin_it++;
  }while(lin_it<=maxInIt && RMStot>kickout);
  //--- End of Inner Loop --------------------------------------------------80

  return lambda;
}
