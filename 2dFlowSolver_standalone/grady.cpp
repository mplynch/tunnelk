#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
 
using namespace std;

// Subroutine grady returns gradient in the y direction
vector< vector<double> > grady(vector< vector<double> > q,
                               vector< vector<int> > tri,
                               vector< vector<int> > n2t,
                               vector< vector<double> > node)
{
  int i, j, n;       // counters
  int nn;            // number of nodes
  int n1, n2, n3;    // nodes on triangle vertices
  int gtri;          // global triangle number

  vector<double> ny; // normal in the x direction
  vector<double> v0; // 1D vector of zeros
  vector<double> q_y_tri; // cell gradient for a triangle
  vector< vector<double> > q_y;     //the node x gradient
  
  nn = node.size();

/*//*** Test Block ***
for(n=0; n<nn; n++){
q[n][0] = 1.0*node[n][0] + 2.0*node[n][1];
q[n][1] = 3.0*node[n][0] + 4.0*node[n][1];
q[n][2] = 5.0*node[n][0] + 6.0*node[n][1];
q[n][3] = 7.0*node[n][0] + 8.0*node[n][1];
}
//******************/

  for(j=0; j<4; j++)
    v0.push_back(0.0);

  for(n=0; n<nn; n++)
  {
    q_y.push_back(v0);
    for (i=0; i<n2t[n].size(); i++)
    {
      gtri = n2t[n][i];
      if( tri[gtri][0] == 0) // i.e. if triangle is not exterior
      {
        n1 = tri[gtri][1];
        n2 = tri[gtri][2];
        n3 = tri[gtri][3];
 
        ny.clear();

        ny.push_back( -(node[n3][0] - node[n2][0]) );
        ny.push_back( -(node[n1][0] - node[n3][0]) );
        ny.push_back( -(node[n2][0] - node[n1][0]) );

        for(j=0; j<4; j++)
        {
          q_y_tri.push_back( 1.0/2.0 * (q[n1][j]*(ny[1]+ny[2]) 
                                      + q[n2][j]*(ny[0]+ny[2]) 
                                      + q[n3][j]*(ny[1]+ny[0]) ));
        }
        for(j=0; j<4; j++)
          q_y[n][j] += q_y_tri[j];
         
        q_y_tri.clear();
      }
    }
      for(j=0; j<4; j++)
        q_y[n][j] /= node[n][2]*3.0;
  }

  return q_y;
}
