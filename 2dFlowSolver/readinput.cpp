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

  sprintf(infile, "design.in");

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
}
