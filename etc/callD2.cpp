#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <iostream>
using namespace std;
#include <string>
#include <sstream>

#include <jc.h>
#include <macros.h>

#define _COMMENTS "\
 mettre ici des comentaires appropries\n\
 sur plusieurs lignes eventuelement\n"

void helper(){}
// **********************************************************************
//                        VARIABLES GLOBALES
// **********************************************************************

// **********************************************************************
//                            MACROS
// **********************************************************************

int main(int na,char *para[]) 
{
  int i;
  
  int verbose = 0;
  int L1 = 12;
  int L2 = -1;
  int L3 = -1;
  int L4 = -1;
  _INITQ;
  for (i=1;i<=na;i++)
    {
      char *st,noml[255],ok,j;
     
      st = para[i];
      ok = i==na;
      if (!ok)
        cmdline+=string(st)+" ";
      
      for (j=0;j<2;j++)
	{
	  if (j && !ok) 
	    {
	      printf(_COMMENTS);
	      printf("usage (%s):\n",st);
	    }
	  // METTRE ICI LES DIFFERNTES VALEURS DE LA LIGNE DE COMMANDE
	  _QU(verbose,"%d"," (niveau de blabla)");
	  _QU(L1,"%d"," (valeur de n)");
	  _QU(L2,"%d"," (valeur de n)");
	  _QU(L3,"%d"," (valeur de n)");
	  _QU(L4,"%d"," (valeur de n)");
	}
      if (!ok) 
        {
          helper();
          exit(0);
        }
    }
  if (verbose)
    {
      printf("# %s\n",redo.c_str());
      printf("# %s\n",cmdline.c_str());
      printf("# %s\n",infostring.c_str());
    }
  if (L2<0)
    L2 = -L1*L2;
  if (L3<0)
    L3 = -L1*L3;
  if (L4<0)
    L4 = -L1*L4;
  D2 *myD2 = L4==1 ? new D2(L1,L2,L3,1,0):  new D2(L1,L2,L3,L4,1,0);
  _IMPRIM_PARAM(stdout);
  int m = myD2->m;
  printf("# m= %d\n",m);
  fflush(stdout);

  printf("#! distance_carree  #sites_a_D2_de_O #sites_a<_=D2\n");
  int z = 0;
  for(i=0;i<m;i++)
    {
      int k = myD2->ou(i*i);
      if (k==-1)
        continue;
      z += myD2->N[k];
      printf("%d %d %d\n",i*i,myD2->N[k],z);
      if (verbose)
        cout <<  myD2->comment(i) << endl;
    }
  return 0;
  for(;;)
    {
      char line[256];
      int n;
      if (verbose)
        {
          printf("Entrer d2 ");
          fflush(stdout);
          if (fgets(line,255,stdin)==NULL)
            break;
          sscanf(line,"%d",&n);
        }
      else
        n = 2;
      if (n>L1*L1/4)
        break;
      cout <<  myD2->comment(n) << endl;
      if(!verbose)
        break;
    }

  return 0;
}		/* FIN main() */
// *******************************************************************
