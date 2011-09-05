#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
 
using namespace std;

// Subroutine gradx returns gradient in the x direction
vector< vector<double> > gradx(vector< vector<double> > q,
                               vector< vector<int> > tri,
                               vector< vector<int> > n2t,
                               vector< vector<double> > node)
{
  int i, j, n;       // counters
  int nn;            // number of nodes
  int n1, n2, n3;    // nodes on triangle vertices
  int gtri;          // global triangle number

  vector<double> nx; // normal in the x direction
  vector<double> v0; // 1D vector of zeros
  vector<double> q_x_tri; // cell gradient for a triangle
  vector< vector<double> > q_x;     //the node x gradient

  nn = node.size();

  for(j=0; j<4; j++)
    v0.push_back(0.0);

  for(n=0; n<nn; n++)
  {
    q_x.push_back(v0);
    for (i=0; i<n2t[n].size(); i++)
    {
      gtri = n2t[n][i];
      if( tri[gtri][0] == 0) // i.e. if triangle is not exterior
      {
        n1 = tri[gtri][1];
        n2 = tri[gtri][2];
        n3 = tri[gtri][3];
 
        nx.clear();

        nx.push_back(  node[n3][1] - node[n2][1] );
        nx.push_back(  node[n1][1] - node[n3][1] );
        nx.push_back(  node[n2][1] - node[n1][1] );

        for(j=0; j<4; j++)
        {
          q_x_tri.push_back( 1.0/2.0 * (q[n1][j]*(nx[1]+nx[2]) 
                                      + q[n2][j]*(nx[0]+nx[2]) 
                                      + q[n3][j]*(nx[1]+nx[0]) ));
        }
        for(j=0; j<4; j++)
          q_x[n][j] += q_x_tri[j];
         
        q_x_tri.clear();
      }
    }
      for(j=0; j<4; j++)
        q_x[n][j] /= node[n][2]*3.0;
  }

  return q_x;
}
