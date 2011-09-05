//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 28Feb09
// Discription:
//   Convert mesh and flow variables to tecplot format
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

void tplot(const char* filename, vector<double> x, vector<double> y, 
           vector<vector<double> > Q,
           vector<vector<int> > tri)
{
  FILE *fp;
  int i,n;
  int nn, nt;
  double gamma = 1.4;
  double spd2;  // speed squared
  vector<double> mach; //mach number
  vector<double> p;    //pressure

  nn = x.size();
  nt = tri.size();

  for (n=0; n<nn; n++)
  {
    mach.push_back(sqrt( pow(Q[n][1]/Q[n][0],2) + pow(Q[n][2]/Q[n][0],2) ));
    spd2 = (pow(Q[n][1],2) + pow(Q[n][2],2))/pow(Q[n][0],2);
    p.push_back( (gamma-1.0)*(Q[n][3]-0.5*Q[n][0]*spd2) );
  }

  printf("\nTecPlot Filename = <%s>\n",filename);

  if ((fp = fopen(filename,"w")) == 0)
  {
    printf("\nError opening file <%s>.",filename);
    exit(0);
  }

  fprintf(fp,"TITLE = \"NACA0012 Airfoil Data\"\n");
  fprintf(fp,"VARIABLES = \"x\" \"y\" \"density\" \"Mach #\" \"Pressure\"\n");
  fprintf(fp,"ZONE T = \"square\" N=%d, E=%d, F=FEPOINT, ET=TRIANGLE\n",nn,nt);
  for (i=0; i<nn; i++)
  {
    fprintf(fp,"%19.10e %19.10e %19.10e %19.10e %19.10e\n",x[i],y[i],Q[i][0],mach[i],p[i]);
  }
 fprintf(fp,"\n");

  for (i=0; i<nt; i++)
  {
    tri[i][0]++; //tecplot wants cell index to start at 1, not 0 
    tri[i][1]++;
    tri[i][2]++;

    int n1 = tri[i][0];
    int n2 = tri[i][1];
    int n3 = tri[i][2];
    fprintf(fp,"%5d %5d %5d\n",n1,n2,n3);
  }
  fclose(fp);
}
