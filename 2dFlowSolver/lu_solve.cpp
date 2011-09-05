//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 7Feb09
// Discription:
//  Code to solve a linear system of the form Ax = b using LU factorization 
//  b in this code is called rhs (right hand side)
//  the vector x is saved into rhs and returned
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

vector<double> lu_solve(int flag, vector< vector<double> > A, vector<double> rhs)
{
  // if flag = 1, additional debugging information is printed 
  int i, j, k;     //counter variables
  int d=1;         //odd(-) or even(+) number of row interchanges 
  int n;           //dimension of the square matrix A
  int imax;        
  double dum, sum, aamax; 
  
  vector<int> indx;           //records row permutations
  vector<double> scale;       //stores the implicit scaling of each row
 
  n = A.size();

  //--- Initialize vectors ---------------------------------------------------80
  for(i=0; i<n; i++)
  {
    indx.push_back(0);
    scale.push_back(0.0);
  }

  if(flag == 1)
  {
    printf("\n--- Matrix A ---\n");
    for(i=0; i<n; i++)
    {
      for(j=0; j<n; j++)
      {
        printf("%6.3f ",A[i][j]);
      }
      printf("\n");
    }

    printf("\n--- RHS, b ---\n");
    for(i=0; i<n; i++)
    {
      printf("%6.3f ",rhs[i]);
    }
    printf("\n");
  }

  //-------------------------------------------------------------------------80
  // Loop over rows to get the implicit scaling information and 
  // check for Singularity 
  for(i=0; i<n; i++)
  {
    aamax=0.0; 
    for(j=0; j<n; j++)
    {
      if( fabs(A[i][j]) > aamax )
        aamax=fabs(A[i][j]);
    }
    if(aamax == 0.0)
    {
      printf("\nERROR: Singular matrix\n");
      exit(1);
    }
    scale[i] = 1.0/aamax;
  }

  //--- Compute LU ----------------------------------------------------------80
  for(j=0; j<n; j++)
  {
    if(j > 0)
    {
      for(i=0; i<=j-1; i++)
      {
        sum=A[i][j];
        if(i > 0)
        {
          for(k=0; k<=i-1; k++)
          {
            sum -= A[i][k]*A[k][j];
          }
          A[i][j] = sum;
        }
      }
    }
    aamax=0.0; 
    for(i=j; i<n; i++)
    {
      sum=A[i][j];
      if(j > 0)
      {
        for(k=0; k<=j-1; k++)
        {
          sum -= A[i][k]*A[k][j];
        }
        A[i][j] = sum;
      }
      dum = scale[i] * fabs(sum);
      if(dum >= aamax)
      {
        imax=i;
        aamax=dum;
      }
    }
    if(j != imax)
    {
      for(k=0; k<n; k++)
      {
        dum = A[imax][k];
        A[imax][k] = A[j][k];
        A[j][k] = dum;
      }
      d=-d;
      scale[imax] = scale[j];
    }
    indx[j] = imax;
    if(j != n)
    {
      if(A[j][j] == 0.0)
        A[j][j] = 1.0e-20;
      dum = 1.0 / A[j][j]; 
      for(i=j+1; i<n; i++)
      {
        A[i][j] = A[i][j] * dum;
      }
    }
  }
 
  if(flag == 1)
  {
    printf("\n--- LU of Matrix A ---\n");
    for(i=0; i<n; i++)
    {
      for(j=0; j<n; j++)
      {
        printf("%6.3f ",A[i][j]);
      }
      printf("\n");
    }
    printf("\n");
    printf("odd (-1) or even (+1) number of row permutations = %d\n",d);
    printf("partial pivoting row permutaions: ");
    for(i=0; i<n; i++)
    {
      printf(" %d ",indx[i]);
    }
    printf("\n");
  }

  //--- solve for b (in Ax = b) using forward then back substitution ---------80
  //the lu subroutine uses partial pivoting to reduce roundoff error so row 
  //permutations must be accounted for.
  int ii; //first nonvanishing element of rhs
  int p;  //indext to deal with the permutations of LU

  ii=-1;
  //--- forward substitution ---
  for(i=0; i< n; i++)
  {
    p = indx[i];
    sum = rhs[p]; 
    rhs[p] = rhs[i];
    if(ii != -1)
    {
      for(j=ii; j<=i-1; j++)
      {
        sum -= A[i][j]*rhs[j];
      }
    }
    else 
    {
      if(sum != 0.0)
      {
        ii = i;
      }
    }
    rhs[i] = sum;
  } 

  //--- back substitution ---
  for(i=n-1; i>=0; i--)
  {
    sum = rhs[i];
    if(i < n-1)
    {
      for(j=i+1; j<n; j++)
      {
        sum -= A[i][j]*rhs[j];
      }
    }
    rhs[i] = sum / A[i][i];
  }

  if(flag == 1)
  {
    printf("\n--- Solution, x ---\n");
    for(i=0; i<n; i++)
    {
      printf("%6.3f ",rhs[i]);
    }
  }

  return rhs;
}
