//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 18Oct09
// Discription:
//   Print Residual
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

void printResid(int flag,
                vector< vector<double> > R) 
{
  char filename[32];
  FILE *fp;
  int i,n,nn;
  double eps = 1.0e-7;
  vector< vector<double> > R_B;  // the forward Mode differential of the residual

  //--- Finite difference (R was read in before any convergence) -------------80
  // for this case node_B[214][1] == 0 and it is assumed that the real part of the 
  // mesh has been perturbed 
  // We want to find (R_pertubed - R_original)/delta_B 
  // We assume R_original to be zero and delta_B to be 1.0e-7
  if(flag == 0)
  {
    sprintf(filename, "finiteDiff.txt");
    if ((fp = fopen(filename,"w")) == 0)
    {
      printf("\nError opening file <%s>.",filename);
      exit(0);
    }
    printf("\n Residual Filename = <%s>\n",filename);

    nn = R.size();

    fprintf(fp,"----- Residuals -----\n");
    fprintf(fp,"Node #      R1                  R2");
    fprintf(fp,"                  R3                  R4\n");
    for (n=0; n<nn; n++)
    {
      fprintf(fp,"%5d %19.10e %19.10e %19.10e %19.10e\n",n,R[n][0],R[n][1],R[n][2],R[n][3]);
    }
  
    fprintf(fp,"\n\n----- Finite Difference of residual -----\n");
    fprintf(fp,"Node #      dR/dB               dR/dB");
    fprintf(fp,"                  dR/dB               dR/dB\n");
    for (n=0; n<nn; n++)
    {
      fprintf(fp,"%5d %19.10e %19.10e %19.10e %19.10e\n"
              ,n,R[n][0]/eps,R[n][1]/eps,R[n][2]/eps,R[n][3]/eps);
    }
    fclose(fp);

    printf(" Finite difference calculated as (R_perturbed - R_orig)/1.0e-7\n");
    printf(" n=214   R[214][0]=%13.6e  R[214][1]=%13.6e  R[214][2]=%13.6e  R[214][3]=%13.6e\n"
          ,R[214][0]/eps,R[214][1]/eps,R[214][2]/eps,R[214][3]/eps);
  }

  //--- Forward Mode difference (R_B was read in before any convergence) -----80
  else if(flag == 1)
  {
    sprintf(filename, "forwardMode.txt");
    if ((fp = fopen(filename,"w")) == 0)
    {
      printf("\nError opening file <%s>.",filename);
      exit(0);
    }
    printf("\n Residual Filename = <%s>\n",filename);
  
    R_B = R; // R_B was actualy read in but it was called R initially
    nn = R_B.size();
  
    fprintf(fp,"----- Derivative of Residuals w.r.t design variable -----\n");
    fprintf(fp,"Node #      R1_B                 R2_B");
    fprintf(fp,"                 R3_B                 R4_B\n");
    for (n=0; n<nn; n++)
    {
      fprintf(fp,"%5d %19.10e %19.10e %19.10e %19.10e\n"
                ,n,R_B[n][0],R_B[n][1],R_B[n][2],R_B[n][3]);
    }
  
    fclose(fp);
  
    printf("--- dR/dB from Finite difference ---\n");
    printf(" n=214   R[214][0]=-3.534459e-03   R[214][1]=-6.355319e-03   R[214][2]=-1.202232e-03   R[214][3]=-1.125296e-02\n");

    printf("\n--- dR/dB from Forward Mode differentiation ---\n");
    printf(" n=214 R_B[214][0]=%13.6e R_B[214][0]=%13.6e R_B[214][0]=%13.6e R_B[214][0]=%13.6e\n"
            ,R_B[214][0],R_B[214][1],R_B[214][2],R_B[214][3]);
  }


  //--- Examine dQ/dB --------------------------------------------------------80
  else if(flag == 3)
  {
    vector<double> Q_orig(4);
    vector<double> Q_perturbed(4);
    vector<double> Q_B(4);
    double deltaB = 1.0e-7;

    //--- For Node 214 ----
    Q_orig[0] =  7.8937570431380e-01;
    Q_orig[1] =  7.7939985832568e-01;
    Q_orig[2] =  3.2199828796402e-03;
    Q_orig[3] =  1.7040071856014e+00;

    Q_perturbed[0] =  7.8937628543247e-01;
    Q_perturbed[1] =  7.7940006751108e-01;
    Q_perturbed[2] =  3.2204805087137e-03;
    Q_perturbed[3] =  1.7040084139793e+00;

    for(i=0; i<4; i++)
      Q_B[i] = (Q_perturbed[i] - Q_orig[i]) / deltaB;

    printf("--- Q_B using finite difference ---");
    for(i=0; i<4; i++)
      printf("\n Q_B[214][%d] = %20.13e",i,Q_B[i]);
    printf("\n");
  }

  else 
    printf("\nERROR: invalid residual file flag\n");

}
