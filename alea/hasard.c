/* INCLUDED: randjc() cgl() R250() R1279() VERSION FONCTION
(JC Angles d'Auriac Decembre 1993, Voir rapport de convention pour les details)
use /dev/random if seed=0 or seed=-1, exit in case of panic
can be called from fotran
*/
#include "hasard.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// modifie par separation .c .h le  mar oct  8 11:08:57 CEST 2002  
static unsigned int VarCgl;			/* 0<= Random <=0x7fffffff */
static unsigned int Var1R250,Var2R250;		/* 0<= Random <=0xffffffff */
static unsigned int Var1R1279,Var2R1279;	/* 0<= Random <=0xffffffff */
static unsigned int tabR250[250];
unsigned int tabR1279[1279]; /* static enlever pour amorcer plus large */

/* ************************************************************************** */

unsigned int *tabR1279Addresse() {return tabR1279;}

unsigned int cgl()
{
unsigned int q,r,Q,R;

q=16807*(VarCgl/0x10000);
r=16807*(VarCgl%0x10000);
Q=q/0x8000;
R=q%0x8000;
R=R*0x10000 + r;
if (R>=0x80000000) {Q++;R-=0x80000000;}
R=Q+R;
if (R>=0x7fffffff) {Q++;R-=0x7fffffff;}
VarCgl=R;
return(R);
}			/* Fin cgl0() */
void initcgl(unsigned int x){VarCgl=x;}
/* ************************************************************************** */

unsigned int R250()
{
unsigned int x;

x=tabR250[Var1R250]^tabR250[Var2R250];
tabR250[Var1R250]=x;
if (++Var1R250==250) Var1R250=0;
if (++Var2R250==250) Var2R250=0;
return(x);
}		/* R250() */
/* ************************************************************************** */

void initR250(int *seedarg)
/* Use cgl() to set the intial vector. But the 8 lsb and the
8 msb of cgl() being not very good, they are discarded,
and two cgl() per entry are used */
{
  unsigned int k1,k2;
  unsigned int seed = *seedarg;

  if ((seed ==(unsigned int)-1) || (seed == 0))
    {
      FILE *ft = fopen("/dev/urandom","r");
     if (ft==NULL)
       {
	 fprintf(stderr,"Seed = %d and /dev/urandom can't be opened\n",seed);
	 exit(0);
       }
     if (fgets((char *)(&seed),sizeof(int),ft)==NULL)
       {
	 fprintf(stderr,"Could not read in /dev/urandom");
	 exit(0);
       }
     fclose(ft);
    }
  *seedarg = seed;
  Var2R250=VarCgl;VarCgl=seed; /* Save the current value of VarCgl */
  for (Var1R250=0;Var1R250<250;Var1R250++) 
    {
      k1=cgl();k1=k1<<8;k1=k1&0xffff0000; /* shift to skip the 8 msb */
      k2=cgl();k2=k2>>8;k2=k2&0x0000ffff; /* shift to skip the 8 lsb */
      tabR250[Var1R250]=k1|k2;
    }
  VarCgl=Var2R250;             /* Restore the current value of VarCgl */
  Var1R250=0;Var2R250=250-103;
}		/* FIN initR250() */
/* ************************************************************************** */

void initR1279(int *seedarg)
/* Use cgl() to set the intial vector. But the lsb of cgl() being not
very good, they are discarded and two cgl() per entry are used */
{
  unsigned int k1,k2;
  unsigned int seed = *seedarg;
  
  if ((seed ==(unsigned int)-1) || (seed == 0))
    {
      FILE *ft = fopen("/dev/urandom","r");
     if (ft==NULL)
       {
	 fprintf(stderr,"Seed = %d and /dev/urandom can't be opened\n",seed);
	 exit(0);
       }
     if (fgets((char *)(&seed),sizeof(int),ft)==NULL)
       {
	 fprintf(stderr,"Could not read in /dev/urandom");
	 exit(0);
       }
     fclose(ft);
    }

  *seedarg = seed; 
  Var2R1279=VarCgl;VarCgl=seed; /* Save the current value of VarCgl */
  for (Var1R1279=0;Var1R1279<1279;Var1R1279++) 
    {
      k1=cgl();k1=k1<<8;k1=k1&0xffff0000; /* shift to skip the 8 msb */
      k2=cgl();k2=k2>>8;k2=k2&0x0000ffff; /* shift to skip the 8 lsb */
      tabR1279[Var1R1279]=k1|k2;
    }
  VarCgl=Var2R1279;             /* Restore the current value of VarCgl */
  Var1R1279=0;Var2R1279=1279-1063;
}		/* FIN initR1279() */
/* ************************************************************************** */

void initR1279_2(int *seedarg1,int *seedarg2)
/* Use cgl() to set the intial vector. But the lsb of cgl() being not
very good, they are discarded and two cgl() per entry are used */
{
  unsigned int k1,k2;
  unsigned int seed1 = *seedarg1;
  unsigned int seed2 = *seedarg2;
  
  if ((seed1 ==(unsigned int)-1) || (seed1 == 0))
    {
      FILE *ft = fopen("/dev/urandom","r");
     if (ft==NULL)
       {
	 fprintf(stderr,"Seed = %d and /dev/urandom can't be opened\n",seed1);
	 exit(0);
       }
     if (fgets((char *)(&seed1),sizeof(int),ft)==NULL)
       {
	 fprintf(stderr,"Could not read in /dev/urandom");
	 exit(0);
       }
     fclose(ft);
    }
  if ((seed2 ==(unsigned int)-1) || (seed2 == 0))
    {
      FILE *ft = fopen("/dev/urandom","r");
     if (ft==NULL)
       {
	 fprintf(stderr,"Seed = %d and /dev/urandom can't be opened\n",seed2);
	 exit(0);
       }
     if (fgets((char *)(&seed2),sizeof(int),ft)==NULL)
       {
	 fprintf(stderr,"Could not read in /dev/urandom");
	 exit(0);
       }
     fclose(ft);
    }

  *seedarg1 = seed1; 
  Var2R1279=VarCgl;VarCgl=seed1; /* Save the current value of VarCgl */
  for (Var1R1279=0;Var1R1279<1279;Var1R1279++) 
    {
      k1=cgl();k1=k1<<8;k1=k1&0xffff0000; /* shift to skip the 8 msb */
      k2=cgl();k2=k2>>8;k2=k2&0x0000ffff; /* shift to skip the 8 lsb */
      tabR1279[Var1R1279]=k1|k2;
    }
  *seedarg2 = seed2; 
  Var2R1279=VarCgl;VarCgl=seed2; /* Save the current value of VarCgl */
  for (Var1R1279=1;Var1R1279<1279;Var1R1279++) 
    {
      k1=cgl();k1=k1<<8;k1=k1&0xffff0000; /* shift to skip the 8 msb */
      k2=cgl();k2=k2>>8;k2=k2&0x0000ffff; /* shift to skip the 8 lsb */
      tabR1279[Var1R1279] += k1|k2;
    }
  VarCgl=Var2R1279;             /* Restore the current value of VarCgl */
  Var1R1279=0;Var2R1279=1279-1063;
}		/* FIN initR1279_2() */
/* ************************************************************************** */

unsigned int R1279()
{
  unsigned int x;
  x=tabR1279[Var1R1279]^tabR1279[Var2R1279];
  tabR1279[Var1R1279]=x;
  if (++Var1R1279==1279) 
    Var1R1279=0;
  if (++Var2R1279==1279) 
    Var2R1279=0;
  return(x);
}		/* FIN R1279() */
/* ************************************************************************** */
double U1279()
{
  return R1279()/(double) (unsigned int )(-1);
}     // U1279()
// **************************************************************

double Gauss()
{
  double x,y,s;
  static double no = (double)((unsigned int)(-1));

  for (;;)
    {
      x=((double)R1279())/no;
      y=((double)R1279())/no;

      s=x*x+y*y;
      if (s<=1)
        break;
   }
  s=sqrt(-2*log(s)/s);
  if (R1279() & 0x800)
    s = -s;
  return x*s;
}                 /* FIN Gauss() */
/* ******************************************************************** */

int equivR1279(int x)
{
  return (x>0 ? x+2147483647 : x-2147483647);
}     // FIN int equiv(int x)
// **********************************************************************

void initr1279_(int *seed)
{
  initR1279(seed);
}     // FIN InitR1279_()
// ***********************************************************************

unsigned int r1279_()
{
  return R1279();
}     // FIN InitR1279_()
// ***********************************************************************

double gauss_()
{
  return Gauss();
}     // FIN Gauss()
// ********************************************************************

double uniform_()
{
  return U1279();
}     // U1279()
// **************************************************************

unsigned int uniform1279(unsigned int borneSup)
{
  unsigned int m = (unsigned int)(-1)-(unsigned int)(-1)%borneSup;
  while (1)
    {
      unsigned int R = R1279();
      if (R<m)
        return R%borneSup;
      //printf("retire\n"); servait a verifier nb retirages
    }
}     // FIN unsigned int uniform1279(unsigned int borneSup)
// **************************************************************

void shuffleR1279(unsigned int *a,unsigned int n)
// entree a est un tableau de n entiers
// en sortie a[i] est l'image de i par la permutation
{
  unsigned int i;
  for(i=0;i<n;i++)
    a[i] = i;
  for(i=0;i<n-1;i++)
    {
      unsigned int j = uniform1279(n-i);
      unsigned int aux = a[i];
      a[i] = a[i+j];
      a[i+j] = aux;
    }
}     // FIN void shuffle(unsigned int *a,unsigned int n)
// **************************************************************************
