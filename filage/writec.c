#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include "filage.h"

int main(int na,char *para[]) 
{
  double A[3]={1.1,2.2,3.3};
  char filename[256] = "essai.dat";
  char comments[256] = "L= 6\n u = 0.34\nv =0.76\n";
  long long int size_in_bytes = sizeof(A);
  printf("%d\n",writeinfile(filename,comments,(char *)A,&size_in_bytes));

  return 0;
}		/* FIN main() */
/* *********************************************************** */
