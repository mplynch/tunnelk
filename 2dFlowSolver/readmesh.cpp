//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 21Feb09
// Discription:
//  Code for reading in 2D mesh with only one block
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

void readmesh(char* filename,
              vector< vector<double> >        &node,
              vector< vector<int> >           &trivec,
              vector< vector<int> >           &quadvec,
              vector< vector< vector<int> > > &bound)
{
  FILE *fp;
  int bdim = 120;
  char buff[bdim];
  int nblock;         // Number blocks
  int nn, nt, nq, nb; // Number of nodes, blocks, triangles, quads, boudaries
  int i, n, t, q, b;  // Counters
  int bc;             // boundary condition

  double *x, *y;      // x and y node coordinates
  int    *nbs;        // number of edges for each boundary    nbs[nb]
  int    **tri;       // tri-node connectivity  [nt][3]
  int    **quad;      // quad-node connectivity [nq][4]
  int    ***bs;       // nodes for each edge of each boundary bs[nb][nbs][2]

  //--- Open 2D grid file for reading ----------------------------------------80
  if ((fp=fopen(filename,"r")) == NULL)
  {
    printf("\nCould not open file <%s>\n",filename);
    exit(0);
  }
  printf(" Reading Mesh File: %s",filename);

  extern long mesh_offset;
  fseek(fp, mesh_offset, SEEK_SET);

  //--- Read in Nodes --------------------------------------------------------80
  fgets(buff,bdim,fp); // Node Header text from file
  fgets(buff,bdim,fp); // Line containing number of grid points
  sscanf(buff,"%d",&nn);
  printf("\n  Number of grid points    = %d",nn);
  if ((x = (double*)malloc(nn*sizeof(double))) == NULL)
  {
    printf("\nCould not allocate memory for x\n");
    exit(0);
  }
  if ((y = (double*)malloc(nn*sizeof(double))) == NULL)
  {
    printf("\nCould not allocate memory for y\n");
    exit(0);
  }

  for (n=0; n < nn; n++)
  {
    fgets(buff,bdim,fp);
    sscanf(buff,"%lg %lg",&x[n],&y[n]);
  }
  
  //--- Read in Blocks -------------------------------------------------------80
  fgets(buff,bdim,fp); // Block Header text from file
  fgets(buff,bdim,fp); // Line containing number of blocks
  sscanf(buff,"%d",&nblock);
  if (nblock != 1)
  {
    printf("\nError: Number of Blocks must be 1\n");
    exit(0);
  }
  printf("\n  Number of blocks         = %d",nblock);

  //--- Read in Triangles ----------------------------------------------------80
  fgets(buff,bdim,fp); // Triangle Header text from file
  fgets(buff,bdim,fp); // Line containing number of tri-elements
  sscanf(buff,"%d",&nt);
  printf("\n  Number of triangles      = %d",nt);
  if ((tri = (int**)malloc(nt*sizeof(int*))) == NULL)
  {
    printf("\nCould not allocate memory for triangle connectivity\n");
    exit(0);
  }
  for (t=0; t < nt; t++)
  {
    if ((tri[t] = (int*)malloc(3*sizeof(int))) == NULL)
    {
      printf("\nCould not allocate memory for triangle connectivity\n");
      exit(0);
    }

    // Indexing in mesh file should be FORTRAN-like 
    // (i.e. numbering starts at 1 instead of 0)
    fgets(buff,bdim,fp);
    sscanf(buff,"%d %d %d",&tri[t][0],&tri[t][1],&tri[t][2]);
    // decrement numbers for C indexing
    --tri[t][0];
    --tri[t][1];
    --tri[t][2];
  }
    
  //--- Read in Quads --------------------------------------------------------80
  fgets(buff,bdim,fp); // Quad Header text from file
  fgets(buff,bdim,fp); // Line containing number of quad-elements
  sscanf(buff,"%d",&nq);
  printf("\n  Number of quadrilaterals = %d",nq);

  if ((quad = (int**)malloc(nq*sizeof(int*))) == NULL)
  {
    printf("\nCould not allocate memory for quadrilateral connectivity");
    exit(0);
  }

  for (q=0; q < nq; q++)
  {
    if ((quad[q] = (int*)malloc(4*sizeof(int))) == NULL)
    {
      printf("\nCould not allocate memory for quadrilateral connectivity");
      exit(0);
    }
    // Read in quad connectivity
    // Indexing should be FORTRAN-like(i.e. numbering starts at 1, instead of 0)
    fgets(buff,bdim,fp);
    sscanf(buff,"%d %d %d %d",&quad[q][0],&quad[q][1],
                              &quad[q][2],&quad[q][3]);
    /* decrement numbers for c indexing */
    --quad[q][0];
    --quad[q][1];
    --quad[q][2];
    --quad[q][3];
  }

  //--- Read in Boundaries ---------------------------------------------------80
  fgets(buff,bdim,fp); //Number of boundaries header file
  fgets(buff,bdim,fp); //Read in number of boundaries
  sscanf(buff,"%d",&nb);
  printf("\n  Number of boundaries     = %d",nb);
  nbs = new int[nb];
  bs = (int***)malloc(nb*sizeof(int**));
  for (b=0; b < nb; b++)
  {
    fgets(buff,bdim,fp); //Read in header file
    fgets(buff,bdim,fp); //Read in boundary codition
    sscanf(buff,"%d",&bc);
    fgets(buff,bdim,fp); //Read in header file
    fgets(buff,bdim,fp); //Read in number of points on boundary
    sscanf(buff,"%d",&nbs[b]);
  
    //printf("\n   Boundary %d has %d segments",b,nbs[b]);
    //printf("\n   Boundary %d has boundary condition %d -",b,bc);
    if(bc == 1000)
      printf("\n   slip wall");
    else if(bc == 1001)
      printf("\n   no slip wall");
    else if(bc == 2000)
      printf("\n   freestream");
    else
      printf(" unknown boundary condition type!\n");

    bs[b] = (int**)malloc(nbs[b]*sizeof(int*));
    for (i=0; i < nbs[b]; i++)
    {
      bs[b][i] = (int*)malloc(3*sizeof(int));
      fgets(buff,bdim,fp);
      sscanf(buff,"%d %d",&bs[b][i][0], &bs[b][i][1]);
      bs[b][i][0]--;
      bs[b][i][1]--;
      
      bs[b][i][2] = bc; //add boundary codition to boundary segment.
    }
  }
  fclose(fp);

  //--- Convert Array Pointers to Vectors ------------------------------------80
  vector<int>              v0;   //generic 1D int vector
  vector< vector<int> >    v00;  //generic 2D int vector
  vector<double>           dv0;  //generic 1D double vector
  vector< vector<double> > dv00; //generic 2D double vector
  
  //--- nodes ---
  for(n=0; n<nn; n++)
  {
    dv0.push_back(x[n]);
    dv0.push_back(y[n]);
  
    node.push_back(dv0);
    dv0.clear();
  }

  // --- triangles ---
  for(t=0; t<nt; t++)
  {
    v0.push_back(tri[t][0]);
    v0.push_back(tri[t][1]);
    v0.push_back(tri[t][2]);
  
    trivec.push_back(v0);
    v0.clear();
  }

  //--- quads ---
  for(q=0; q<nq; q++)
  {
    v0.push_back(quad[q][0]);
    v0.push_back(quad[q][1]);
    v0.push_back(quad[q][2]);
    v0.push_back(quad[q][3]);
  
    quadvec.push_back(v0);
    v0.clear();
  }

  //--- boundaries ---
  for(b=0; b<nb; b++)
  {
    for(i=0; i<nbs[b]; i++)
    {
      v0.push_back(bs[b][i][0]);
      v0.push_back(bs[b][i][1]);
      v0.push_back(bs[b][i][2]);
 
      v00.push_back(v0);
      v0.clear();
    }
    bound.push_back(v00);
    v00.clear();
  }

  //--- Clean Up Memory ------------------------------------------------------80
  free(x);
  free(y);

  for (t=0; t<nt; t++)
    free(tri[t]);
  free(tri);

  for (q=0; q<nq; q++)
    free(quad[q]);
  free(quad);

  for (b=0; b<nb; b++)
    for (i=0; i<nbs[b]; i++)
      free(bs[b][i]);
  for (b=0; b<nb; b++)
    free(bs[b]);
  free(bs);
}
