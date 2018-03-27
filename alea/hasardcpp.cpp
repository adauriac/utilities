#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <iostream>
using namespace std;
#include "hasardcpp.h"

rndclass::rndclass(unsigned int seed,int prestep)
{
  reset(seed,prestep);
}     // FIN rndclass::rndclass(unsigned int seedp,int prestep)
// ******************************************************************

void rndclass::reset(unsigned int seedp,int prestep)
{
  if (seedp==0)
    {
      FILE *ft= fopen((char*)"/dev/random",(char*)"r");
      if (ft==NULL)
        {
          fprintf(stderr,"Can't open /dev/random\n");
          exit(0);
        }
      int s1,s2;
      if (fgets((char *)(&s1),sizeof(int),ft)==NULL)
        {
          fprintf(stderr,"pas pu lire s1\n");
          exit(0);
        }
      if (fgets((char *)(&s2),sizeof(int),ft)==NULL)
        {
          fprintf(stderr,"pas pu lire s2\n");
          exit(0);
        }

      seed = (s1<<16) + (s2&65535);
    }
  else
    seed = seedp;
  VarCgl=seed;
  for (int i=0;i<1279;i++) 
    {
      unsigned int k1=cgl();
      k1=k1<<8;
      k1=k1&0xffff0000; /* shift to skip the 8 msb */
      unsigned int k2=cgl();
      k2=k2>>8;
      k2=k2&0x0000ffff; /* shift to skip the 8 lsb */
      tabR1279[i] = k1|k2;
    }
  Var1R1279 = 0;
  Var2R1279 = 1279-1063;
  cpt = 0;
  for(int i=0;i<prestep;i++)
    R1279();
}     // FIN void rndclass::reset()
// ********************************************************************

unsigned int rndclass::cgl()
{
  unsigned int q,r,Q,R;
  q=16807*(VarCgl/0x10000);
  r=16807*(VarCgl%0x10000);
  Q=q/0x8000;
  R=q%0x8000;
  R=R*0x10000 + r;
  if (R>=0x80000000) 
    {
      Q++;
      R-=0x80000000;
    }
  R=Q+R;
  if (R>=0x7fffffff) 
    {
      Q++;
      R-=0x7fffffff;
    }
  VarCgl=R;
  return R;
}     // FIN rndclass::unsigned int cgl()
// ********************************************************************

unsigned int rndclass::R1279()
{
  unsigned int x;
  x = tabR1279[Var1R1279]^tabR1279[Var2R1279];
  tabR1279[Var1R1279] = x;
  if (++Var1R1279==1279) 
    Var1R1279 = 0;
  if (++Var2R1279==1279) 
    Var2R1279 = 0;
  cpt++;
  return x;
}     // FIN unsigned int rndclass::R1279()
/* ************************************************************************** */
