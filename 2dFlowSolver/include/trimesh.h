#include "List.h"
#include "Linked_List.h"

int trimesh(int nn,int tdim,
            vector< vector< vector<int> > > bs,
            vector<double> x,
            vector<double> y,
            int tri[][3]);

int search(int seed, double xt, double yt, 
           double x[], double y[], 
           int tri[][3], int nbr[][3]);

void make_nbrs(Linked_List *mknbr,
               int nn, 
               int tdim, 
               int tri[][3], 
               int nbr[][3], 
               Linked_List **Lhash);
