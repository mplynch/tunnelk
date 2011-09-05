//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 3Feb09
// Discription:
//   Generate Pressure plot. This can be viewed using gnu.scr
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

vector<double> pplot(const char*              filename, 
                     double                   alpha,
                     vector<int>              surf_node,
                     vector< vector<double> > &node,
                     vector< vector<double> > &node_B,
                     vector< vector<double> > &Q,
                     vector< vector<double> > &Q_B,
                     vector<double>           &Qfree,
                     vector<double>           &liftdrag_B) //comes in empty
{
  printf("\nPressure Coefficient Filename = <%s>\n",filename);

  const double pi = 3.141592653589793;

  FILE *fp;
  int i;
  int n, bn;          // node number on top and bottom surface
  int snum;           // number of surface nodes
  double gamma = 1.4; // ratio of specific heats
  double spd2;        // speed squared
  double spd2_inf;    // freestream speed squared
  double dist;        // distance of node from nose
  double rho_inf;     // freestream density
  double u_inf;       // freestream x velocity
  double v_inf;       // freestream y velocity
  double e_inf;       // freestream internal energy
  double p_inf;       // freestream pressure

  vector<double> x;   // x coordinate
  vector<double> y;   // y coordinate
  vector<double> p;   // pressure    
  vector<double> cp;  // pressure coefficient

  //--- perturbation related variables ---
  double lift_pert;   // the lift from a perturbed mesh
  double drag_pert;   // the drag from a perturbed mesh
  double deltaBeta;   // the perturbation of the design variable
  double L_B;         // dLift/dBeta (forward mode differential)
  double dLdB;        // dLift/dBeta (finite difference)
  double spd2_B;      // derivative of speed squared w.r.t. Beta

  vector<double> x_B;   // derivative of x coordinate w.r.t. Beta
  vector<double> y_B;   // derivative of y coordinate w.r.t. Beta
  vector<double> p_B;   // derivative of pressure w.r.t. Beta
  vector<double> cp_B;  // derivative of pressure coefficient w.r.t. Beta

  snum = surf_node.size();

  //--- Freestream Conditions ---
  rho_inf  = Qfree[0];
  u_inf    = Qfree[1]/Qfree[0];
  v_inf    = Qfree[2]/Qfree[0];
  e_inf    = Qfree[3]; 
  spd2_inf = (pow(Qfree[1],2) + pow(Qfree[2],2))/pow(Qfree[0],2);
  p_inf    = (gamma-1.0)*(e_inf-0.5*rho_inf*spd2_inf);

  for(n=0; n<snum; n++)
  {
    x.push_back(node[surf_node[n]][0]);
    y.push_back(node[surf_node[n]][1]);

    x_B.push_back(node_B[surf_node[n]][0]);
    y_B.push_back(node_B[surf_node[n]][1]);
  }

  // IMPORTANT NOTE: n and surf_node[n] are the same in this case
  // this is not true generally
  for(n=0; n<snum; n++)
  {
    //spd2 = (pow(Q[n][1],2) + pow(Q[n][2],2))/pow(Q[n][0],2);
    spd2 = pow(Q[n][1]/Q[n][0],2) + pow(Q[n][2]/Q[n][0],2);
    p.push_back( (gamma-1.0)*(Q[n][3]-0.5*Q[n][0]*spd2) );
    cp.push_back( (p[n] - p_inf)/(0.5*rho_inf*spd2_inf));

    spd2_B = 2.0*(Q[n][1]/Q[n][0])*(Q_B[n][1]/Q[n][0] - (Q[n][1]*Q_B[n][0])/(Q[n][0]*Q[n][0]))
           + 2.0*(Q[n][2]/Q[n][0])*(Q_B[n][2]/Q[n][0] - (Q[n][2]*Q_B[n][0])/(Q[n][0]*Q[n][0]));
    p_B.push_back( (gamma-1.0)*(Q_B[n][3]-0.5*(Q_B[n][0]*spd2 + Q[n][0]*spd2_B)) );
    cp_B.push_back( p_B[n] / (0.5*rho_inf*spd2_inf) );
  }

  if ((fp = fopen(filename,"w")) == 0)
  {
    printf("\nError opening file <%s>.",filename);
    exit(0);
  }

  //--- Calculate Lift and Drag and lift_B and drag_B ------------------------80
  double xPlus, yPlus;   // the x and y values at n+1/2
  double xMinus, yMinus; // the x and y values at n-1/2
  double xHat, yHat;     // x and y components of the unit normal vector
  double xnoHat, ynoHat; // x and y components of the non-unit normal vector
  double len;            // length of the airfoil segment
  double force;          // magnitude of the force at surface node n  

  vector<double> drag(snum);  // drag contribution of node n
  vector<double> lift(snum);  // drag contribution of node n
  vector<double> liftdrag(2); // vector that is returned by pplot
                              // liftdrag[0] = lift;
                              // liftdrag[1] = drag;

  //--- Derivatives w.r.t. Beta ---
  double xPlus_B, yPlus_B;  
  double xMinus_B, yMinus_B;
  double xHat_B, yHat_B;   
  double xnoHat_B, ynoHat_B;   
  double len_B;           
  double force_B;        

  vector<double> drag_B(snum);
  vector<double> lift_B(snum);
  //vector<double> liftdrag_B(2); // this is now an input
  liftdrag_B.push_back(0.0);
  liftdrag_B.push_back(0.0);
 
  printf("\n alpha = %f degrees (%f radians)",alpha*180.0/pi,alpha);
  printf("\n Lift and Drag calculated on %d points\n",snum-1);
  liftdrag[0] = 0.0;
  liftdrag[1] = 0.0;
  liftdrag_B[0] = 0.0;
  liftdrag_B[1] = 0.0;

  for(n=1; n<snum; n++)
  {
    //--- Calculate unit normal ---
    if(n == snum-1)
    {
      xPlus = (x[0]+x[n])/2.0;
      yPlus = (y[0]+y[n])/2.0;

      xPlus_B = (x_B[0]+x_B[n])/2.0;
      yPlus_B = (y_B[0]+y_B[n])/2.0;
    }
    else
    {
      xPlus = (x[n+1]+x[n])/2.0;
      yPlus = (y[n+1]+y[n])/2.0;

      xPlus_B = (x_B[n+1]+x_B[n])/2.0;
      yPlus_B = (y_B[n+1]+y_B[n])/2.0;
    }
    xMinus = (x[n]+x[n-1])/2.0;
    yMinus = (y[n]+y[n-1])/2.0;
    xnoHat =   yPlus - yMinus;   // This will cause the normal to point into the surface
    ynoHat = -(xPlus - xMinus);  // which is the direction that the pressure would point
    len = sqrt(xnoHat*xnoHat + ynoHat*ynoHat);

    xHat  = xnoHat / len;        // convert normal to unit normal 
    yHat  = ynoHat / len;
    force = cp[n] * len;

    xMinus_B = (x_B[n]+x_B[n-1])/2.0;
    yMinus_B = (y_B[n]+y_B[n-1])/2.0;
    xnoHat_B =   yPlus_B - yMinus_B;
    ynoHat_B = -(xPlus_B - xMinus_B);
    len_B = 0.5/sqrt(xnoHat*xnoHat + ynoHat*ynoHat) * (2.0*xnoHat*xnoHat_B + 2.0*ynoHat*ynoHat_B);

    xHat_B  = xnoHat_B/len - (xnoHat*len_B)/(len*len);
    yHat_B  = ynoHat_B/len - (ynoHat*len_B)/(len*len);
    force_B = cp_B[n]*len + cp[n]*len_B;

#if 0
// This section prints out additional information for looking at finite difference
if(n == 42)
{
  printf("\n");
  printf("\nx%d    = %17.10e",n,x[n]);
  printf("\ny%d    = %17.10e",n,y[n]);

  printf("\nQ%d_0  = %17.10e",n,Q[n][0]);
  printf("\nQ%d_1  = %17.10e",n,Q[n][1]);
  printf("\nQ%d_2  = %17.10e",n,Q[n][2]);
  printf("\nQ%d_3  = %17.10e",n,Q[n][3]);

  printf("\np%d    = %17.10e",n,p[n]);
  printf("\ncp%d   = %17.10e",n,cp[n]);

  printf("\nxPlus  = %17.10e",xPlus);
  printf("\nyPlus  = %17.10e",yPlus);
  printf("\nxMinus = %17.10e",xMinus);
  printf("\nyMinus = %17.10e",yMinus);
  printf("\nxHat   = %17.10e",xHat);
  printf("\nyHat   = %17.10e",yHat);
  printf("\nlen    = %17.10e",len);
  printf("\nforce  = %17.10e",force);

  printf("\n");
  printf("\n--- Derivatives ---");
  printf("\nx_B%d  = %17.10e",n,x_B[n]);
  printf("\ny_B%d  = %17.10e",n,y_B[n]);

  printf("\nQ_B%d_0  = %17.10e",n,Q_B[n][0]);
  printf("\nQ_B%d_1  = %17.10e",n,Q_B[n][1]);
  printf("\nQ_B%d_2  = %17.10e",n,Q_B[n][2]);
  printf("\nQ_B%d_3  = %17.10e",n,Q_B[n][3]);

  printf("\np_B%d    = %17.10e",n,p_B[n]);
  printf("\ncp_B%d   = %17.10e",n,cp_B[n]);

  printf("\nxPlus_B  = %17.10e",xPlus_B);
  printf("\nyPlus_B  = %17.10e",yPlus_B);
  printf("\nxMinus_B = %17.10e",xMinus_B);
  printf("\nyMinus_B = %17.10e",yMinus_B);
  printf("\nxHat_B   = %17.10e",xHat_B);
  printf("\nyHat_B   = %17.10e",yHat_B);
  printf("\nlen_B    = %17.10e",len_B);
  printf("\nforce_B  = %17.10e",force_B);
}
#endif

  //--- Lift ---
    lift[n] = -force*xHat*sin(alpha) + force*yHat*cos(alpha);
    liftdrag[0] += lift[n];

    lift_B[n] = -(force_B*xHat*sin(alpha) + force*xHat_B*sin(alpha))
               + (force_B*yHat*cos(alpha) + force*yHat_B*cos(alpha));
    liftdrag_B[0] += lift_B[n];

  //--- Drag ---
    drag[n] = force*xHat*cos(alpha) + force*yHat*sin(alpha);
    liftdrag[1] += drag[n];

    drag_B[n] =  (force_B*xHat*cos(alpha) + force*xHat_B*cos(alpha))
               + (force_B*yHat*sin(alpha) + force*yHat_B*sin(alpha));
    liftdrag_B[1] += drag_B[n];
  }

#if 0
// This section prints out additional information for looking at forward mode
// differentials of lift and drag
  printf("\n node  lift");
  for(n=0; n<snum; n++)
  {
    //printf("\n %3d   %17.10e",n,lift[n]);
  }
  printf("\n");

  printf("\n lift[42]   = %17.10e",lift[42]);
  printf("\n lift[43]   = %17.10e",lift[43]);
  printf("\n lift[44]   = %17.10e",lift[44]);
  printf("\n Total Lift = %17.10e",liftdrag[0]);
  printf("\n");
  
  printf("\n lift_B[42]   = %17.10e",lift_B[42]);
  printf("\n lift_B[43]   = %17.10e",lift_B[43]);
  printf("\n lift_B[44]   = %17.10e",lift_B[44]);
  printf("\n TotalLift_B  = %17.10e",liftdrag_B[0]);
  printf("\n");
  printf("\n");

  lift_pert = 1.5355456977e-01;
  deltaBeta = 1.0e-7;
  dLdB = (lift_pert - liftdrag[0]) / deltaBeta;

  printf("\n");
  printf("\n Original Lift                   = %17.10e",liftdrag[0]);
  printf("\n Perturbed Lift                  = %17.10e",lift_pert);
  printf("\n DL/DB (finite difference)       = %10.3e",dLdB);
  printf("\n DL/DB (forward mode difference) = %10.3e",liftdrag_B[0]);
  printf("\n");

  printf("\n Original Drag = %17.10e",liftdrag[1]);
  printf("\n");
  printf("\n");
#endif

#if 0
// Print out Qs
printf("\n");
for(i=0; i<4; i++)
  printf("\n Q42_%d = %20.13e",i,Q[42][i]);
printf("\n");
for(i=0; i<4; i++)
  printf("\n Q43_%d = %20.13e",i,Q[43][i]);
printf("\n");
for(i=0; i<4; i++)
  printf("\n Q44_%d = %20.13e",i,Q[44][i]);

printf("\n");
exit(0);

#endif

  //--- Plot Cp on the airfoil surfaces --------------------------------------80
  dist = 0.0;
  i = 1;
  // cp at the nose
  fprintf(fp,"%10.6f %19.10e %19.10e\n",dist,cp[31],cp[31]);
  // cp on the top surface
  for (n=32; n<62 ; n++)
  {
    dist += sqrt( pow(x[n]-x[n-1],2) + pow(y[n]-y[n-1],2) );
  
    bn = n-2*i;
    fprintf(fp,"%10.6f %19.10e %19.10e\n",dist,cp[n],cp[bn]);
    i++;
  }
  dist += sqrt( pow(x[0]-x[61],2) + pow(y[0]-y[61],2) );
  fprintf(fp,"%10.6f %19.10e %19.10e\n",dist,cp[0],cp[0]); //cp at the tail

  fclose(fp);

  return liftdrag;
}
