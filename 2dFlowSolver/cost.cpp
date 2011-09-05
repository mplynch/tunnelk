//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 3Feb09
// Discription:
//  Calculates value of cost function
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

int cost(vector< vector<double> > &Q,
         vector< vector<double> > &Q_B,
         double                   lift, // lift on the real part of the mesh 
         double                   drag, // drag on the real part of the mesh
         vector<double>           &liftdrag_B) // derivatives of lift and drag
{
  FILE *fp;
  char filename[32];
  int bdim = 100;
  char buff[bdim];

  int i,j,k;        // counters
  double lift_t;    // the target lift
  double drag_t;    // the target drag
  double w_lift;    // the wieghting factor for the lift term
  double w_drag;    // the wieghting factor for the drag term
  double cost;      // Cost on the (real part of) mesh that was solved
  double cost_pert; // the cost on the perturbed mesh
  double lift_pert; // the lift on the perturbed mesh
  double drag_pert; // the drag on the perturbed mesh
  double lift_B;    // derivative of lift w.r.t. Beta
  double drag_B;    // derivative of drag w.r.t. Beta
  double cost_B;    // derivative of cost w.r.t. Beta

  sprintf(filename, "liftdrag.txt");
  printf("\n Printing Lift and Drag to File: <%s>\n",filename);
  if ((fp = fopen(filename,"r+")) == 0)
  {
    printf("\nError opening file <%s>.",filename);
    return(1);
  }
  rewind(fp);
  fprintf(fp,"%21.12e\n",lift);
  fprintf(fp,"%21.12e\n",drag);
  fclose(fp);

  lift_B = liftdrag_B[0];
  drag_B = liftdrag_B[1];

#if 1
  w_lift = 1.0;
  w_drag = 0.0;
  lift_t = 0.106331;
  drag_t = 0.142946;

  cost =  w_lift*(lift - lift_t)*(lift - lift_t)
        + w_drag*(drag - drag_t)*(drag - drag_t);

  cost_B =  2.0*w_lift*(lift - lift_t)*lift_B
          + 2.0*w_drag*(drag - drag_t)*drag_B;

#if 0
  printf(" Lift        = %21.12e\n",lift);
  printf(" target Lift = %21.12e\n",lift_t);
  printf(" lift_B      = %21.12e\n",lift_B);
  printf(" Drag        = %21.12e\n",drag);
  printf(" target Drag = %21.12e\n",drag_t);
  printf(" Drag_B      = %21.12e\n",drag_B);
  printf(" Cost        = %21.12e\n",cost);
  printf(" Cost_B      = %21.12e\n",cost_B);
#endif

  sprintf(filename, "cost_b.txt");
  printf("\n Printing dirivative of cost to File: <%s>\n",filename);
  if ((fp = fopen(filename,"r+")) == 0)
  {
    printf("\nError opening file <%s>.",filename);
    return(1);
  }
  rewind(fp);
  fprintf(fp,"%21.12e\n",cost_B);
  fclose(fp);
#endif

  return 0;
}
