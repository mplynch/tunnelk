#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <vector>
  
using namespace std;

// --- Subroutine to read in a mesh file in Anderson Format ---
void readinput(vector<int>      &int_input, 
               vector<double>   &dub_input) 
{
int_input.clear();
dub_input.clear();
  FILE *fp; 
  int line_num = 1;
  int bdim = 100;
  char infile[bdim], buff[bdim], var[bdim];

  int ival;
  double val=999;

//accuracy  1       // Order of accuracy 
int_input.push_back(1);
//viscosity 0       // 1 for viscous, 0 for inviscid
int_input.push_back(0);
//maxInIt   100     // maximum number of inner itterations for flow solver
int_input.push_back(1);
//maxOutIt  200     // maximum number of outer itterations for flow solver
int_input.push_back(50);
//maxOutS   1000    // maximum number of outer itterations for Winslow Smoother
int_input.push_back(1000);
//AdFlag    0       // Adaptation Flag: 0-none, 1-pressure, 2-velocity, 3-mach
int_input.push_back(0);
//DFlag     0       // Derefinement Flag: 1-derefine, 0-don't derefine
int_input.push_back(0);
//SFlag     2       // Smooth Flag: 0-don't smooth, 1-winslow smooth, 2-Complex Winslow
int_input.push_back(2);
//p         2       // Adaptation length parameter
int_input.push_back(2);
//restart   0       // Restart Flag: 0-start from freestream, 1-start from restart file
int_input.push_back(0);
//design    0       // Design Flag: 1 if sensitivity derivatives are to be calculated
int_input.push_back(0);
//mach      0.80    // Mach Number
dub_input.push_back(0.80);
//AoA       1.25    // angle of attack (in degrees) for the flow solver
dub_input.push_back(1.25);
//gamma     1.4     // ratio of specific heats
dub_input.push_back(1.4);
//rho       1.0     // density
dub_input.push_back(1.0);
//mu        1.0     // viscosity
dub_input.push_back(1.0);
//CFLmin    1.0     // minimum CFL number
dub_input.push_back(1.0);
//CFLmax    100.0   // maximum CFL number
dub_input.push_back(100.0);
//iramp     100.0   // number of iterations to ramp CFL from min to max
dub_input.push_back(100.0);
//Rthresh   0.001   // Refinement threshold   (if < 0, Rthresh = Af_mean + C_R*Af_sdev)
dub_input.push_back(0.001);
//Dthresh   0.00001 // Derefinement threshold (if < 0, Dthresh = Af_mean - C_D*Af_sdev)
dub_input.push_back(0.00001);
//alpha     0.0     // Angle of Attack (in degrees) for the smoother (rotation angle)
dub_input.push_back(0.0);

// <tunnelk modification>
#if 0
  sprintf(infile, "input.in");

  //--- Open input file for reading -------------------------------------------80
  if((fp=fopen(infile,"r")) == NULL)
  {
    printf("\nCould not open file: <%s>\n",infile); exit(0);
  }

  printf("\n Reading Input File: <%s>\n",infile);

  //--- Read file -------------------------------------------------------------80
  while(!feof(fp))
  {
    fgets(buff, bdim, fp);
    
    if(line_num>2 && line_num<14)
    {
      sscanf(buff, "%s %d .*",var, &ival);
      //printf("  %-9s = %d\n",var, ival);
      int_input.push_back(ival);
    }
    if(line_num>13 && line_num<25)
    {
      sscanf(buff, "%s %lg .*",var, &val);
      //printf("  %-9s = %f\n",var, val);
      dub_input.push_back(val);
    }
    line_num++;
  }

  fclose(fp);
#endif
// </tunnelk modification>
}
