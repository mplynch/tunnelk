//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 16Feb09
// Discription:
//  Code to solve a flow solution on an unstructured grid in .eptr format
//----------------------------------------------------------------------------80

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

#include "read.h"
#include "area.h"
#include "flux.h"
#include "lu.h"
#include "plot.h"
#include "grad.h"
#include "cost.h"
#include "printResid.h"
#include "dCostdQ.h"
#include "dCostdX.h"
#include "transpose.h"
#include "multipliers.h"
#include "reverseMode.h"
#include "checkpoint.h"

vector< vector<double> > solve(int                                ref_it,
                               double                             &lift,
                               double                             &drag,
                               vector< vector<double> >           &node,
                               vector< vector<double> >           &node_ImPrt,
                               vector< vector<int> >              &edge,
                               vector<int>                        &info,
                               vector<int>                        &int_input,
                               vector<double>                     &dub_input,
                               vector<double>                     &Qfree,
                               vector<int>                        &node_list)
{
  printf("\n--- Generating CFD Flow Solution ---\n");
  FILE *fp;
  int bdim = 100;
  char filename[32];
  char buff[bdim];

  const double pi = 3.141592653589793;

  int i, j, k, n, t;         // counter variables
  int nn;                    // number of nodes
  int ne;                    // number of edges
  int nt;                    // number of triangles
  int nbe;                   // number of boundary edges
  int iau_dim,ia_dim,ja_dim; // dimensions of ia, iau and ja
  int n1, n2, n3;            // edge nodes (n3 is the off-edge vertex node)
  int design;                // Design flag: if 0, don't calculate desing variables
  int accuracy;              // order or accuracy for the flow solver
  int viscous;               // 1 for viscous, 0 for inviscid
  int maxInIt;               // maximum number of inner itterations
  int maxOutIt;              // maximum number of outer itterations
  int restart;               // restart flag: 0=freestrema, 1=restart
  int it;                    // iteration number of the total system (outer loop)
  int lin_it;                // iteration number of the linear system (inner loop)
  int flag;                  // flag for controlling output from lu_solve 
  int ntest;                 // a test node
  int node1, node2, node3;   // nodes
  
  double mach;               // Mach Number
  double alpha;              // angle of attack in degrees
  double gamma;              // ratio of specific heats
  double rho;                // viscosity
  double mu;                 // density
  double CFLmin;             // minimum CFL number
  double CFLmax;             // maximum CFL number
  double iramp;              // number of iterations to ramp CFL from min to max
  double kickout;            // The convergence criterion
  double kickwayout;         // The convergence criterion to exit the outer loop
  double Twall = 1.0;        // Wall temperature - used for NO SLIP boundary condition
  double ramp;               // ramping coefficient
  double CFL;                // CFL number 
  double denominator;        // bottom of deta t equation
  double c;                  // speed of sound at node n
  double spd2;               // speed squared
  double x_mid;              // x-component of edge midpoint
  double y_mid;              // y-component of edge midpoint
  double x_face;             // x-component of dual face midpoint
  double y_face;             // y-component of dual face midpoint
  double nx,ny;              // non-normalized normal vector components
  double nx_hat,ny_hat;      // normalized normal vector components
  double len;                // face length
  double RMSnode;            // The 2-norm of the Residual for a row
  double delQnode;           // The 2-norm of the Residual for a row
  double RMStot;             // The total Residual of the Linear system (inner loop)
  double RMSout;             // The total Residual of the Flow Field (outer loop)
  double RMSdelQ;            // The RMS of the change in Q
  double p_left;             // Pressure on the face to the left of the midpoint
  double p_right;            // Pressure on the face to the right of the midpoint
  double v2_left;            // Velocity squared on the face to the left of the midpoint
  double v2_right;           // Velocity squared on the face to the right of the midpoint
  double u, v;               // x and y components of velocity
  double p;                  // pressure 
  double e;                  // energy
  double xmid, ymid;         // midpoint of the triangle
  double tri_area;           // triangle area

  vector<int>    v1;        // generic 1D int vector
  vector<double> v1d;       // generic 1D double vector
  vector<double> fvisc;     // viscous flux vector
  vector<double> liftdrag;  // the lift and drag (calculated in pplot.cpp)
  vector<double> del_t;     // timestep, delta t, at each node
  vector<double> fplus;     // positive split flux vector
  vector<double> fminus;    // negative split flux vector
  vector<double> b;         // right hand side for solving a given row (node)
  vector<double> offD;      // off diagonal contribution
  vector<double> Dx;        // The diagonal contribution; used to calculate residual
  vector<double> RMS;       // The Residual vector RMS = b - [offD]x - [D]x
  vector<double>  Q_L;      // Q_left for a given face
  vector<double>  Q_R;      // Q_right for a given face
  vector<double> node_area; // node area

  vector <vector<int> >    v2;      // generic 2D int vector
  vector <vector<double> > v2d;     // generic 2D double vector 
  vector< vector<double> > dfplus;  // positive split flux Jacobian
  vector< vector<double> > dfminus; // negative split flux Jacobian
  vector< vector<double> > dfwall;  // flux Jacobian at a solid surface
  vector< vector<double> > Q;       // State Vector
  vector< vector<double> > Q_x;     // x gradient at a given node Q_x[nn][4]
  vector< vector<double> > Q_y;     // y gradient at a given node Q_y[nn][4]
  vector <vector<double> > delQ;    // State vector at each node  delQ[nn][4]
  vector <vector<double> > RHS;     // Right Hand Side vector at each node RHS[nn][4]
  vector< vector<double> > dfvisc1(4,vector<double>(4)); // viscous flux jacobians
  vector< vector<double> > dfvisc2(4,vector<double>(4));
  vector< vector<double> > dfvisc3(4,vector<double>(4));

  vector< vector< vector<double> > > A; // Global matrix: A[ja_dim][4][4] 

  nn = node.size();

  //--- Set variables based on input file (input.in) -------------------------80
  accuracy = int_input[0];
  viscous  = int_input[1];
  maxInIt  = int_input[2];
  maxOutIt = int_input[3];
  restart  = int_input[9];
  design   = int_input[10];

  mach     = dub_input[0];
  alpha    = dub_input[1];
  gamma    = dub_input[2];
  rho      = dub_input[3];
  mu       = dub_input[4];
  CFLmin   = dub_input[5];
  CFLmax   = dub_input[6];
  iramp    = dub_input[7];

  ne = edge.size();
  nt = info[2];
  nbe = info[3];
  fflush(stdout);

  //--- Create node-to-node, node-to-edge, node-to-tri conectivity lists -----80
  vector< vector <int> > n2n; //node to node list n2n[nn][num connected nodes]
  vector< vector <int> > n2e; //node to edge list n2e[nn][num connected edges]
  vector< vector <int> > n2t; //node to tri  list n2t[nn][num connected tris]
  vector< vector <int> > t2e; //tri to edge  list t2e[nt][3]

  //--- initialize n2n, n2e and n2t with nn empty vectors ---
  for(i=0; i<nn; i++)
  {
    n2n.push_back(v1); //n2n[nn][conected nodes]
    n2e.push_back(v1); //n2e[nn][conected edges]
    n2t.push_back(v1); //n2t[nn][conected cells]
  }

  //--- create lists ---
  for(i=0; i<ne; i++)
  {
    n2n[edge[i][0]].push_back(edge[i][1]);
    n2n[edge[i][1]].push_back(edge[i][0]);
     
    n2e[edge[i][0]].push_back(i);
    n2e[edge[i][1]].push_back(i);
     
    vector<int>::iterator result;
    result = find( n2t[edge[i][0]].begin(), n2t[edge[i][0]].end(), edge[i][2] );
    if( result == n2t[edge[i][0]].end() )
       n2t[edge[i][0]].push_back(edge[i][2]);
    result = find( n2t[edge[i][1]].begin(), n2t[edge[i][1]].end(), edge[i][2] );
    if( result == n2t[edge[i][1]].end() )
       n2t[edge[i][1]].push_back(edge[i][2]);

    result = find( n2t[edge[i][0]].begin(), n2t[edge[i][0]].end(), edge[i][3] );
    if( result == n2t[edge[i][0]].end() )
       n2t[edge[i][0]].push_back(edge[i][3]);
    result = find( n2t[edge[i][1]].begin(), n2t[edge[i][1]].end(), edge[i][3] );
    if( result == n2t[edge[i][1]].end() )
       n2t[edge[i][1]].push_back(edge[i][3]);
  }

#if 0 //--- PRINT BLOCK ------------------------------------------------------80PRINT
printf("\n nn    = %2d",nn);
printf("\n ne    = %2d",ne);
printf("\n n2t   = %2d",n2t[278][5]);

printf("\n\n GOODBYE \n\n"); CHECKPT; exit(1);
#endif //--- PRINT BLOCK -----------------------------------------------------80PRINT

  //add node itself to each entry in n2n
  for (n=0; n < nn; n++)
    n2n[n].push_back(n);
  
  //--- Create triangle list -------------------------------------------------80
  //tri[nt][4]  This includes both interior and exterior triangles
  //the first entry indicates whether the triangle is interior (0) or exterior (1)
  vector< vector <int> > tri; 
  vector< vector <double> > centroid; 

  for(t=0; t<nt; t++)
  {
    tri.push_back(v1);       //create a vector of empty vectors nt units long
    tri[t].push_back(0);     //initialize all entries to interior
   
    centroid.push_back(v1d);   //create a centroid vector nt units long
  }

  for(n=0; n<nn; n++)
    for(i=0; i<n2t[n].size(); i++)
      tri[ n2t[n][i] ].push_back(n);
  
  //--- calculate tri-to-edge connectivity ---------------------------------80
  for(t=0; t<nt; t++)
    t2e.push_back(v1); //n2n[nn][conected nodes]

  for(i=0; i<ne; i++)
  {
    t2e[ edge[i][2] ].push_back(i);
    t2e[ edge[i][3] ].push_back(i);
  }

 //--- Make Sure triangles are Wound Correctly-------------------------------80
  for(i=0; i<ne; i++) // loop over edges
  {
    n1 = edge[i][0];
    n2 = edge[i][1];

    //--- Tri to the left ---
    if((tri[ edge[i][2] ][1] == n1 && tri[ edge[i][2] ][2] != n2) ||
       (tri[ edge[i][2] ][2] == n1 && tri[ edge[i][2] ][3] != n2) ||
       (tri[ edge[i][2] ][3] == n1 && tri[ edge[i][2] ][1] != n2) )
    { //if n1 isn't before n2 on tri-to-left, switch nodes so that it is
      n1 = tri[ edge[i][2] ][1];
      tri[ edge[i][2] ][1] = tri[ edge[i][2] ][2];
      tri[ edge[i][2] ][2] = n1;
    }

    //--- Tri to the right ---
    if((tri[ edge[i][3] ][1] == n1 && tri[ edge[i][3] ][2] == n2) ||
       (tri[ edge[i][3] ][2] == n1 && tri[ edge[i][3] ][3] == n2) ||
       (tri[ edge[i][3] ][3] == n1 && tri[ edge[i][3] ][1] == n2) )
    { //if n1 is before n2 on tri-to-right, switch nodes so that it isn't
      n1 = tri[ edge[i][3] ][1];
      tri[ edge[i][3] ][1] = tri[ edge[i][3] ][2];
      tri[ edge[i][3] ][2] = n1;
    }
  }

  //--- determine interior/exterior status -----------------------------------80
  // tri[t][0] = 1 -> exterior triangle
  // tri[t][0] = 0 -> interior triangle
  for(t=0; t<nt; t++)
  {
    if(tri[t].size() < 4)
      tri[t][0] = 1;
  }

  //--- Generate Mesh File ---------------------------------------------------80
  vector< vector<int> > intri;  // a list of interior triangles
  vector<double>  xvec; 
  vector<double>  yvec; 

  i=0;
  for(t=0; t<nt; t++)
  {
    if(tri[t][0] == 0)
    {
      intri.push_back(v1); 
      intri[i].push_back(tri[t][1]);
      intri[i].push_back(tri[t][2]);
      intri[i].push_back(tri[t][3]);
      
      i++;
    }
  }

  for(n=0; n<nn; n++)
  {
    xvec.push_back(node[n][0]);
    yvec.push_back(node[n][1]);
  }

  //--- Calculate centroids of interior triangles ----------------------------80
  //if triangle is exterior, centroid is set to 0,0
  for(t=0; t<nt; t++)
  {
    if(tri[t][0] == 0)
    {
      xmid = (node[ tri[t][1] ][0] + node[ tri[t][2] ][0] + node[ tri[t][3] ][0]) / 3.0;
      ymid = (node[ tri[t][1] ][1] + node[ tri[t][2] ][1] + node[ tri[t][3] ][1]) / 3.0;  
      centroid[t].push_back(xmid);
      centroid[t].push_back(ymid);
    }
    else
    {
      centroid[t].push_back(0.0);
      centroid[t].push_back(0.0);
    }
  }   

  //--- Calculate Control Volume Areas for each Node -------------------------80
  // Add area to end of node vector: node[nn][2] = area
  for(n=0; n<nn; n++)
  {
    node_area.push_back(0.0);
    for (i=0; i<n2t[n].size(); i++)
    {
      if( tri[ n2t[n][i] ][0] == 0)
      {
        node1 = tri[ n2t[n][i] ][1];
        node2 = tri[ n2t[n][i] ][2];
        node3 = tri[ n2t[n][i] ][3];
 
        tri_area = area(node[node1][0],node[node1][1],
                        node[node2][0],node[node2][1],
                        node[node3][0],node[node3][1]);

        node_area[n] += 1.0/3.0 * tri_area;
      }
    }
    node[n].push_back(node_area[n]);
  }

  //--- Build compressed row storage arrays ia, ja, iau ----------------------80
  vector<int> iau;
  vector<int> ia;
  vector<int> ja;

  i=0;
  for(n=0; n<nn; n++)
  {
    ia.push_back(i);
    for(j=0; j<n2n[n].size(); j++)
    {
      ja.push_back(n2n[n][j]);
      i++;
    } 
    iau.push_back(i-1);
  }
  ia.push_back(i);

  iau_dim = iau.size();
  ia_dim  = ia.size();
  ja_dim  = ja.size();

  alpha = alpha*pi/180.0;

  u = mach * cos(alpha);
  v = mach * sin(alpha);
  p = 1.0/gamma;
  e = p/(gamma - 1.0) + rho*0.5*(pow(u,2)+pow(v,2));

  Qfree.push_back(rho);
  Qfree.push_back(rho*u);
  Qfree.push_back(rho*v);
  Qfree.push_back(e);

  //--- Initialize Solution Vector -------------------------------------------80
  if(restart == 0) //Start from Freestream Conditions
  {
    for(n=0; n<nn; n++)
    {
      Q.push_back(v1d);
  
      Q[n].push_back(rho);
      Q[n].push_back(rho*u);
      Q[n].push_back(rho*v);
      Q[n].push_back(e);
    }

    for(i=0; i<ne; i++)
    {
      if(edge[i][4] == 1001)
      {
        n1 = edge[i][0];
        n2 = edge[i][1];

        Q[n1][1] = 0.0;
        Q[n1][2] = 0.0;
        Q[n1][3] = Twall / (gamma*(gamma-1.0)) * rho;
  
        Q[n2][1] = 0.0;
        Q[n2][2] = 0.0;
        Q[n2][3] = Twall / (gamma*(gamma-1.0)) * rho;
      }
    }
  }
  else if(restart == 1) //Start from Restart Conditions
  {
    for(i=0; i<4; i++)
      v1d.push_back(0.0);
    for(n=0; n<nn; n++)
      Q.push_back(v1d);
    v1d.clear();
  
// <tunnelk modification>
#if 0
    sprintf(filename, "smoothConverged.dat");
    if((fp=fopen(filename,"rb")) == NULL)
    {
      printf("\nCould not open file: <%s>\n",filename); exit(0);
    }
    rewind(fp);

    for(n=0; n<nn; n++)
      for(i=0; i<4; i++)
        fread(&Q[n][i], sizeof(double), 1, fp);
  
    fclose(fp);
    printf("\n Restart file: <%s>\n",filename);
#endif
// </tunnelk modification>
  }
  else
  {
    printf("\nError: Invalid Restart Flag");
    exit(0);
  }

  //--- Initialize Delta-Q and RHS Matrix to zero -----------------------------80

  for(j=0; j<4; j++)
  {
    v1d.push_back(0.0);
    Q_L.push_back(0.0);
    Q_R.push_back(0.0);
  }
    
  for(n=0; n<nn; n++)
  {
    delQ.push_back(v1d); 
    RHS.push_back(v1d); 
  }

  v1d.clear();
  for(j=0; j<4; j++)
    v1d.push_back(0.0);
  for(j=0; j<4; j++)
    v2d.push_back(v1d);
  for(n=0; n < ja_dim; n++)
    A.push_back(v2d);
 
  //--- FLOW SOLUTION LOOP Variables -----------------------------------------80
  it        = 1;   
  lin_it    = 1;  
  kickout = 1.0e-15; 

  for(i=0; i<4; i++)
  {
    offD.push_back(0.0);
    Dx.push_back(0.0);
    b.push_back(0.0);
    RMS.push_back(0.0);
  }

  //--------------------------------------------------------------------------80
  //------ FLOW SOLUTION LOOP (Outer Loop) -----------------------------------80
  do
  {
    //--- Zero Out A, RHS, and delQ ------------------------------------------80
    for(n=0; n<ja_dim; n++)
      for(i=0; i<4; i++)
        for(j=0; j<4; j++)
          A[n][i][j] = 0.0;

    for(n=0; n<nn; n++)
    {
      for(i=0; i<4; i++)
      {
        RHS[n][i] = 0.0;     // Residual (R)
        delQ[n][i] = 0.0;
      }
    }

    //--- Calculate CFL and delta t ------------------------------------------80
    ramp = ((double)it-1.0)/iramp;
    CFL = ramp*CFLmax + (1.0-ramp)*CFLmin;
   
    if(CFL > CFLmax)
      CFL = CFLmax;
 
    del_t.clear();
    for(n=0; n<nn; n++)
    {
      spd2 = (pow(Q[n][1],2) + pow(Q[n][2],2))/pow(Q[n][0],2);
      p = (gamma-1.0)*(Q[n][3]-0.5*Q[n][0]*spd2);
      c = sqrt(gamma*p/Q[n][0]);

      denominator =    0.0;
      for(i=0; i<n2e[n].size(); i++)
      {
        n1=edge[ n2e[n][i] ][0];
        n2=edge[ n2e[n][i] ][1];
        x_mid = (node[n1][0]+node[n2][0])/2.0;
        y_mid = (node[n1][1]+node[n2][1])/2.0;

        //face to the left of edge n2e[n][i]
        nx =   centroid[ edge[ n2e[n][i] ][2] ][1] - y_mid;
        ny = -(centroid[ edge[ n2e[n][i] ][2] ][0] - x_mid);
        len = sqrt( pow(nx,2) + pow(ny,2) );
        nx_hat = nx/len;
        ny_hat = ny/len;
        denominator += (fabs(Q[n][1]/Q[n][0]*nx_hat + Q[n][2]/Q[n][0]*ny_hat) + c)*len;

        //face to the right of edge n2e[n][i]
        nx =   centroid[ edge[ n2e[n][i] ][3] ][1] - y_mid;
        ny = -(centroid[ edge[ n2e[n][i] ][3] ][0] - x_mid);
        len = sqrt( pow(nx,2) + pow(ny,2) );
        nx_hat = nx/len;
        ny_hat = ny/len;
        denominator += (fabs(Q[n][1]/Q[n][0]*nx_hat + Q[n][2]/Q[n][0]*ny_hat) + c)*len;
      }
     
     del_t.push_back(CFL*node[n][2]/denominator);
    }
    
    //--- Add area/delta_t to Diagonals --------------------------------------80
    for(n=0; n<iau.size(); n++)
    {
      for(i=0; i<4; i++)
      {
        for(j=0; j<4; j++)
        {
          if(i == j)
          {
            A[ iau[n] ][i][j] = node_area[n]/del_t[n];
          }
        }
      }
    }

    //--- Calculate Gradients ------------------------------------------------80
    Q_x = gradx(Q,tri,n2t,node);
    Q_y = grady(Q,tri,n2t,node);

    //--- Loop over Edges and calculate contributions from Interior Cells ----80
    for(i=0; i<ne; i++)
    {
      n1=edge[i][0];
      n2=edge[i][1];
      x_mid = (node[n1][0]+node[n2][0])/2.0;
      y_mid = (node[n1][1]+node[n2][1])/2.0;
  
      //--- Triangle to the left of edge i -----------------------------------80
      x_face = (x_mid + centroid[ edge[i][2] ][0])/2.0;
      y_face = (y_mid + centroid[ edge[i][2] ][1])/2.0;
      nx =   centroid[ edge[i][2] ][1] - y_mid;
      ny = -(centroid[ edge[i][2] ][0] - x_mid);
  
      len = sqrt( pow(nx,2) + pow(ny,2) );

      if(accuracy == 1) 
      {
        for(j=0; j<4; j++)
        {
          Q_L[j] = Q[n1][j];
          Q_R[j] = Q[n2][j];
        }
      }
      else if(accuracy == 2) 
      {
        for(j=0; j<4; j++)
        {
          Q_L[j] = Q[n1][j] + Q_x[n1][j]*(x_face-node[n1][0])
                            + Q_y[n1][j]*(y_face-node[n1][1]);
          Q_R[j] = Q[n2][j] + Q_x[n2][j]*(x_face-node[n2][0])
                            + Q_y[n2][j]*(y_face-node[n2][1]);
        }
      }
      else
      {
        printf("\nERROR: Solver only handles first and second order accuracy\n");
        exit(0);
      }

      fplus   = fluxp(Q_L, nx, ny, len);
      fminus  = fluxm(Q_R, nx, ny, len);
      dfplus  = dfluxp(Q_L, nx, ny, len);
      dfminus = dfluxm(Q_R, nx, ny, len);

      RHS[ edge[i][0] ][0] += fplus[0] + fminus[0]; //Residual of n1
      RHS[ edge[i][0] ][1] += fplus[1] + fminus[1];
      RHS[ edge[i][0] ][2] += fplus[2] + fminus[2];
      RHS[ edge[i][0] ][3] += fplus[3] + fminus[3];
  
      RHS[ edge[i][1] ][0] -= fplus[0] + fminus[0]; //Residual of n2
      RHS[ edge[i][1] ][1] -= fplus[1] + fminus[1];
      RHS[ edge[i][1] ][2] -= fplus[2] + fminus[2];
      RHS[ edge[i][1] ][3] -= fplus[3] + fminus[3];

      for(j=0; j<4; j++)
      { 
        for(k=0; k<4; k++)
        {
          A[ iau[n1] ][j][k] += dfplus[j][k];  //value on diagonal of n1
          A[ iau[n2] ][j][k] -= dfminus[j][k]; //value on diagonal of n2
        }
      }

      for(n=ia[n1]; n<ia[n1+1]; n++)
      {
        if(ja[n] == n2)
        {         
          for(j=0; j<4; j++)
          {
            for(k=0; k<4; k++)
            {
              A[n][j][k] += dfminus[j][k]; //off diagonal of n1
            }
          }
        }
      }
  
      for(n=ia[n2]; n<ia[n2+1]; n++)
      {
        if(ja[n] == n1)
        {         
          for(j=0; j<4; j++)
          {
            for(k=0; k<4; k++)
            {
              A[n][j][k] -= dfplus[j][k]; //off diagonal of n2
            }
          }
        }
      }
    
      if(viscous)
      {
        if(tri[edge[i][2]][1]!=n1 && tri[edge[i][2]][1]!=n2 )
          n3 = tri[edge[i][2]][1];
        else if(tri[edge[i][2]][2]!=n1 && tri[edge[i][2]][2]!=n2 )
          n3 = tri[edge[i][2]][2];
        else 
          n3 = tri[edge[i][2]][3];
        
        fvisc = fluxV(n1, n2, n3, node, Q, Qfree, mu, mach, nx, ny, len);

        for(k=0; k<4; k++)
        {
          RHS[n1][k] -= fvisc[k]; //Viscous Residual added to n1 (edge[i][0])
          RHS[n2][k] += fvisc[k]; //Viscous Residual subtracted from n2 (edge[i][1])
        }
  
        dfluxV(n1, n2, n3, node, Q, Qfree, mu, mach, nx, ny, len, 
               dfvisc1, dfvisc2, dfvisc3); // Returns the viscous flux jacobians 

        //--- add (or subtract) diagonal contributions ---
        for(j=0; j<4; j++)
        { 
          for(k=0; k<4; k++)
          {
            A[ iau[n1] ][j][k] -= dfvisc1[j][k]; //diagonal of n1
            A[ iau[n2] ][j][k] += dfvisc2[j][k]; //diagonal of n2
          }
        }
    
        //--- add (or subtract) off-diagonal contributions ---
        for(n=ia[n1]; n<ia[n1+1]; n++) //off-diagonals of n1
        {
          if(ja[n] == n2)
          {         
            for(j=0; j<4; j++)
              for(k=0; k<4; k++)
                A[n][j][k] -= dfvisc2[j][k];
          }
          if(ja[n] == n3)
          {         
            for(j=0; j<4; j++)
              for(k=0; k<4; k++)
                A[n][j][k] -= dfvisc3[j][k];
          }
        }
    
        for(n=ia[n2]; n<ia[n2+1]; n++) //off-diagonals of n2
        {
          if(ja[n] == n1)
          {         
            for(j=0; j<4; j++)
              for(k=0; k<4; k++)
                A[n][j][k] += dfvisc1[j][k];
          }
          if(ja[n] == n3)
          {         
            for(j=0; j<4; j++)
              for(k=0; k<4; k++)
                A[n][j][k] += dfvisc3[j][k];
          }
        }
      }//End of Viscous Section

      //--- Triangle to the right of edge i ------------------------------------80
      if(tri[ edge[i][3] ][0] == 0) //check that triangle is in the interior
      {
        x_face = (x_mid + centroid[ edge[i][3] ][0])/2.0;
        y_face = (y_mid + centroid[ edge[i][3] ][1])/2.0;

        nx =   centroid[ edge[i][3] ][1] - y_mid;
        ny = -(centroid[ edge[i][3] ][0] - x_mid);
  
        len = sqrt( pow(nx,2) + pow(ny,2) );
      
        if(accuracy == 1) 
        {
          for(j=0; j<4; j++)
          {
            Q_L[j] = Q[n2][j];
            Q_R[j] = Q[n1][j];
          }
        }
        else if(accuracy == 2) 
        {
          for(j=0; j<4; j++)
          {
            Q_L[j] = Q[n2][j] + Q_x[n2][j]*(x_face-node[n2][0])
                              + Q_y[n2][j]*(y_face-node[n2][1]);
            Q_R[j] = Q[n1][j] + Q_x[n1][j]*(x_face-node[n1][0])
                              + Q_y[n1][j]*(y_face-node[n1][1]);
          }
        }
        else
        {
          printf("\nERROR: Solver only handles first and second order accuracy\n");
          exit(0);
        }

        fplus   = fluxp(Q_L, nx, ny, len);
        fminus  = fluxm(Q_R, nx, ny, len);
        dfplus  = dfluxp(Q_L, nx, ny, len);
        dfminus = dfluxm(Q_R, nx, ny, len);
  
        RHS[ edge[i][1] ][0] += fplus[0] + fminus[0];
        RHS[ edge[i][1] ][1] += fplus[1] + fminus[1];
        RHS[ edge[i][1] ][2] += fplus[2] + fminus[2];
        RHS[ edge[i][1] ][3] += fplus[3] + fminus[3];
  
        RHS[ edge[i][0] ][0] -= fplus[0] + fminus[0];
        RHS[ edge[i][0] ][1] -= fplus[1] + fminus[1];
        RHS[ edge[i][0] ][2] -= fplus[2] + fminus[2];
        RHS[ edge[i][0] ][3] -= fplus[3] + fminus[3];

        for(j=0; j<4; j++)
        { 
          for(k=0; k<4; k++)
          {
            A[ iau[n2] ][j][k] += dfplus[j][k];  //Diagonal of n2
            A[ iau[n1] ][j][k] -= dfminus[j][k]; //Diagonal of n1
          }
        }
     
        for(n=ia[n2]; n<ia[n2+1]; n++)
        {
          if(ja[n] == n1)
          {
            for(j=0; j<4; j++)
            {
              for(k=0; k<4; k++)
              {
                A[n][j][k] += dfminus[j][k]; //off-diagonal of n2
              }
            }
          }
        }
    
        for(n=ia[n1]; n<ia[n1+1]; n++)
        {
          if(ja[n] == n2)
          {
            for(j=0; j<4; j++)
            {
              for(k=0; k<4; k++)
              {
                A[n][j][k] -= dfplus[j][k]; //off-diagonal of n1
              }
            }
          }
        }

        if(viscous)
        {
          if(tri[edge[i][3]][1]!=n1 && tri[edge[i][3]][1]!=n2 )
            n3 = tri[edge[i][3]][1];
          else if(tri[edge[i][3]][2]!=n1 && tri[edge[i][3]][2]!=n2 )
            n3 = tri[edge[i][3]][2];
          else 
            n3 = tri[edge[i][3]][3];
            
          fvisc = fluxV(n1, n2, n3, node, Q, Qfree, mu, mach, nx, ny, len);  

          for(k=0; k<4; k++)
          {  
            RHS[n2][k] -= fvisc[k]; //Viscous Residual added to n2 (edge[i][1])
            RHS[n1][k] += fvisc[k]; //Viscous Residual subtracted from n1 (edge[i][0])
          }
    
          dfluxV(n1, n2, n3, node, Q, Qfree, mu, mach, nx, ny, len, 
                 dfvisc1, dfvisc2, dfvisc3); // Returns the viscous flux jacobians 
  
          //--- add (or subtract) diagonal contributions ---
          for(j=0; j<4; j++)
          { 
            for(k=0; k<4; k++)
            {
              A[ iau[n1] ][j][k] += dfvisc1[j][k]; //diagonal of n1
              A[ iau[n2] ][j][k] -= dfvisc2[j][k]; //diagonal of n2
            }
          }
    
          //--- add (or subtract) off-diagonal contributions ---
          for(n=ia[n1]; n<ia[n1+1]; n++) //off-diagonals of n1
          {
            if(ja[n] == n2)
            {         
              for(j=0; j<4; j++)
                for(k=0; k<4; k++)
                  A[n][j][k] += dfvisc2[j][k];
            }
            if(ja[n] == n3)
            {         
              for(j=0; j<4; j++)
                for(k=0; k<4; k++)
                  A[n][j][k] += dfvisc3[j][k];
            }
          }
      
          for(n=ia[n2]; n<ia[n2+1]; n++) //off-diagonals of n2
          {
            if(ja[n] == n1)
            {         
              for(j=0; j<4; j++)
                for(k=0; k<4; k++)
                  A[n][j][k] -= dfvisc1[j][k];
            }
            if(ja[n] == n3)
            {         
              for(j=0; j<4; j++)
                for(k=0; k<4; k++)
                  A[n][j][k] -= dfvisc3[j][k];
            }
          }
        }//End of Viscous Section
      
      }//end of interior check
    }//end of edge loop
  
    //--- Loop over Bounday Edges ----------------------------------------------80
    for(i=0; i<ne; i++)
    {
      if(edge[i][4] == 1000) // Solid Surface Inviscid Boundary ----------------80
      {
        n1=edge[i][0];
        n2=edge[i][1];
        x_mid = (node[n1][0]+node[n2][0])/2.0;
        y_mid = (node[n1][1]+node[n2][1])/2.0;
  
        v2_left  = pow(Q[n1][1]/Q[n1][0],2) + pow(Q[n1][2]/Q[n1][0],2);
        v2_right = pow(Q[n2][1]/Q[n2][0],2) + pow(Q[n2][2]/Q[n2][0],2);
        p_left  = (Q[n1][3] - 0.5*v2_left *Q[n1][0])*(gamma-1);
        p_right = (Q[n2][3] - 0.5*v2_right*Q[n2][0])*(gamma-1);
  
        //--- node 1 ---
        nx =   y_mid - node[n1][1];
        ny = -(x_mid - node[n1][0]);
        len = sqrt( pow(nx,2) + pow(ny,2) );
        RHS[n1][1] += nx*p_left;
        RHS[n1][2] += ny*p_left;
  
        dfwall = dfluxWall(Q[n1], nx, ny, len);
        for(j=0; j<4; j++)
        {
          for(k=0; k<4; k++)
          {
            A[ iau[n1] ][j][k] += dfwall[j][k];
          }
        }

        //--- node 2 ---
        nx =   node[n2][1] - y_mid ;
        ny = -(node[n2][0] - x_mid);
        len = sqrt( pow(nx,2) + pow(ny,2) );
        RHS[n2][1] += nx*p_right;
        RHS[n2][2] += ny*p_right;
  
        dfwall = dfluxWall(Q[n2], nx, ny, len);
        for(j=0; j<4; j++)
        {
          for(k=0; k<4; k++)
          {
            A[ iau[n2] ][j][k] += dfwall[j][k];
          }
        }
      }//end of solid surface Inviscid boundary
  
      if(edge[i][4] == 1001) // NO SLIP Boundary -----------------------------80
      {
        n1=edge[i][0];
        n2=edge[i][1];

        // Zero out momentum and energy of all entries for row n1 and n2
        for(n=ia[n1]; n<ia[n1+1]; n++)
          for(j=1; j<4; j++)
            for(k=0; k<4; k++)
              A[n][j][k] = 0.0;
  
        for(n=ia[n2]; n<ia[n2+1]; n++)
          for(j=1; j<4; j++)
            for(k=0; k<4; k++)
              A[n][j][k] = 0.0;

        // Calculate Diagonal
        A[ iau[n1] ][1][1] = 1.0;
        A[ iau[n1] ][2][2] = 1.0;
        A[ iau[n1] ][3][3] = 1.0;

        A[ iau[n2] ][1][1] = 1.0;
        A[ iau[n2] ][2][2] = 1.0;
        A[ iau[n2] ][3][3] = 1.0;

        A[ iau[n1] ][3][0] = -Twall / (gamma*(gamma-1.0));
        A[ iau[n2] ][3][0] = -Twall / (gamma*(gamma-1.0));
  
        //Set Residual of Momentum and Energy to Zero 
        for(j=1; j<4; j++)
        {
          RHS[n1][j] = 0.0;
          RHS[n2][j] = 0.0;
        }

      }//end of NO SLIP boundary
   
      if(edge[i][4] == 2000) // Freestream Boundary ----------------------------80
      {
        n1=edge[i][0];
        n2=edge[i][1];
        // Set Diagonal to Identity Matrix
        for(j=0; j<4; j++)
        {
          for(k=0; k<4; k++)
          {
            A[ iau[edge[i][0]] ][j][k] = 0.0;
            A[ iau[edge[i][1]] ][j][k] = 0.0;
            if(j == k)
            {
              A[ iau[edge[i][0]] ][j][k] = 1.0;
              A[ iau[edge[i][1]] ][j][k] = 1.0;
            }
          }
        }
        // Zero out the other entries
        for(n=ia[ edge[i][0] ]; n<ia[ edge[i][0]+1 ]; n++)
        {
          if(n != iau[ edge[i][0] ])
          {
            for(j=0; j<4; j++)
            {
              for(k=0; k<4; k++)
              {
                A[n][j][k] = 0.0;
              }
            }
          }
        }
  
        for(n=ia[ edge[i][1] ]; n<ia[ edge[i][1]+1 ]; n++)
        {
          if(n != iau[ edge[i][1] ])
          {
            for(j=0; j<4; j++)
            {
              for(k=0; k<4; k++)
              {
                A[n][j][k] = 0.0;
              }
            }
          }
        }
  
        //Set Residual to Zero 
        for(j=0; j<4; j++)
        {
          RHS[n1][j] = 0.0;
          RHS[n2][j] = 0.0;
        }

      }//end of freestream
    }//end of loop calculating boundary contributions to global matrix A
    
    //---*** LINEAR SYSTEM LOOP (inner Loop) ***--------------------------------80
    //-- Solve Ax=b for a given row (row n) using LU factoriazation ------------80
    //A, 4x4 = diagonal entry of the row: A[ [iau[n] ]
    //b, 1x4 = RHS minus all of the off diagonal entries: b-offD
    //x, 1x4 = delQ[n]

    //--- Solve Linear System Using Point Implicit Scheme ----------------------80
    do
    {
      RMStot = 0.0;
  
      for(n=0; n<nn; n++)
      {
        RMSnode = 0.0;
  
        for(i=0; i<4; i++)
        {
          b[i] = -RHS[n][i];
        }
  
        for(j=ia[n]; j<ia[n+1]; j++)
        {
          if( j != iau[n] )
          {
            for(i=0; i<4; i++)
            {
              offD[i] = A[j][i][0]*delQ[ja[j]][0] +
                        A[j][i][1]*delQ[ja[j]][1] +
                        A[j][i][2]*delQ[ja[j]][2] +
                        A[j][i][3]*delQ[ja[j]][3];
            }
  
            for(i=0; i<4; i++)
              b[i] -= offD[i];
          }
        }
          
        for(i=0; i<4; i++)
        {
          Dx[i] = A[iau[n]][i][0]*delQ[n][0] +
                  A[iau[n]][i][1]*delQ[n][1] +
                  A[iau[n]][i][2]*delQ[n][2] +
                  A[iau[n]][i][3]*delQ[n][3];
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
            if(isnan(A[ iau[n] ][i][j]) == 1)
            {
              printf("\nn=%d A[%d][%d][%d]=Nan\n",n,iau[n],i,j);
              printf("(%f, %f)\n",node[n][0],node[n][1]);
              printf("(Q[%d][0] = %f)\n",n,Q[n][0]);
              printf("(Q[%d][1] = %f)\n",n,Q[n][1]);
              printf("(Q[%d][2] = %f)\n",n,Q[n][2]);
              printf("(Q[%d][3] = %f)\n",n,Q[n][3]);
              exit(0);
            }
          }
        }
       
        flag = 0; // 0:minimize output, 1:print additional output from lu_solve
        delQ[n] = lu_solve(flag, A[iau[n]], b);

        RMStot += pow(RMSnode,2);
      }//End of node loop
         
      RMStot = sqrt(RMStot/nn);
  
      ntest = -1;    //a random node to test values of the solution vector
      if(ntest >= 0) //set to -1 if no output is desired
      {
        if(lin_it == 1)
        {
          printf("\n Values for node %d",ntest);
          printf("\n Step  Residual      d_rho    d_rho*u  d_rho*v  d_e      Area");
        }
        printf("\n%5d  %e %8.5f %8.5f %8.5f %8.5f %8.5f",
               lin_it,RMStot,delQ[ntest][0],delQ[ntest][1],delQ[ntest][2],
               delQ[ntest][3],node[ntest][2]);
      }

      lin_it++;

    }while(lin_it<=maxInIt && RMStot>kickout);
    //--- End of Inner Loop --------------------------------------------------80

    for(n=0; n<nn; n++)
      for(i=0; i<4; i++)
        Q[n][i] += delQ[n][i];

    //--- Calculate Right Hand Side Residual ---
    RMSout = 0.0;
    for(n=0; n<nn; n++)
    {
      RMSnode = 0.0;
      for(i=0; i<4; i++)
      {
         RMSnode += pow(RHS[n][i],2);
      }
      RMSnode = sqrt(RMSnode/4.0);

      RMSout += pow(RMSnode,2);
    }
    RMSout = sqrt(RMSout/nn);

    //--- Calculate delta Q Residual ---
    RMSdelQ = 0.0;
    for(n=0; n<nn; n++)
    {
      delQnode = 0.0;
      for(i=0; i<4; i++)
      {
         delQnode += pow(delQ[n][i],2);
      }
         delQnode = sqrt(delQnode/4.0);

         RMSdelQ += pow(delQnode,2);
    }
    RMSdelQ = sqrt(RMSdelQ/nn);
    
    if(it == 1 || it%100 == 0)
    {
      printf("\nOuter  Inner");
      printf("\nLoop   Loop   CFL#     RMS_RHS       RMSdelQ");
    }
 
    if(it%10 == 0)
      printf("\n%4d  %4d   %8.4f %13.6e %13.6e",it,lin_it-1,CFL,RMSout,RMSdelQ);
    fflush(stdout);

    lin_it = 1;   

    if(ntest >= 0)
    {
       printf("\n rho=%6.4f rho_u=%6.4f rho_v=%6.4f e=%6.4f",
           Q[ntest][0],Q[ntest][1],Q[ntest][2],Q[ntest][3]);
    }

    it++;
    kickwayout = kickout * 10.0;
  }while(it<=maxOutIt && RMSout>kickwayout);
  //--- End of Outer Loop ----------------------------------------------------80
  printf("\n%4d         %8.4f %13.6e %13.6e",it,CFL,RMSout,RMSdelQ);
  
  //--- Write Binary Restart File --------------------------------------------80

  sprintf(filename, "restart%02d.dat",ref_it-1);
  printf("\n");
  printf("\n writing out restart file: <%s>\n",filename);

// <tunnelk modification>
#if 0
  fp=fopen(filename,"wb"); //"wb" - write binary

  for(n=0; n<nn; n++)
    for(i=0; i<4; i++)
      fwrite(&Q[n][i], sizeof(double), 1, fp);
  
  fclose(fp);
#endif
// </tunnelk modification>

  //--- Generate Tecplot file with grid and solution -------------------------80
  extern const char* solution_filename;
  char* tplotfile = (char*)solution_filename;
  printf("\n writing out solution file: <%s>\n",tplotfile);
  tplot(tplotfile, xvec, yvec, Q, intri);

  //--- Generate Plot of Pressure Coeficients on the Solid Surface -----------80
  vector< vector<double> > surf_node; // x,y coordinates of nodes on a solid surface
  //vector<int> node_list;              // list of boundary condition for each node

  for(n=0; n<nn; n++)
  {
    for(i=0; i<n2e[n].size(); i++)
    {
      if(edge[ n2e[n][i] ][4] == 1000 || edge[ n2e[n][i] ][4] == 1001)
      {
        node_list.push_back(n);
        break;
      }
    }
  }

  printf("\nM_free=%6.4f AoA(deg)=%6.4f AoA(rads)=%6.4f\n",mach,alpha*180.0/pi,alpha);

  // --- Execute pplot -------------------------------------------------------80
  // pplot calculates lift, drag, pressure profile on the surface
  char pplotfile[32];
  sprintf(pplotfile, "pressure.gnu");

  liftdrag = pplot(pplotfile, alpha, node_list, node, Q, Qfree);

  lift = liftdrag[0];
  drag = liftdrag[1];

  printf(" Total lift = %f\n",lift);
  printf(" Total drag = %f",drag);
  printf("\n");

  return Q;
}
