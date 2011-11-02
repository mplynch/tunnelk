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
#include "designFlowSolve.h"
#include "dCostdQ.h"
#include "dCostdX.h"
#include "transpose.h"
#include "multipliers.h"
#include "reverseMode.h"
#include "optimization.h"

double walltime()
{
  static struct timeval tp;
  static struct timezone tzp;
  return 0==gettimeofday(&tp,&tzp) ?
         (double)(tp.tv_sec)+(double)(tp.tv_usec)*1.0e-6 : 0.0;
}

int run()
{ 
  printf("\n ------ Starting solve.exe ------ \n");
  //--- Get Start Time -------------------------------------------------------80
  double start = walltime();

  //--- Variables ------------------------------------------------------------80
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
  int restart;          // Restart Flag: 1-use restart file, 0-start from Freestream
  int lambdaFlag = 1;   // Flag that specifies whether or not to calculate lambda
  int design;           // Design flag: if 0, don't calculate design variables

  double x,y;           // coordinates
  double drag;          // drag on the airfoil (calculated in pplot)
  double lift;          // drag on the airfoil (calculated in pplot)

  vector<int> v0;        // generic 1D int vector
  vector<int> node_list; // list of boundary condition for each node
  vector<int> info;      // info for the solver nn, ne, tri (including external), nbe
  vector<int> int_input; // integer input

  vector<double> dv0;   // generic 1D double vector
  vector<double> Qfree; // Freestream state vector

  vector<double> dub_input; // integer input
  vector<double> xvec;      // x coordinates of the nodes
  vector<double> yvec;      // y coordinates of the nodes

  vector< vector<int> >    v00;  // generic 2D int vector
  vector< vector<int> >    tri;  // tri[nt][3]
  vector< vector<int> >    quad; // quads (not currently used)
  vector< vector<int> >    edge; // edge[ne][4] - node1, node2, tri-to-left, tri-to-right

  vector< vector<double> > dv00;       // generic 2D double vector
  vector< vector<double> > node;       // real part of node[nn][2]
  vector< vector<double> > node_ImPrt; // imaginary part of node[nn][2]
  vector< vector<double> > Q;          // state vector Q[nn][4]
  vector< vector<double> > smoothnode; // coordinates after smoothing
  vector< vector<double> > lambda;     // Lagrange Multipliers
  vector< vector<double> > lambda00;   // dummy Lagrange Multipliers
  vector< vector<double> > RHS_B;      // derivtive of the Residual
  vector< vector<double> > newxy;      // New x and y coordinates of all the nodes

  vector< vector< vector<int> > > bound; // boundeary information

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

  design = int_input[10];
  if(design == 0)
    printf("\n Code will not calculate design variables\n");
  else
    printf("\n Code will calculate design variables\n");

  //--- Read in 2D Mesh File in .mesh or .cmesh Format ---------------------80
  //meshfile = "naca0012.mesh";
  extern const char* mesh_filename;
  meshfile = (char*)mesh_filename;
  if(false && meshfile[ strlen(meshfile)-5  ] == 'c')
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
// <tunnelk modification>
#if 0
  gplot(gnufilename, node, tri);
#endif
// </tunnelk modification>

  //--- Perform CFD and Design Optimization ----------------------------------80
  if(design)
  {
    printf("\nERROR: Current version is not compiled to handle desing optimization\n\n");
    exit(0);
    //Q = designSolve(ref_it, lift, drag, node, node_ImPrt, edge, info, int_input, dub_input,
    //                Qfree,node_list,lambda,lambdaFlag,RHS_B);
    //opt01(dub_input[1]);
  }
  //--- Perform CFD only -----------------------------------------------------80
  else
  {
    Q = solve(ref_it, lift, drag, node, node_ImPrt, edge, info, int_input, dub_input,
              Qfree,node_list);
  }

  fflush(stdout);
  //--- Get End Time ---------------------------------------------------------80
  double stop = walltime();

  printf("\n Total time in seconds = %12.5e\n",stop-start);
  printf("\n");

  extern std::string message;
  message = "ran through driver.cpp, w/o actually\n"
            "calling solve()";

  return 0;
}
