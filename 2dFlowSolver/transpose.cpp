//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 20Nov
// Discription:
//  Transposes a jadim x [4x4] matrix in compressed row storage format
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;


vector< vector< vector<double> > > traspose(vector< vector< vector<double> > > R_Q,
                                            vector<int>                        iau,
                                            vector<int>                        ia,
                                            vector<int>                        ja)
{
  printf("\n Calculating Transpose");
  int i,j,k,n;  // counters
  int nn;       // number of nodes
  int offset;   // offset for calculating transpose position
  int iau_dim;  // dimensions of iau
  int ia_dim;   // dimensions of ia
  int ja_dim;   // dimensions of ja
  int row;      // the effective row in compressed row storage
  int column;   // the effective column in compressed row storage

  iau_dim = iau.size();
  ia_dim  = ia.size();
  ja_dim  = ja.size();

  // R_Q_T[jadim][4][4] is the transpose of R_Q. This is what is returned
  vector< vector< vector<double> > > R_Q_T(ja_dim,vector< vector<double> >(4,vector<double>(4)));

  //--- Transpose the Global Matrix ------------------------------------------80
#if 0
// Test Section
vector< vector<int> > A(5,vector<int>(5));;
nn = 5;
ia.clear(); iau.clear(); ja.clear();
ja.push_back(1); iau.push_back(4);  ia.push_back(0);
ja.push_back(2); iau.push_back(8);  ia.push_back(5);
ja.push_back(3); iau.push_back(12); ia.push_back(9);
ja.push_back(4); iau.push_back(16); ia.push_back(13);
ja.push_back(0); iau.push_back(20); ia.push_back(17);
ja.push_back(0);                    ia.push_back(21);
ja.push_back(2);   
ja.push_back(4);   
ja.push_back(1);   
ja.push_back(0);   
ja.push_back(1);   
ja.push_back(3);   
ja.push_back(2);   
ja.push_back(0);   
ja.push_back(2);   
ja.push_back(4);   
ja.push_back(3);   
ja.push_back(0);   
ja.push_back(1);   
ja.push_back(3);   
ja.push_back(4);   
vector<int> A1(ja.size());
vector<int> A1_T(ja.size());
A[0][0]=0;  A[0][1]=1;  A[0][2]=2;  A[0][3]=3;  A[0][4]= 4;
A[1][0]=10; A[1][1]=11; A[1][2]=12; A[1][3]=-1; A[1][4]=14;
A[2][0]=20; A[2][1]=21; A[2][2]=22; A[2][3]=23; A[2][4]=-1;
A[3][0]=30; A[3][1]=-1; A[3][2]=32; A[3][3]=33; A[3][4]=34;
A[4][0]=40; A[4][1]=41; A[4][2]=-1; A[4][3]=43; A[4][4]=44;
A1[0]=1;
A1[1]=2;
A1[2]=3;
A1[3]=4;
A1[4]=0;
A1[5]=10;
A1[6]=12;
A1[7]=14;
A1[8]=11;
A1[9]=20;
A1[10]=21;
A1[11]=23;
A1[12]=22;
A1[13]=30;
A1[14]=32;
A1[15]=34;
A1[16]=33;
A1[17]=40;
A1[18]=41;
A1[19]=43;
A1[20]=44;
printf("\nA1.size=%d",int(A1.size()) );

printf("\nia[0]=%d",ia[0]);
printf("\nia[1]=%d",ia[1]);
printf("\niau[0]=%d",iau[0]);
printf("\n");

for(n=0; n<nn; n++)
{
  row=n;
  for(j=ia[n]; j<ia[n+1]; j++)
  {
    column=ja[j];
    offset=0;
    for(k=ia[column]; k<ia[column+1]; k++)
    {
      if(row==ja[k]) break;
      offset++;
    }
    A1_T[ia[column]+offset] = A1[j];
    printf("\nrow=%d column=%d j=%2d offset=%d",row,column,j,offset);
    printf(" A1_T[ia[column]+offset]=%2d A1[j]=%2d",A1_T[ia[column]+offset],A1[j]);
  }
  printf("\n");
}

printf("\n");
for(i=0; i<A1_T.size(); i++)
  printf("A1_T[%2d]=%d\n",i,A1_T[i]);
printf("\n");

printf("\n");
exit(0);
#endif

nn = iau_dim;
for(n=0; n<nn; n++)
{
  row=n;
  for(j=ia[n]; j<ia[n+1]; j++)
  {
    column=ja[j];
    offset=0;
    for(k=ia[column]; k<ia[column+1]; k++)
    {
      if(row==ja[k]) break;
      offset++;
    }
    R_Q_T[ia[column]+offset] = R_Q[j];
  }
}

  //--- Transpose all of the 4x4 inner matrices ------------------------------80
  vector< vector< vector<double> > > TEMP = R_Q_T;
  for(n=0; n<ja_dim; n++)
    for(i=0; i<4; i++)
      for(j=0; j<4; j++)
        R_Q_T[n][i][j] = TEMP[n][j][i];
   TEMP.clear();

#if 0
  printf("\n");
  for(n=0; n<nn; n++)
  {
    printf(" R_Q[iau[%3d]]=%9.2e %9.2e %9.2e %9.2e  R_Q_T[iau[%3d]]=%9.2e %9.2e %9.2e %9.2e\n",n,R_Q[iau[n]][0][0],R_Q[iau[n]][0][1],R_Q[iau[n]][0][2],R_Q[iau[n]][0][3],n,R_Q_T[iau[n]][0][0],R_Q_T[iau[n]][0][1],R_Q_T[iau[n]][0][2],R_Q_T[iau[n]][0][3]);
    printf("               %9.2e %9.2e %9.2e %9.2e                  %9.2e %9.2e %9.2e %9.2e\n"  ,R_Q[iau[n]][1][0],R_Q[iau[n]][1][1],R_Q[iau[n]][1][2],R_Q[iau[n]][1][3],  R_Q_T[iau[n]][1][0],R_Q_T[iau[n]][1][1],R_Q_T[iau[n]][1][2],R_Q_T[iau[n]][1][3]);
    printf("               %9.2e %9.2e %9.2e %9.2e                  %9.2e %9.2e %9.2e %9.2e\n"  ,R_Q[iau[n]][2][0],R_Q[iau[n]][2][1],R_Q[iau[n]][2][2],R_Q[iau[n]][2][3],  R_Q_T[iau[n]][2][0],R_Q_T[iau[n]][2][1],R_Q_T[iau[n]][2][2],R_Q_T[iau[n]][2][3]);
    printf("               %9.2e %9.2e %9.2e %9.2e                  %9.2e %9.2e %9.2e %9.2e\n"  ,R_Q[iau[n]][3][0],R_Q[iau[n]][3][1],R_Q[iau[n]][3][2],R_Q[iau[n]][3][3],  R_Q_T[iau[n]][3][0],R_Q_T[iau[n]][3][1],R_Q_T[iau[n]][3][2],R_Q_T[iau[n]][3][3]);
    printf("\n");
  }
  printf("\n");

  printf("\n--- Compressed Row Storage Stats ---\n");
  printf("length of ja  = %d\n",ja_dim);
  printf("length of ia  = %d\n",ia_dim);
  printf("length of iau = %d\n",iau_dim);
  exit(0);
#endif

  return R_Q_T;
}
