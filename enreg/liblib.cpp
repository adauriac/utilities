#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <iostream>
using namespace std;


// **********************************************************************
//                            MACROS
// **********************************************************************
#include "/home/dauriac/include/macros.h"

class enreg
{
public:
  int nlines;
  int ncols;
  double **cols;
  char *comments;

  enreg(FILE *ft);
  enreg(char *string,int nompascontenu=1);
  ~enreg(){};
  int llintInComments(char *target,long long int *val);// retourne 0,1 ou 2 suivant pas de target, exactement 1, plus de 1
  int doubleInComments(char *target,double *val);// retourne 0,1 ou 2 suivant pas de target, exactement 1, plus de 1
};

enreg::enreg(FILE *ft)
{
  int commentslength = 0;
  int commentsreserved = 0;
  int linesreserved = 0;
#define _LINE_MAX 1024
#define _BLOCK_SIZE (10*_LINE_MAX)
#define _NBFIELDS_MAX 50
  comments = NULL;
  nlines = 0;
  ncols = 0;
  for(;;)
    {
      char line[_LINE_MAX];
      if (fgets(line,1024,ft)==NULL) // ATTENTION SI LINE >1024 !!!
        break;
      if (line[0]=='#')
        {
          if (commentslength==commentsreserved)
            {
              commentsreserved+= _BLOCK_SIZE;
              _REALLOUE(comments,commentsreserved,char);
            }
          strcat(comments,line);
          continue;
        }
      // lectrue d'une data
      double values[_NBFIELDS_MAX];
      char *nptr,*endptr;
      int i;
      for(nptr=line,i=0;i<_NBFIELDS_MAX;i++)
        {
          values[i] = strtod(nptr,&endptr);
          if (endptr==nptr)
            break;
          nptr = endptr;
        }
      if (i==_NBFIELDS_MAX)
        _STOP("Trop de colonne (>%d) editer et recompiler\n",_NBFIELDS_MAX);
      if (ncols)
        {
          // est-ce conforme a la reference ?
          if (i!=ncols)
            _STOP("Nombre de colonne non constante %d au lieu de %d\n",i,ncols);
        }
      else
        {
          // donc premiere fois que data ce sera la reference
          ncols = i;
          _ALLOUE(cols,ncols,double *);
          linesreserved = _BLOCK_SIZE;
          for(i=0;i<ncols;i++)
            {
              _ALLOUE(cols[i], linesreserved,double);
            }
        }
      if (nlines == linesreserved)
        {
          linesreserved = _BLOCK_SIZE;
          for(i=0;i<ncols;i++)
            {
              _ALLOUE(cols[i], linesreserved,double);
            }
        }
      for(i=0;i<ncols;i++)
        {
          cols[i][nlines] = values[i];
          //printf("%.16lg ",values[i]);
        }
      //printf("\n");
      nlines ++;
    }
}     // FIN enreg::enreg(FILE *ft);
// **********************************************************************

int enreg::llintInComments(char *target,long long int *val)
// la PREMIERE valeur de target est mise dans val et le nombre de valeur trouvee est retourne
{
  char *pt = strpbrk(comments,target);
  if (pt==NULL)
    return 0;
  // recherche du egal
  pt += strlen(target);
  for (;;)
    {
      if (*pt==0)
        return 0;
      if (*pt=='=')
        break;
      if (*pt != ' ')
        return 0;
      pt++;
    }
  pt ++; // pour skipper le =
  int i = sscanf(pt,"%lld",val);
  if (i!=1)
    return 0;
  pt = strpbrk(pt+1,target);
  if (pt==NULL)
    return 1;
  // il y en a au moins 2
  return 2;
}     // FIN int enreg::intInComments(char target,long long int *val)
// **********************************************************************

int enreg::doubleInComments(char *target,double *val)
// la PREMIERE valeur de target est mise dans val et le nombre de valeur trouvee est retourne
{
  char *pt = strpbrk(comments,target);
  if (pt==NULL)
    return 0;
  // recherche du egal
  pt += strlen(target);
  for (;;)
    {
      if (*pt==0)
        return 0;
      if (*pt=='=')
        break;
      if (*pt != ' ')
        return 0;
      pt++;
    }
  pt ++; // pour skipper le =
  int i = sscanf(pt,"%lf",val);
  if (i!=1)
    return 0;
  pt = strpbrk(pt+1,target);
  if (pt==NULL)
    return 1;
  // il y en a au moins 2
  return 2;
}     // FIN int enreg::doubleInComments(char target,double *val)
// **********************************************************************

int main()
{
  FILE *ft = fopen("exa","r");
  enreg my(ft);
  printf("%s",my.comments);
  printf("ncols= %d nlines= %d\n",my.ncols,my.nlines);
  int i;
  for(i=0;i<my.nlines;i++)
    {
      int j;
      for(j=0;j<my.ncols;j++)
        printf("%.16lg ",my.cols[j][i]);
      printf("\n");
    }
  long long int lli;
  i = my.llintInComments((char *)"L",&lli);
  printf("i= %d val= %lld\n",i,lli);
  double x;
  i = my.doubleInComments((char *)"x",&x);
  printf("i= %d val= %.16lg\n",i,x);
}
