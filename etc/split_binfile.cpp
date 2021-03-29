#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <iostream>
using namespace std;

#define _COMMENTS "split asci file (comments) and bin file (data) \n"

void helper(){}
// **********************************************************************
//                        VARIABLES GLOBALES
// **********************************************************************

// **********************************************************************
//                            MACROS
// **********************************************************************
#include <macros.h>
#include <filage.h>

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
  if (na!=2)
    _STOP("%s",_COMMENTS);

  // for (i=1;i<na;i++)
  //   {
  //     char *st,noml[255],ok,j;
     
  //     st = para[i];
  //     ok = 0;
      
  //     for (j=0;j<2;j++)
  //       {
  //         if (j && !ok) 
  //           {
  //             printf(_COMMENTS);
  //             printf("usage (%s):\n",st);
  //           }
  //         // METTRE ICI LES DIFFERNTES VALEURS DE LA LIGNE DE COMMANDE
  //         _Q(verbose,"%d"," (niveau de blabla)");
  //       }
  //     if (!ok) 
  //       {
  //         helper();
  //         exit(0);
  //       }
  //   }
  // if (verbose)
  //   {
  //     printf("# ");
  //     for (i=0;i<na;i++) printf("%s ",para[i]);
  //     printf("\n");
  //   }

  char *comments,*buf;
  long long int sizebuf_in_bytes;

  i = readfromfile(para[1],&comments,&buf,&sizebuf_in_bytes);
  if (i)
    _STOP("erreur %d readfromfile\n",i);
  printf("%s\n",comments);
  char newname[1024];
  sprintf(newname,"%s.data",para[1]);
  FILE *ft=fopen(newname,"r");
  if (ft!=NULL)
    _STOP("%s existe deja\n",newname);
  ft=fopen(newname,"w");
  if (ft==NULL)
    _STOP("could not open %s\n",newname);

  i = fwrite(buf,sizeof(char),sizebuf_in_bytes,ft);
  fclose(ft);
  if (i!=sizebuf_in_bytes)
    fprintf(stderr,"ATTENTION only %d bytes written out of %lld\n",
            i,sizebuf_in_bytes);

  fprintf(stderr,"%s closed\n",newname);
  return 0;
}		/* FIN main() */
// *******************************************************************
