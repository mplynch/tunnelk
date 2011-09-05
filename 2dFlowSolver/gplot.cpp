//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 3Feb09
// Discription:
//   Convert mesh to gnulot format
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

void gplot(const char* filename, 
           vector< vector<double> > node, 
           vector<vector<int> >     tri)
{
  FILE *fp;
  int i;
  int nt = tri.size();

  printf("\n GNUPLOT Filename = <%s>\n",filename);

  if ((fp = fopen(filename,"w")) == 0)
  {
    printf("\nError opening file <%s>.",filename);
    exit(0);
  }
  for (i=0; i < nt; i++)
  {
    int n0 = tri[i][0];
    int n1 = tri[i][1];
    int n2 = tri[i][2];
    if(isnan(node[n0][0]) == 1 || isnan(node[n0][1]) == 1 ||
       isnan(node[n1][0]) == 1 || isnan(node[n1][1]) == 1 ||
       isnan(node[n2][0]) == 1 || isnan(node[n2][1]) == 1)
    {
      printf("\nNAN generated during gnuplot conversion\n");
      exit(1);
    }
    fprintf(fp,"%19.10e %19.10e 0.0\n",  node[n0][0],node[n0][1]);
    fprintf(fp,"%19.10e %19.10e 0.0\n",  node[n1][0],node[n1][1]);
    fprintf(fp,"%19.10e %19.10e 0.0\n",  node[n2][0],node[n2][1]);
    fprintf(fp,"%19.10e %19.10e 0.0\n\n",node[n0][0],node[n0][1]);
  }
  fclose(fp);
}
