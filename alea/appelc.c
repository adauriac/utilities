#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include "hasard.h"

#define _COMMENTS "\
 mettre ici des comentaires appropries\n\
 sur plusieurs lignes eventuelement\n"
// **********************************************************************
//                        VARIABLES GLOBALES
// **********************************************************************
static int verbose;

// **********************************************************************
//                            MACROS
// **********************************************************************
#define _ALLOUE(nom,size,type){\
	nom = (type *)malloc((size)*sizeof(type));\
	if (nom==0) {printf("Not enough memory for %s\n",#nom);exit(0);}}
/* _ALLOUE(T1,n*n,double) */

#define _REALLOUE(nom,size,type){\
        nom = (type *)realloc((void *)nom,(size)*sizeof(type));\
        if (nom==0) {printf("Can't realloc %s\n",#nom);exit(0);}}
/* _REALLOUE(T1,n*n,double) */

#define _OUVREFILE(nom,ft,mode) {\
	ft = fopen(nom,mode);\
	if (ft==0) {printf("Can't open %s",nom);exit(0);}}

#define _Q(nom,format,comment)\
if (j)\
  {\
    if (!ok){sprintf(noml,"%s=%s%s\n",#nom,format,comment);printf(noml,nom);}\
  }\
else\
  {\
     strcpy(noml,#nom);strcat(noml,"=");\
     if (strncmp(st,noml,strlen(noml))==0) \
         {ok = (sscanf(&st[strlen(noml)],format,&nom)==1);}\
  }
// **********************************************************************
//                            PROTOTYPE DES FUNCTIONS
// **********************************************************************

// **********************************************************************
//                            DEFINITION DES FUNCTIONS
// **********************************************************************
void Stop(char *st){printf("%s",st);exit(0);}    //FIN Stop()
// ******************************************************************

int main(int na,char *para[]) 
{
  int i,n;
  
  verbose=0;
  n=12;
  int seed = 151415;
  int seed1 = 10602988;
  int seed2 = 14186;
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
	  _Q(n,"%d"," (valeur de n)");
	  _Q(seed,"%d"," (valeur de n)");
	  _Q(seed1,"%d"," pour le deuxieme appel");
	  _Q(seed2,"%d"," pour le deuxieme appel");
	}
      if (!ok) exit(0);
      
    }
  if (verbose)
    {
      printf("# ");
      for (i=0;i<na;i++) printf("%s ",para[i]);
      printf("\n");
    }
  initR1279(&seed);
  printf("# seed utilisee= %d\n",seed);
  for(i=0;i<10;i++)
    printf("%.10lf %.10lf\n",Gauss(),U1279());
  printf("\n amorceur double\n");
  initR1279_2(&seed2,&seed1);
  unsigned int *pt = tabR1279Addresse();
  for(int bidon=0;bidon<10;bidon++)
    printf("%d %d\n",bidon,pt[bidon]);

  return 0;
}		/* FIN main() */
/* *********************************************************** */
