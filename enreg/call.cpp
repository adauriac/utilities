#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <iostream>
using namespace std;

// **********************************************************************
//                            MACROS
// **********************************************************************
#include "/home/dauriac/include/macros.h"
#include "enreg.h"

int main(int na,char *para[]) 
{
  int i;
  
  char format[] = " i8 f64 i64";
  char filename[] = "data";
  char *pt0;
  double *pt1;
  long long int *pt2;

  enreg my(format,filename,2);
  if (!my.ok)
    {
      printf("%s\n",my.status.c_str());
      _STOP("ok=0\n");
    }
  printf("comments= %s\n",my.comments.c_str());
  
  pt0 = (char *)my.cols[0];
  pt1 = (double *)my.cols[1];
  pt2 = (long long int *)my.cols[2];
  for(i=0;i<my.nline;i++)
    {
      printf("%hhd %lg %lld\n",pt0[i],pt1[i],pt2[i]);
    }
  printf("***********************************************\n");
  printf("# test de fakefile\n");
  char *fakefile = (char*)"#_comment\\n#z=8\\n1_2.3e10_2\\n2_3.2_6\\n\\n\\n3_7_8\\n#inter\\n8_0.1_12\\n_12_1e-7_-666666666\\n";
  printf("traite la fakefile %s\n",fakefile);
  enreg my2(format,fakefile,1,1);
  if (!my2.ok)
    {
      printf("%s\n",my2.status.c_str());
      _STOP("ok=0\n");
    }
  printf("comments= %s\n",my2.comments.c_str());
  pt0 = (char *)my2.cols[0];
  pt1 = (double *)my2.cols[1];
  pt2 = (long long int *)my2.cols[2];
  for(i=0;i<my2.nline;i++)
    {
      printf("%hhd %lg %lld\n",pt0[i],pt1[i],pt2[i]);
    }
  return 0;
}		/* FIN main() */
// *******************************************************************
