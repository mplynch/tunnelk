//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 14Oct09
// Discription:
//  calclulates dQ/dB
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

int main(int argcs, char* pArgs[])
{ 
  printf(" --- Calculating dQ/dB --- \n");

  FILE *fp;
  char filename[32];
  int i,j,k,n;  // counters
  int nn = 575;
  double dB = 1.0e-5; // the change in the design variable

  vector< vector<double> > Q0(nn,vector<double>(4)); //nn x 4 array - Original Mesh
  vector< vector<double> > Q(nn,vector<double>(4));  //nn x 4 array - Perturbed Mesh
  vector< vector<double> > Q_B(nn,vector<double>(4)); //difference between values
 
  //--- Read in binary Q file of original mesh------------------------------80
  if((fp=fopen("restart-0.dat","rb")) == NULL)
  {
    printf("\nCould not open file: <restart.dat>\n"); exit(0);
  }
  rewind(fp);
  for(n=0; n<nn; n++)
    for(i=0; i<4; i++)
      fread(&Q0[n][i], sizeof(double), 1, fp);
  fclose(fp);
 
  //--- Read in binary Q file of perturbed mesh-----------------------------80
  if((fp=fopen("restart-1.dat","rb")) == NULL)
  {
    printf("\nCould not open file: <restart.dat>\n"); exit(0);
  }
  rewind(fp);
  for(n=0; n<nn; n++)
    for(i=0; i<4; i++)
      fread(&Q[n][i], sizeof(double), 1, fp);
  fclose(fp);

  //--- Calcuce dQ/dB ------------------------------------------------------80
  for (n=0; n<nn; n++)
    for (i=0; i<4; i++)
      Q_B[n][i] = (Q[n][i] - Q0[n][i]) / dB;

  //--- Write out Q values -------------------------------------------------80
  sprintf(filename, "qVals.txt");
  printf("\noutput Filename = <%s>\n",filename);
  if ((fp = fopen(filename,"w")) == 0)
  {
    printf("\nError opening file <%s>.",filename);
    exit(0);
  }

  fprintf(fp," --- dQ/dB  ---\n");
  fprintf(fp,"Node           dQ/dB[0]            dQ/dB[1]");
  fprintf(fp,"            dQ/dB[2]            dQ/dB[3]\n");
  for (n=0; n<nn; n++)
  {
    fprintf(fp,"%4d %19.10e %19.10e %19.10e %19.10e\n",n,Q_B[n][0],Q_B[n][1],Q_B[n][2],Q_B[n][3]);
  }
  fprintf(fp,"\n");

  fprintf(fp," --- Original Mesh ---\n");
  fprintf(fp,"   n               Q[0]                Q[1]");
  fprintf(fp,"                Q[2]                Q[3]\n");
  for (n=0; n<nn; n++)
  {
    fprintf(fp,"%4d %19.10e %19.10e %19.10e %19.10e\n",n,Q[n][0],Q[n][1],Q[n][2],Q[n][3]);
  }
  fprintf(fp,"\n");

  printf("\n");
}
