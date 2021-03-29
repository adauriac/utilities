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

  enreg my(format,filename,2);
  if (!my.ok)
    {
      printf("%s\n",my.status.c_str());
      _STOP("ok=0\n");
    }
  printf("comments= %s\n",my.comments.c_str());

  char *pt0 = (char *)my.cols[0];
  double *pt1 = (double *)my.cols[1];
  long long int *pt2 = (long long int *)my.cols[2];
  for(i=0;i<my.nline;i++)
    {
      printf("%hhd %lg %lld\n",pt0[i],pt1[i],pt2[i]);
    }
  i = my.nline/2;
  printf("\nreallocation %d -> %d\n",my.nline,i);
  my.realloue(i);
  for(i=0;i<my.nline;i++)
    {
      printf("%hhd %lg %lld\n",pt0[i],pt1[i],pt2[i]);
    }
  
  return 0;
}		/* FIN main() */
// *******************************************************************
