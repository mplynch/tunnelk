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
                     vector< vector<double> > &Q,
                     vector<double>           &Qfree)
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
  double dLdB;        // dLift/dBeta (finite difference)

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
  }

  // IMPORTANT NOTE: n and surf_node[n] are the same in this case
  // this is not true generally
  for(n=0; n<snum; n++)
  {
    spd2 = pow(Q[n][1]/Q[n][0],2) + pow(Q[n][2]/Q[n][0],2);
    p.push_back( (gamma-1.0)*(Q[n][3]-0.5*Q[n][0]*spd2) );
    cp.push_back( (p[n] - p_inf)/(0.5*rho_inf*spd2_inf));
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

  printf("\n alpha = %f degrees (%f radians)",alpha*180.0/pi,alpha);
  printf("\n Lift and Drag calculated on %d points\n",snum-1);
  liftdrag[0] = 0.0;
  liftdrag[1] = 0.0;

  for(n=1; n<snum; n++)
  {
    //--- Calculate unit normal ---
    if(n == snum-1)
    {
      xPlus = (x[0]+x[n])/2.0;
      yPlus = (y[0]+y[n])/2.0;
    }
    else
    {
      xPlus = (x[n+1]+x[n])/2.0;
      yPlus = (y[n+1]+y[n])/2.0;
    }
    xMinus = (x[n]+x[n-1])/2.0;
    yMinus = (y[n]+y[n-1])/2.0;
    xnoHat =   yPlus - yMinus;   // This will cause the normal to point into the surface
    ynoHat = -(xPlus - xMinus);  // which is the direction that the pressure would point
    len = sqrt(xnoHat*xnoHat + ynoHat*ynoHat);

    xHat  = xnoHat / len;        // convert normal to unit normal 
    yHat  = ynoHat / len;
    force = cp[n] * len;

    //--- Lift ---
    lift[n] = -force*xHat*sin(alpha) + force*yHat*cos(alpha);
    liftdrag[0] += lift[n];

    //--- Drag ---
    drag[n] = force*xHat*cos(alpha) + force*yHat*sin(alpha);
    liftdrag[1] += drag[n];
  }

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
