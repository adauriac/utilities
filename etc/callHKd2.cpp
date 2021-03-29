#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <iostream>
using namespace std;
#include "etc.h"
#include "../alea/hasard.h"

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
#include "/home/dauriac/include/macros.h"

// **********************************************************************
//                            PROTOTYPE DES FUNCTIONS
// **********************************************************************
// **********************************************************************
//                            DEFINITION DES FUNCTIONS
// **********************************************************************

int main(int na,char *para[]) 
{
  int i;
  
  int verbose = 0;
  int L1=16;
  int L2=16;
  int per = 1;
  double x =0.4;
  int seed = 51154;
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
	  _QU(per,"%d"," (valeur de n)");
	  _QU(seed,"%d"," (valeur de n)");
	}
      if (!ok) 
        {
          helper();
          exit(0);
        }
    }
  initR1279(&seed);
  if (verbose) 
    _IMPRIM_PARAM(stdout);
  int V = L1*L2;
  int *site;
  _ALLOUE(site,V,int);
  for(i=0;i<V;i++)
    site[i]=0;
  vector<coord2> s;
  for(i=0;i<V*x;i++)
    {
      coord2 c;
      c.x = R1279()%L1;
      c.y = R1279()%L1;
      s.push_back(c);
      site[_INDEX_SITE_2(c.x,c.y)] = 1;
    }
  int *lab = labelise_alahk(s,L1,L2,per);
  labelise_alahk(site,L1,L2,per);
  for(i=0;i<V;i++)
    if (site[i] != lab[i])
      _STOP("i= %d lab[i]= %d site[i]= %d\n",i,lab[i],site[i]);
  printf("Les deux procedures ont donne le meme resultat : YOUPEE\n");
  for(i=0;i<V;i++)
    printf("%2d%c",lab[i],i%L1==L1-1 ? '\n':' ');

  vector< vector<int> > struct_amas = structamas(lab,V);
  printf("%d amsas\n",(int)struct_amas.size());
  for(i=0;i<(int)struct_amas.size();i++)
  {
    printf("amas %d\n",i);
    vector<int> v = struct_amas[i];
    for (int j=0;j<(int)v.size();j++)
      printf("%d ",v[j]);
    printf("\n");
  }

  if (verbose)
    _PRINT_TIMENOW(stdout);  
  return 0;
}		/* FIN main() */
// *******************************************************************
