//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 17Nov09
// Discription:
//   Calculated dI/dQ for the adjoint equation
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

vector< vector<double> > dCostdQ(double                   alpha, 
                                 vector<int>              &surf_node,
                                 vector< vector<double> > &node,
                                 vector< vector<double> > &Q,
                                 vector<double>           &Qfree)
{
#if 0
//Perturb pt. 43 Qs
  Q[43][3] += 1.0e-7;
#endif

  printf("\n Calculating dI/dQ");
  const double pi = 3.141592653589793;

  int i,j,k,n;        // counters
  int snum;           // number of surface nodes
  int nn;             // total number of nodes
  double gamma = 1.4; // ratio of specific heats
  double spd2_inf;    // freestream speed squared
  double rho_inf;     // freestream density
  double u_inf;       // freestream x velocity
  double v_inf;       // freestream y velocity
  double e_inf;       // freestream internal energy
  double p_inf;       // freestream pressure

  vector<double> x;   // x coordinate
  vector<double> y;   // y coordinate

  //--- Cost Variables -------------------------------------------------------80
  // note that only vallues on the surface are needed for calculating cost
  snum = surf_node.size(); // IMPORTANT NOTE: n and surf_node[n] are the same in this case
  nn   = node.size();    
  double I;      // Cost
  double lift_t; // the target lift
  double drag_t; // the target drag
  double w_lift; // the wieghting factor for the lift term
  double w_drag; // the wieghting factor for the drag term

  vector<double> rho(snum);  // density
  vector<double> e(snum);    // energy
  vector<double> spd2(snum); // speed squared
  vector<double> p(snum);    // pressure
  vector<double> cp(snum);   // pressure coefficient
  vector< vector< double > > rho_Q(snum,vector<double>(4));   // drho/dQ 
  vector< vector< double > > e_Q(snum,vector<double>(4));     // de/dQ 
  vector< vector< double > > spd2_Q(snum,vector<double>(4));  // dspeedSquared/dQ
  vector< vector< double > > p_Q(snum,vector<double>(4));     // dp/dQ 
  vector< vector< double > > cp_Q(snum,vector<double>(4));    // dcp/dQ 
  vector< vector< double > > L_Q(snum,vector<double>(4));     // dLift/dQ 

  //--- Freestream Conditions ------------------------------------------------80
  rho_inf  = Qfree[0];
  u_inf    = Qfree[1]/Qfree[0];
  v_inf    = Qfree[2]/Qfree[0];
  e_inf    = Qfree[3]; 
  spd2_inf = (pow(Qfree[1],2) + pow(Qfree[2],2))/pow(Qfree[0],2);
  p_inf    = (gamma-1.0)*(e_inf-0.5*rho_inf*spd2_inf);

  //--- Get coordinates of surface points ------------------------------------80
  for(n=0; n<snum; n++)
  {
    x.push_back(node[surf_node[n]][0]);
    y.push_back(node[surf_node[n]][1]);
  }

  //--- Calculate derivatives w.r.t. Qs --------------------------------------80
  for(n=0; n<snum; n++)
  {
    //--- density ---
    rho[n]      = Q[n][0];
    rho_Q[n][0] = 1.0;
    rho_Q[n][1] = 0.0;
    rho_Q[n][2] = 0.0;
    rho_Q[n][3] = 0.0;

    //--- energy ---
    e[n]      = Q[n][3];
    e_Q[n][0] = 0.0;
    e_Q[n][1] = 0.0;
    e_Q[n][2] = 0.0;
    e_Q[n][3] = 1.0;

    //--- speed squared ---
    spd2[n]      = (pow(Q[n][1],2) + pow(Q[n][2],2))/pow(Q[n][0],2);
    spd2_Q[n][0] = -2.0*(pow(Q[n][1],2) + pow(Q[n][2],2))/pow(Q[n][0],3); 
    spd2_Q[n][1] = 2.0*Q[n][1]/pow(Q[n][0],2); 
    spd2_Q[n][2] = 2.0*Q[n][2]/pow(Q[n][0],2); 
    spd2_Q[n][3] = 0.0;
  
    //--- pressure ---
    p[n]      = (gamma-1.0)*(e[n]-0.5*rho[n]*spd2[n]);
    p_Q[n][0] = (gamma-1.0)*(e_Q[n][0]-0.5*rho[n]*spd2_Q[n][0]-0.5*spd2[n]*rho_Q[n][0]);
    p_Q[n][1] = (gamma-1.0)*(e_Q[n][1]-0.5*rho[n]*spd2_Q[n][1]-0.5*spd2[n]*rho_Q[n][1]);
    p_Q[n][2] = (gamma-1.0)*(e_Q[n][2]-0.5*rho[n]*spd2_Q[n][2]-0.5*spd2[n]*rho_Q[n][2]);
    p_Q[n][3] = (gamma-1.0)*(e_Q[n][3]-0.5*rho[n]*spd2_Q[n][3]-0.5*spd2[n]*rho_Q[n][3]);

    //--- pressure coefficient ---
    cp[n]      = (p[n] - p_inf)/(0.5*rho_inf*spd2_inf);
    cp_Q[n][0] = p_Q[n][0] / (0.5*rho_inf*spd2_inf);
    cp_Q[n][1] = p_Q[n][1] / (0.5*rho_inf*spd2_inf);
    cp_Q[n][2] = p_Q[n][2] / (0.5*rho_inf*spd2_inf);
    cp_Q[n][3] = p_Q[n][3] / (0.5*rho_inf*spd2_inf);
  }

  //--- Calculate Lift and Drag and lift_Q and drag_Q ------------------------80

  //--- Mesh variable (all derivatives w.r.t. Q are zero) ---
  double xPlus, yPlus;   // the x and y values at n+1/2
  double xMinus, yMinus; // the x and y values at n-1/2
  double xHat, yHat;     // x and y components of the unit normal vector
  double xnoHat, ynoHat; // x and y components of the non-unit normal vector
  double len;            // length of the airfoil segment
  double force;          // magnitude of the force at surface node n  

  vector< vector< double > > force_Q(snum,vector<double>(4));   // dforce/dQ 

  vector<double> drag(snum);  // drag contribution of node n
  vector<double> lift(snum);  // drag contribution of node n

  double dragTot = 0.0;    // Total Drag
  double liftTot = 0.0;    // Total Lift

  vector< vector<double> > drag_Q(snum,vector<double>(4)); // dD/dQ at a single point
  vector< vector<double> > lift_Q(snum,vector<double>(4)); // dL/dQ at a single point
  //vector< vector< vector<double> > > drag_Q(snum,vector< vector<double> >(snum,vector<double>(4)));
  //vector< vector< vector<double> > > lift_Q(snum,vector< vector<double> >(snum,vector<double>(4)));

  vector< vector<double> > dragTot_Q(snum,vector<double>(4)); // dD/dQ for entire surface 
  vector< vector<double> > liftTot_Q(snum,vector<double>(4)); // dL/dQ for entire surface 
 
  // Derivative of cost with respect to Q; this is what is Returned
  // It is nnx4 long but only the surface nodes are non-zero
  vector< vector<double> > I_Q(nn,vector<double>(4));

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

    //--- Calculate Force ---
    force         = cp[n] * len;
    force_Q[n][0] = cp_Q[n][0] * len;
    force_Q[n][1] = cp_Q[n][1] * len;
    force_Q[n][2] = cp_Q[n][2] * len;
    force_Q[n][3] = cp_Q[n][3] * len;

    //--- Lift ---
    lift[n]      = -force*xHat*sin(alpha) + force*yHat*cos(alpha);
    lift_Q[n][0] = -force_Q[n][0]*xHat*sin(alpha) + force_Q[n][0]*yHat*cos(alpha);
    lift_Q[n][1] = -force_Q[n][1]*xHat*sin(alpha) + force_Q[n][1]*yHat*cos(alpha);
    lift_Q[n][2] = -force_Q[n][2]*xHat*sin(alpha) + force_Q[n][2]*yHat*cos(alpha);
    lift_Q[n][3] = -force_Q[n][3]*xHat*sin(alpha) + force_Q[n][3]*yHat*cos(alpha);

    liftTot += lift[n];
    liftTot_Q[n][0] += lift_Q[n][0];
    liftTot_Q[n][1] += lift_Q[n][1];
    liftTot_Q[n][2] += lift_Q[n][2];
    liftTot_Q[n][3] += lift_Q[n][3];

    //--- Drag ---
    drag[n]      = force*xHat*cos(alpha) + force*yHat*sin(alpha);
    drag_Q[n][0] = force_Q[n][0]*xHat*cos(alpha) + force_Q[n][0]*yHat*sin(alpha);
    drag_Q[n][1] = force_Q[n][1]*xHat*cos(alpha) + force_Q[n][1]*yHat*sin(alpha);
    drag_Q[n][2] = force_Q[n][2]*xHat*cos(alpha) + force_Q[n][2]*yHat*sin(alpha);
    drag_Q[n][3] = force_Q[n][3]*xHat*cos(alpha) + force_Q[n][3]*yHat*sin(alpha);

    dragTot += lift[n];
    dragTot_Q[n][0] += drag_Q[n][0];
    dragTot_Q[n][1] += drag_Q[n][1];
    dragTot_Q[n][2] += drag_Q[n][2];
    dragTot_Q[n][3] += drag_Q[n][3];
  }

  //--- Calculate Cost ---
  w_lift = 1.0;
  w_drag = 0.0;
  lift_t = 0.106331;
  drag_t = 0.142946;
  I = w_lift*(liftTot-lift_t)*(liftTot-lift_t) + w_drag*(dragTot-drag_t)*(dragTot-drag_t);

  for(n=1; n<snum; n++)
    for(i=0; i<4; i++)
      I_Q[n][i] =  2.0*w_lift*(liftTot - lift_t)*liftTot_Q[n][i]
                 + 2.0*w_drag*(dragTot - drag_t)*dragTot_Q[n][i];

#if 0
//Error checking vs. finite Diff
  printf("\n");
  printf("liftTot          = %17.10e\n",liftTot);
  printf("liftTot_Q[43][0] = %17.10e\n",liftTot_Q[43][0]);

  for(i=0; i<4; i++)
    printf("Q[43][%d] = %17.10e\n",i,Q[43][i]);

  printf("\nCost       = %17.10e\n",I);
  for(i=0; i<4; i++)
    printf("Cost_Q43_%d = %17.10e\n",i,I_Q[43][i]);

  exit(0);
#endif

  return I_Q;
}
