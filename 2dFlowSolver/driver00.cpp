//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 09Sept09
// Discription:
//  Code for optimizing an airfoil
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/time.h>

using namespace std;

#include "read.h"
#include "plot.h"
#include "convert.h"
#include "flowSolve.h"
#include "dCostdQ.h"
#include "dCostdX.h"
#include "transpose.h"
#include "multipliers.h"
#include "reverseMode.h"

double walltime()
{
  static struct timeval tp;
  static struct timezone tzp;
  return 0==gettimeofday(&tp,&tzp) ?
         (double)(tp.tv_sec)+(double)(tp.tv_usec)*1.0e-6 : 0.0;
}

int main(int argcs, char* pArgs[])
{ 
  printf("\n ------ Starting solve.exe ------ \n");
  //--- Get Start Time -------------------------------------------------------80
  double start = walltime();

  //--- Variables ------------------------------------------------------------80
  const double pi = 3.14159265;

  FILE *fp;
  char filename[32];
  char gnufilename[32];
  char* meshfile;
  char newmeshfile[32];
  char restartfile[32];
  int  bdim = 420;
  char buff[bdim];

  time_t tm;
  clock_t time_0, time_1;

  int i, j, k, n, t;    // Counters
  int nn, nt, nb;       // Number of nodes, triangles and boundaries
  int nbe;              // Number of boundary edges
  int ref_it = 0;       // Refinement iteration number
  int dnum;             // number of design variables
  int smoothmax;        // maximum number of outer itterations for Winslow Smoother
  int restart;          // Restart Flag:      1-use restart file, 0-start from Freestream

  double x,y;           // coordinates
  double drag;          // drag on the airfoil (calculated in pplot)
  double lift;          // drag on the airfoil (calculated in pplot)
  double baselineCost;  // The cost before the mesh is moved 

  vector<int>              v0;   //generic 1D int vector
  vector< vector<int> >    v00;  //generic 2D int vector
  vector<double>           dv0;  //generic 1D double vector
  vector< vector<double> > dv00; //generic 2D double vector

  vector<int>    info;         // info for the solver nn, ne, tri (including external), nbe
  vector<double> xvec;         // x coordinates of the nodes
  vector<double> yvec;         // y coordinates of the nodes
  vector<int>    int_input;    // integer input
  vector<double> dub_input;    // integer input
  vector<double> deltacost(2); // dI in the dI/dB sensitivity derivative

  vector< vector<int> >    tri;        // tri[nt][3]
  vector< vector<int> >    quad;       // quads (not currently used)
  vector< vector<int> >    edge;       // edge[ne][4] - node1, node2, tri-to-left, tri-to-right
  vector< vector<double> > node;       // real part of node[nn][2]
  vector< vector<double> > node_ImPrt; // imaginary part of node[nn][2]
  vector< vector<double> > Q;          // state vector Q[nn][4]
  vector< vector<double> > smoothnode; // coordinates after smoothing

  vector< vector< vector<int> > > bound; // boundeary information
  vector< vector<double> >        newxy; // New x and y coordinates of all the nodes

  //--- Check for correct number of arguments --------------------------------80
  if (argcs != 2)
  {
    printf("\nUsage:");
    printf("\nrefine.exe  2D_Mesh_Filer \n\n");
    exit(0);
  }

  fflush(stdout);

  //--- Read in information from design.in -----------------------------------80
  readinput(int_input, dub_input);

  if(int_input[0] == 1)
    printf("\n Case is first order accurate");
  else
    printf("\n Case is second order accurate");

  if(int_input[1] == 0)
    printf("\n Case is inviscid");
  else
    printf("\n Case is viscous");

  restart = int_input[9];
  if(restart == 0)
    printf("\n Starting from Freestream Conditions\n");
  else
    printf("\n Starting from Restart File\n");

  //--- Read in 2D Mesh File in .mesh or .cmesh Format ---------------------80
  meshfile = pArgs[1];
  if(meshfile[ strlen(meshfile)-5  ] == 'c')
  {
    printf("\n Reading in COMPLEX mesh \n");
    readCmplxmesh(meshfile, node, node_ImPrt, tri, quad, bound);
  }
  else
  {
    printf("\n Reading in NON-COMPLEX mesh \n");
    readmesh(meshfile, node, tri, quad, bound);

    dv0.push_back(0.0);
    dv0.push_back(0.0);
    for(n=0;n<node.size();n++)
      node_ImPrt.push_back(dv0);
  }

  nn = node.size();
  nt = tri.size();
  nb = bound.size();

  convert(node, tri, bound, edge, info);
  
  //--- Create Gnuplot File of mesh ---
  printf("\n Generting Gnuplot File to verify convertion");
  sprintf(gnufilename, "mesh_eptr.gnu");
  gplot(gnufilename, node, tri);

  //--- Perform CFD on new  mesh -------------------------------------------80
  double alpha;                        // angle of attack in degrees
  int lambdaFlag = 1;                  // Flag that specifies whether or not to calculate lambda
  vector<double> Qfree;                // Freestream state vector
  vector<int> node_list;               // list of boundary condition for each node
  vector< vector< double > > lambda;   // Lagrange Multipliers
  vector< vector< double > > lambda00; // dummy Lagrange Multipliers
  vector< vector<double> > RHS_B;      // derivtive of the Residual

  vector< vector<double> > node_B(nn,vector<double>(2));  // derivatives of the mesh coordinates

  Q = solve(ref_it, lift, drag, node, node_ImPrt, edge, info, int_input, dub_input,
            Qfree,node_list,lambda,lambdaFlag,RHS_B);


  //--- get kickout value ----------------------------------------------------80
  double kickout;
  sprintf(filename, "kickout.txt");
  if((fp=fopen(filename,"r")) == NULL)
  {
    printf("\nCould not open file: <%s>\n",filename); exit(0);
  }

  fgets(buff, bdim, fp);
  kickout = atof(buff);
  fclose(fp);

  int adjoint =  0;
  if(kickout < 1.0e-17) adjoint = 1;

  // --- Calculate dI/dQ for the adjoint equation ----------------------------80
  if(adjoint)
  {
    printf("\n Calculating Design Derivatives using Reverse Mode");

    int nDV = 3;  // number of design variables
    int nodeNum;  // the node number

    alpha  = dub_input[1];
    alpha = alpha*pi/180.0;

    vector< vector< double > >           I_Q;      // dI/dQ 
    vector< double >                     I_X(nDV); // dI/dB, Q FIXED
    vector< double >                     I_B(nDV); // dI/dB, Q FIXED

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

      lambdaFlag = 1;
      Q = solve(ref_it, lift, drag, node, node_ImPrt, edge, info, int_input, dub_input,
                Qfree,node_list,lambda00,lambdaFlag,RHS_B);
 
      for(n=0; n<node_ImPrt.size(); n++)
        for(j=0; j<2; j++)
          node_B[n][j] = node_ImPrt[n][j] / 1.0e-7; // HARD CODED DELTA BETA

      I_X[i] = dCostdX(alpha, node_list, node, node_B, Q, Qfree);
      I_B[i] = reverseMode(I_X[i], RHS_B, lambda);
    }

    printf("\n");
    for(i=0; i<nDV; i++)
    {
      nodeNum = 42+i;
      printf("\nReverse Mode Derivative for node %d = %17.10e",nodeNum,I_B[i]);
    }

    printf("\n");
  }

  fflush(stdout);
  //--- Get End Time ---------------------------------------------------------80
  double stop = walltime();

  //printf("\n Total time in seconds = %12.5e\n",stop-start);
  printf("\n");

  return 0;
}
