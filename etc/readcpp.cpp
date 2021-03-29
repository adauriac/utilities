#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include "filage.h"

int main(int na,char *para[]) 
{
  char *comments;
  double *A;
  if (na!=2)
    return 0;
  long long int size_in_bytes;
  printf("return value: %d\n",readfromfile(para[1],&comments,(char **)&A,&size_in_bytes));
  printf("%s\n",comments);
  printf("A[0]= %lf A[%lld]= %lf\n",
         A[0],size_in_bytes/sizeof(double)-1,
         A[size_in_bytes/sizeof(double)-1]);
  return 0;
}		/* FIN main() */
/* *********************************************************** */
