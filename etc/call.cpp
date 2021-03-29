#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <iostream>
using namespace std;

#include <etc.h>
//#include <libetc.cpp>

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
  int n = 12;
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
	  _QU(n,"%d"," (valeur de n)");
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
  #define _N 16
  int iT[_N];
  long long int lliT[_N];
  double dT[_N];
  for (i=0;i<_N;i++)
    {
      iT[i] = i;
      lliT[i] = 4294967296+100+i;
      dT[i] =i*10.;
    }
  printf("i 23 : %d\n",dicho_search(iT,_N,23));
  printf("i 2  :%d\n",dicho_search(iT,_N,2));
  printf("i locate -6   :%d\n",locate(iT,_N,-6));
  printf("i locate 60   :%d\n",locate(iT,_N,60));
  printf("i locate 60   :%d\n",locate(dT,_N,64.5));
  printf("lli[6]  :%d\n",dicho_search(lliT,_N,lliT[6]));

  string str("Split me by whitespaces");
  printf("strinf initiale %s\n",str.c_str());
  vector<string> tokens = split_sep32(str);
  n = tokens.size();
  printf("%d tokens\n",n);
  for(int i=0;i<n;i++)
    printf("%s\n",tokens[i].c_str());
  return 0;
}		/* FIN main() */
// *******************************************************************
