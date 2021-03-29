#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <iostream>
using namespace std;

#include <filage.h>

#define _COMMENTS "extrait les commentaires de fichiers binaires a la JC donnes sur la ligne de commande (syntaxe NON jc)\n"

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
  
#if 0
  int verbose = 0;
  char f[1024] = "";
  for (i=1;i<na;i++)
    {
      char *st,noml[255],ok,j;
     
      st = para[i];
      ok = 0;
      
      for (j=0;j<2;j++)
	{
	  if (j && !ok) 
	    {
	      printf(_COMMENTS);
	      printf("usage (%s):\n",st);
	    }
	  // METTRE ICI LES DIFFERNTES VALEURS DE LA LIGNE DE COMMANDE
	  _Q(verbose,"%d"," (niveau de blabla)");
	  _QS(f,"%s"," nom du fichier dont on veut extaire les commentaires");
	}
      if (!ok) 
        {
          helper();
          exit(0);
        }
    }
  if (verbose)
    {
      printf("# ");
      for (i=0;i<na;i++) printf("%s ",para[i]);
      printf("\n");
    }
#endif
  for (i=1;i<na;i++)
    {
      char *comments;
      int r = readcommentsfromfile(para[i],&comments);
      if (r)
        _STOP("%s\nreadfromfile returned %d for %s\n",_COMMENTS,r,para[i]);
      if (na>2)
        printf("# FILE= %s\n",para[i]);
      printf("%s\n",comments);
    }
  return 0;
}		/* FIN main() */
// *******************************************************************
