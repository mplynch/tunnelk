//----------------------------------------------------------------------------80
// Author: James S Masters
// Date: 10Feb09
// Discription:
//  Calculate Condition Number
//----------------------------------------------------------------------------80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;

#define MIN(x,y) ((x) <= (y) ? (x) : (y))
#define MAX(x,y) ((x) >= (y) ? (x) : (y))

double area(double x1, double y1,
            double x2, double y2,
            double x3, double y3)
{
  //area follows right hand rule and will be negative if uXv is wound clockwise
  double u_x, v_x, u_y, v_y;
  double tri_area;

  u_x = x2 - x1;
  v_x = x3 - x1;
  u_y = y2 - y1;
  v_y = y3 - y1;

  tri_area = 0.5*(u_x*v_y - u_y*v_x);

  return tri_area;
}

double area_B(double x1,   double y1,
              double x2,   double y2,
              double x3,   double y3,
              double x1_B, double y1_B,
              double x2_B, double y2_B,
              double x3_B, double y3_B)
{
  //area follows right hand rule and will be negative if uXv is wound clockwise
  double u_x, v_x, u_y, v_y;
  double u_x_B, v_x_B, u_y_B, v_y_B;
  double tri_area_B;

  u_x = x2 - x1;
  v_x = x3 - x1;
  u_y = y2 - y1;
  v_y = y3 - y1;

  u_x_B = x2_B - x1_B;
  v_x_B = x3_B - x1_B;
  u_y_B = y2_B - y1_B;
  v_y_B = y3_B - y1_B;

  tri_area_B = 0.5*( (u_x_B*v_y + u_x*v_y_B) 
                   - (u_y_B*v_x + u_y*v_x_B) );

  return tri_area_B;
}


double aspect_ratio(double x1, double y1,
                    double x2, double y2,
                    double x3, double y3)
{
  double ar;     //aspect ratio
  double vx;     //x vector
  double vy;     //y vector
  double s;      //semiperimeter of the triangle
  double len[3]; //length of the triangle side
 
  vx = x2 - x1;
  vy = y2 - y1;
  len[0]  = sqrt(vx*vx + vy*vy);

  vx = x3 - x2;
  vy = y3 - y2;
  len[1]  = sqrt(vx*vx + vy*vy);

  vx = x1 - x3;
  vy = y1 - y3;
  len[2]  = sqrt(vx*vx + vy*vy);


  s = 0.5 * (len[0] + len[1] + len[2]);
  ar = ( len[0]*len[1]*len[2] ) /
       ( 8.0*(s - len[0])*(s - len[1])*(s - len[2]) );

  return ar;
}

double condition_number(double x1, double y1,
                        double x2, double y2,
                        double x3, double y3)
{
  double v1_x, v1_y, v2_x, v2_y;
  double A[2][2];
  double W[2][2];
  double Ainv[2][2];
  double Winv[2][2];
  double A_Winv[2][2]; //A_Winv = A*W_inverse
  double W_Ainv[2][2]; //W_Ainv = W*A_inverse
  double normA_Winv;
  double normW_Ainv;
  double Adet, Wdet;
  double CN; // Condition Number

  W[0][0]= 1.0; W[0][1]= 0.5;
  W[1][0]= 0.0; W[1][1]= sqrt(3.0)/2.0;
  Wdet = W[0][0]*W[1][1] - W[0][1]*W[1][0];
  Winv[0][0] = 1/Wdet * W[1][1];
  Winv[0][1] = 1/Wdet * -1 *W[0][1];
  Winv[1][0] = 1/Wdet * -1 *W[1][0];
  Winv[1][1] = 1/Wdet * W[0][0];

  v1_x = x2 - x1;
  v1_y = y2 - y1;
  v2_x = x3 - x1;
  v2_y = y3 - y1;
  A[0][0]= v1_x; A[0][1]= v2_x;
  A[1][0]= v1_y; A[1][1]= v2_y;
  Adet = A[0][0]*A[1][1] - A[0][1]*A[1][0];

  Ainv[0][0] = 1/Adet * A[1][1];
  Ainv[0][1] = 1/Adet * -1 *A[0][1];
  Ainv[1][0] = 1/Adet * -1 *A[1][0];
  Ainv[1][1] = 1/Adet * A[0][0];

  //Calculate A*W_inverse and W*A_inverse
  A_Winv[0][0] = A[0][0]*Winv[0][0] + A[0][1]*Winv[1][0];
  A_Winv[0][1] = A[0][0]*Winv[0][1] + A[0][1]*Winv[1][1];
  A_Winv[1][0] = A[1][0]*Winv[0][0] + A[1][1]*Winv[1][0];
  A_Winv[1][1] = A[1][0]*Winv[0][1] + A[1][1]*Winv[1][1];

  W_Ainv[0][0] = W[0][0]*Ainv[0][0] + W[0][1]*Ainv[1][0];
  W_Ainv[0][1] = W[0][0]*Ainv[0][1] + W[0][1]*Ainv[1][1];
  W_Ainv[1][0] = W[1][0]*Ainv[0][0] + W[1][1]*Ainv[1][0];
  W_Ainv[1][1] = W[1][0]*Ainv[0][1] + W[1][1]*Ainv[1][1];

  // ||A|| = sqrt( trace(A_transpose * A) )
  // ||A|| = sqrt( a11^2 + a21^2 + a12^2 + a22^2)
  normA_Winv = sqrt( pow(A_Winv[0][0],2) + pow(A_Winv[1][0],2) +
                     pow(A_Winv[0][1],2) + pow(A_Winv[1][1],2) );
  normW_Ainv = sqrt( pow(W_Ainv[0][0],2) + pow(W_Ainv[1][0],2) +
                     pow(W_Ainv[0][1],2) + pow(W_Ainv[1][1],2) );

  CN = normA_Winv * normW_Ainv / 2.0;

  return CN;
}
