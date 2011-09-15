//----------------------------------------------------------------------------80j
// Author: James S Masters
// Date: 08Apr09
// Discription:
//  Convert from eptr format to .mesh format
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

#include "List.h"

void convert(vector< vector<double> >        &node,
             vector< vector<int> >           &tri,
             vector< vector< vector<int> > > &bound,
             vector< vector<int> >           &edge,
             vector<int>                     &info)
{
  int i,j,k,n,t;  // counters
  int nn, nt, ne; // number of nodes, triangles and edges
  int nbe;        // number of boundary edges
  int idum=0;     // dummy variable for info vector
  int n1, n2;     // node variables
  
  List **nhash; // node-to-node connectivity     
  List **thash; // node-to-triangle connectivity  

  vector<int> v0;   //generic 1D int vector

  nn = node.size();
  nt = tri.size();

  vector<int> b_check(nn);
  
  printf("\n\n Converting from .mesh format to eptr (edge-pointer) format");
  printf("\n  Number of Nodes     = %d",nn);
  printf("\n  Number of Triangles = %d",nt);

  //--- Establish Connectivity -----------------------------------------------80
  // node-to-node       nhash[nn][conntected nodes]
  // node-to-tri        thash[nn][conntected tris]
  // edge connectivity  edge[ne][4] - node1, node2, tri-to-left, tri-to-right
  
  thash = new List*[nn];
  nhash = new List*[nn];

  for (n=0; n<nn; n++)
    nhash[n] = new List();
  for (n=0; n<nn; n++)
    thash[n] = new List();

  //--- node-to-node & node-to-tri conectivity --------------------------------80
  for (t=0; t < nt; t++)
  {
    for (i=0; i < 3; i++)
    {
      if(i == 0 && !nhash[tri[t][i]]->Is_In_List(tri[t][1]))
        nhash[tri[t][i]]->Add_To_List(tri[t][1]);
      if(i == 0 && !nhash[tri[t][i]]->Is_In_List(tri[t][2]))
        nhash[tri[t][i]]->Add_To_List(tri[t][2]);
      if(i == 1 && !nhash[tri[t][i]]->Is_In_List(tri[t][0]))
        nhash[tri[t][i]]->Add_To_List(tri[t][0]);
      if(i == 1 && !nhash[tri[t][i]]->Is_In_List(tri[t][2]))
       nhash[tri[t][i]]->Add_To_List(tri[t][2]);
      if(i == 2 && !nhash[tri[t][i]]->Is_In_List(tri[t][0]))
        nhash[tri[t][i]]->Add_To_List(tri[t][0]);
      if(i == 2 && !nhash[tri[t][i]]->Is_In_List(tri[t][1]))
        nhash[tri[t][i]]->Add_To_List(tri[t][1]);

      thash[tri[t][i]]->Add_To_List(t);
    }
  }
  //--- Add node itself to each node list in nhash ---
  for (n=0; n < nn; n++)
    nhash[n]->Add_To_List(n);
 
  //--- edge Connectivity ----------------------------------------------------80
  v0.push_back(0); v0.push_back(0);
  for (n=0; n<nn; n++)
  {
    for (i=0; i<nhash[n]->max; i++)
    {
      if( nhash[n]->list[i] > n )
      {
        v0[0] = n;
        v0[1] = nhash[n]->list[i];
        edge.push_back(v0);
      }
    }
  }
  v0.clear();
  ne = edge.size();
  printf("\n  Number of Edges     = %d\n",ne);

  for(i=0; i<ne; i++)
  {
    edge[i].push_back(-1); // triangle to the left
    edge[i].push_back(-1); // triangle to the right
    edge[i].push_back(0);  // edge boundary condition (0 for interior)
  }

  for(i=0; i<ne; i++)
  {
    n1 = edge[i][0];
    n2 = edge[i][1];
    for(j=0; j<thash[n1]->max; j++)
    {
      t = thash[n1]->list[j];
      if(tri[t][0] == n1)
      {
        if(tri[t][1] == n2)
          edge[i][2] = t;
        if(tri[t][2] == n2)
          edge[i][3] = t;
      }
      else if(tri[t][1] == n1)
      {
        if(tri[t][2] == n2)
          edge[i][2] = t;
        if(tri[t][0] == n2)
          edge[i][3] = t;
      }
      else if(tri[t][2] == n1)
      {
        if(tri[t][0] == n2)
          edge[i][2] = t;
        if(tri[t][1] == n2)
          edge[i][3] = t;
      }
      else
      {
        printf("\nERROR: Problem with conversion\n");
      }
    }
    
    if(edge[i][2] == -1)
    {
      edge[i][0] = n2;
      edge[i][1] = n1;
      edge[i][2] = edge[i][3];
      edge[i][3] = -1;
    }
  }

  //--- Boundaries -----------------------------------------------------------80
  nbe = 0;
  for (i=0; i<bound.size(); i++)
  {
    for (j=0; j<bound[i].size(); j++)
    {
      b_check[ bound[i][j][0] ] =  bound[i][j][2];
      nbe++;
    }
  }

  printf("  Number of Bound Pts = %d\n",nbe);
  
  //--- Exterior Triangles ---------------------------------------------------80
  int extri = nt;
  for(i=0; i<ne; i++)
  {
    if(b_check[ edge[i][0] ] != 0)
    {
      if(b_check[ edge[i][1] ] != 0)
      {
        edge[i][4] = b_check[ edge[i][1] ]; 
        edge[i][3] = extri; //include exterior triangles
        extri++;
      }
    }
  }

  info.push_back(nn);
  info.push_back(ne);
  info.push_back(extri);
  info.push_back(nbe);
  info.push_back(idum);
  info.push_back(idum);

  b_check.clear();
  v0.clear();
}
