//----------------------------------------------------------------------------80
// Author: James S Masters
// Date:   16Sep2011
// Discription:
//  Code to run the design optimization portion of the code
//  This is code that was from Kyle Andersons Design class. It was originally in
//  the drive.cpp code. most of it has been commented out. It will have to be
//  heavily debugged if I want to use it again
//----------------------------------------------------------------------------80

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

#include "checkpoint.h"

void opt01(double alpha)
{
  //--- Variables ------------------------------------------------------------80
  const double pi = 3.141592653589793;

  FILE *fp;

  int  bdim = 420;
  char filename[32];
  char buff[bdim];

  int i,j,k,n,t;   // Counters
  int nn;          // number of nodes
  int adjoint = 1;
  int nDV = 3;     // number of design variables
  int nodeNum;     // the node number
 
  double kickout;
  double baselineCost;  // The cost before the mesh is moved 

  vector< double >                     I_X(nDV); // dI/dB, Q FIXED
  vector< double >                     I_B(nDV); // dI/dB, Q FIXED

  vector< vector< double > >           I_Q;      // dI/dQ 

  nn = 1; //scratch value
  vector< vector<double> > node_B(nn,vector<double>(2));  // derivatives of the mesh coordinates
  
  //--- get kickout value ----------------------------------------------------80
  sprintf(filename, "kickout.txt");
  if((fp=fopen(filename,"r")) == NULL)
  {
    printf("\nCould not open file: <%s>\n",filename); exit(0);
  }
  fgets(buff, bdim, fp);
  kickout = atof(buff);
  fclose(fp);

#if 0 //----------------------------------------------------------------------80PRINT

  if(kickout < 1.0e-17)
  {
    // --- Calculate dI/dQ for the adjoint equation --------------------------80
    printf("\n Calculating Design Derivatives using Reverse Mode");
    if(adjoint)
    {
      alpha = alpha*pi/180.0;
  
      I_Q = dCostdQ(alpha, node_list, node, Q, Qfree);
      for(i=0; i<nDV; i++)
      {
        nodeNum = 42+i;
        sprintf(filename, "naca%02d.cmesh",nodeNum);
        node.clear();
        node_ImPrt.clear();
        tri.clear();
        quad.clear();
        bound.clear();
        readCmplxmesh(filename, node, node_ImPrt, tri, quad, bound);
  
        edge.clear();
        info.clear();
        convert(node, tri, bound, edge, info);
        ref_it=0;
        Qfree.clear();
        node_list.clear();
        RHS_B.clear();
  
        lambdaFlag = 0;
        Q = solve(ref_it, lift, drag, node, node_ImPrt, edge, info, int_input, dub_input,
                  Qfree,node_list,lambda00,lambdaFlag,RHS_B);
   
        for(n=0; n<node_ImPrt.size(); n++)
          for(j=0; j<2; j++)
            node_B[n][j] = node_ImPrt[n][j] / 1.0e-7; // HARD CODED DELTA BETA
  
        I_X[i] = dCostdX(alpha, node_list, node, node_B, Q, Qfree);
        I_B[i] = reverseMode(I_X[i], RHS_B, lambda);
      }
  
      sprintf(filename, "cost_b.txt");
      printf("\n Printing dirivative of cost to File: <%s>\n",filename);
      if ((fp = fopen(filename,"w+")) == 0)
      {
        printf("\nError opening file <%s>\n",filename);
        return(1);
      }
      rewind(fp);
      printf("\n");
      for(i=0; i<nDV; i++)
      {
        nodeNum = 42+i;
        printf("\nReverse Mode Derivative for node %d = %17.10e",nodeNum,I_B[i]);
        fprintf(fp,"%21.12e\n",I_B[i]);
      }
      fclose(fp);
  
      printf("\n");
    }
  }
#endif //---------------------------------------------------------------------80PRINT

}
