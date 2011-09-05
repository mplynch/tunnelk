//----------------------------------------------------------------------------80 // Author: James S Masters
// Date: 17Nov09
// Discription:
//   Calculates [dI/dX][dX/dB] for the adjoint equation
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

double dCostdX(double                   alpha, 
               vector<int>              &surf_node,
               vector< vector<double> > &node,
               vector< vector<double> > &node_B,
               vector< vector<double> > &Q,
               vector<double>           &Qfree)
{
  printf("\n Calculating [dI/dX][dX/dB]");
  //*** The values that are being calcualtaed are dI/dB; however this is different 
  // than the values that pere calculated in pplot.cpp for the forward mode method
  // because this is acutally [dI/dX][dX/dB]. i.e. this is dI/dB with Q held constant. 
  int i,j,k,n;                 // counters
  int snum = surf_node.size(); // number of surface nodes
  int nn   = node.size();      // total number of nodes

  double gamma = 1.4; // ratio of specific heats
  double spd2_inf;    // freestream speed squared
  double rho_inf;     // freestream density
  double u_inf;       // freestream x velocity
  double v_inf;       // freestream y velocity
  double e_inf;       // freestream internal energy
  double p_inf;       // freestream pressure

  vector<double> rho(snum);  // density
  vector<double> e(snum);    // energy
  vector<double> spd2(snum); // speed squared
  vector<double> p(snum);    // pressure
  vector<double> cp(snum);   // pressure coefficient

  //--- Mesh variables ---
  vector<double> x;      // x coordinate
  vector<double> y;      // y coordinate
  double xPlus, yPlus;   // the x and y values at n+1/2
  double xMinus, yMinus; // the x and y values at n-1/2
  double xHat, yHat;     // x and y components of the unit normal vector
  double xnoHat, ynoHat; // x and y components of the non-unit normal vector
  double len;            // length of the airfoil segment
  double force;          // magnitude of the force at surface node n  

  //--- Cost Variables ---
  vector<double> drag(snum); // drag contribution of node n
  vector<double> lift(snum); // drag contribution of node n
  double lift_t = 0.106331;  // the target lift
  double drag_t = 0.142946;  // the target drag
  double w_lift = 1.0;       // the wieghting factor for the lift term
  double w_drag = 0.0;       // the wieghting factor for the drag term
  double dragTot = 0.0;      // Total Drag
  double liftTot = 0.0;      // Total Lift
  double I;                  // Cost

  //--- Derivatives w.r.t. Beta, Q FIXED ---
  vector<double> x_B;
  vector<double> y_B;
  double xPlus_B, yPlus_B;  
  double xMinus_B, yMinus_B;
  double xHat_B, yHat_B;   
  double xnoHat_B, ynoHat_B;   
  double len_B;           
  double force_B;
  vector<double> drag_B(snum);
  vector<double> lift_B(snum);
  double dragTot_B;
  double liftTot_B;

  //--- dI/dB = [dI/dX][dX/dB], this is what is returned ---
  // there will be one for each design variable
  double I_B;

  //--- Freestream Conditions ------------------------------------------------80
  rho_inf  = Qfree[0];
  u_inf    = Qfree[1]/Qfree[0];
  v_inf    = Qfree[2]/Qfree[0];
  e_inf    = Qfree[3]; 
  spd2_inf = (pow(Qfree[1],2) + pow(Qfree[2],2))/pow(Qfree[0],2);
  p_inf    = (gamma-1.0)*(e_inf-0.5*rho_inf*spd2_inf);

  //--- Get coordinates and derivatives of surface points --------------------80
  for(n=0; n<snum; n++)
  {
    x.push_back(node[surf_node[n]][0]);
    y.push_back(node[surf_node[n]][1]);

    x_B.push_back(node_B[surf_node[n]][0]);
    y_B.push_back(node_B[surf_node[n]][1]);
#if 0
// Check against finite difference
x[n] = x[n] + x_B[n]*1.0e-7;
y[n] = y[n] + y_B[n]*1.0e-7;
#endif
  }

  //--- Calculate Flow Variables on the surface-------------------------------80
  for(n=0; n<snum; n++)
  {
    //--- density ---
    rho[n]=Q[n][0];
    //--- energy ---
    e[n]=Q[n][3];
    //--- speed squared ---
    spd2[n]=(pow(Q[n][1],2) + pow(Q[n][2],2))/pow(Q[n][0],2);
    //--- pressure ---
    p[n]=(gamma-1.0)*(e[n]-0.5*rho[n]*spd2[n]);
    //--- pressure coefficient ---
    cp[n]=(p[n] - p_inf)/(0.5*rho_inf*spd2_inf);
  }

  //--- Calculate dI/dB, Q Fixed ---------------------------------------------80
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
    force_B = cp[n]*len_B;

    //--- Lift ---
    lift[n]   = - force*xHat*sin(alpha) + force*yHat*cos(alpha);
    lift_B[n] = -(force_B*xHat*sin(alpha) + force*xHat_B*sin(alpha))
               + (force_B*yHat*cos(alpha) + force*yHat_B*cos(alpha));
    liftTot += lift[n];
    liftTot_B += lift_B[n];

    //--- Drag ---
    drag[n]   =  force*xHat*cos(alpha) + force*yHat*sin(alpha);
    drag_B[n] = (force_B*xHat*cos(alpha) + force*xHat_B*cos(alpha))
               + (force_B*yHat*sin(alpha) + force*yHat_B*sin(alpha));
    dragTot += lift[n];
    dragTot_B += drag_B[n];
  }

  //--- Calculate Cost ---
  I = w_lift*(liftTot-lift_t)*(liftTot-lift_t) + w_drag*(dragTot-drag_t)*(dragTot-drag_t);

  I_B = 2.0*w_lift*(liftTot - lift_t)*liftTot_B
       + 2.0*w_drag*(dragTot - drag_t)*dragTot_B;

#if 0
//Error checking vs. finite Diff
  printf("\n");
  printf("lift[42]   = %17.10e\n",lift[42]);
  printf("lift[43]   = %17.10e\n",lift[43]);
  printf("lift[44]   = %17.10e\n",lift[44]);
  printf("lift_B42  = %17.10e\n",lift_B[42]);
  printf("lift_B43  = %17.10e\n",lift_B[43]);
  printf("lift_B44  = %17.10e\n",lift_B[44]);
  printf("I          = %17.10e\n",I);
  printf("I_B        = %17.10e\n",I_B);
  exit(0);
#endif

  return I_B;
}
