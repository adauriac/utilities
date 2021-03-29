#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "filage.h"

// **********************************************************************
//                            PROTOTYPE DES FUNCTIONS
// **********************************************************************
static int get_value_from_string(char *string,char *name,void *r,int *LL);
static char *can(char *st);

// **********************************************************************
//                            DEFINITION DES FUNCTIONS
// **********************************************************************
int readfromfile_(char *filename,char **comments,char **buf,long long int *sizebuf)
{
  return readfromfile(filename,comments,buf,sizebuf);
}     // FIN readfromfile_()
// *************************************************************

int readcommentsfromfile_(char *filename,char **comments)
{
  return readcommentsfromfile(filename,comments);
}     // FIN readfromfile_()
// *************************************************************

int writeinfile_(char *filename,char *comments,char *buf,long long int *sizebuf)
{
  return writeinfile(filename,comments,buf,sizebuf);
}     // FIN readfromfile_()
// *************************************************************

int isbinaryfile(char *filename)
// return 1 only if the bytes 8, ...,15  represents the magic number
{
  FILE *ft = fopen(filename,"r");
  if (ft == NULL)
    return 0;
  long long int tag=0;
  if (fread(&tag,sizeof(long long int),1, ft) != 1)
    return 0;
  if (fread(&tag,sizeof(long long int),1, ft) != 1)
    return 0;
  return (tag == _TAG);
}     // FIN int isbinaryfile(char *filename)
// *************************************************************

int readfromfile(char *filename,char **comments,char **buf,long long int *sizebuf)
{
  struct stat descfile;
  if (stat(filename,&descfile))
    return _CANT_STATE_FILE;

  FILE *ft = fopen(filename,"r");
  if (ft == NULL)
    return _CANT_OPEN_FILE;
  long long int totalsize = descfile.st_size; // corrige 30 avril 2014, 16:44:17
 
  long long int n;
  if (fread(&n,sizeof(long long int),1, ft) != 1)
    return _CANT_READ_SIZE_COMMENTS;
  n -= 8;  // since the tag is not written in the string

  long long int tag;
  if (fread(&tag,sizeof(long long int),1, ft) != 1)
    return _CANT_READ_TAG;
  if (tag != _TAG)
    return _INVALID_TAG;

  char *com = (char *)malloc(n*sizeof(char)+1);
  com[n*sizeof(char)] = 0; // end of string
  *comments = com;
  if (com == NULL)
    return _CANT_ALLOCATE_COMM;
  if ((int)fread(com,sizeof(char),n, ft) != n)
    return _CANT_READ_COMMENTS;

  long long int n2read = totalsize - (8 + 8 + n);
  *sizebuf = n2read;
  char *b = (char *)malloc(n2read*sizeof(char));
  if (b == NULL)
    return _CANT_ALLOCATE_BUF;
  *buf = b;

  if ((long long int) fread(b,sizeof(char),n2read, ft) != n2read)
    return _CANT_READ_DATA;

  fclose(ft);
  return _RETURN_OK;
}      // FIN reeadfromfile()
// *********************************************************************

int readcommentsfromfile(char *filename,char **comments)
{
  struct stat descfile;
  if (stat(filename,&descfile))
    return _CANT_STATE_FILE;

  FILE *ft = fopen(filename,"r");
  if (ft == NULL)
    return _CANT_OPEN_FILE;
 
  long long int n;
  if (fread(&n,sizeof(long long int),1, ft) != 1)
    return _CANT_READ_SIZE_COMMENTS;
  n -= 8;  // since the tag is not written in the string

  long long int tag;
  if (fread(&tag,sizeof(long long int),1, ft) != 1)
    return _CANT_READ_TAG;
  if (tag != _TAG)
    return _INVALID_TAG;

  char *com = (char *)malloc(n*sizeof(char)+1);
  com[n*sizeof(char)] = 0; // end of string
  *comments = com;
  if (com == NULL)
    return _CANT_ALLOCATE_COMM;
  if ((int)fread(com,sizeof(char),n, ft) != n)
    return _CANT_READ_COMMENTS;

  fclose(ft);
  return _RETURN_OK;
}      // FIN reeadcommentsfromfile()
// *********************************************************************

int writeinfile(char *filename,char *comments,char *buf,long long int *size_in_bytes)
{
  long long int size = *size_in_bytes;
  FILE *ft = fopen(filename,"w");
  if (ft == NULL)
    return _CANT_OPEN_FILE;

  long long int size_comm= strlen(comments)+sizeof(long long int);  // tag on 8 bytes + comments
  if (fwrite(&size_comm,sizeof(long long int),1,ft) != 1)
    return _CANT_WRITE_SIZE_COMMENTS;

  long long int tag = _TAG;
  if (fwrite(&tag,sizeof(long long int),1,ft) != 1)
    return _CANT_WRITE_TAG;

  int i = strlen(comments);
  if ((int)fwrite(comments,sizeof(char),i,ft) != i)
    return _CANT_WRITE_COMMENTS;

  if ((int)fwrite(buf,sizeof(char),size,ft) != size)
    return _CANT_WRITE_DATA;

  fclose(ft);

  return _RETURN_OK;
}      // FIN writeinfile()
// *********************************************************************

int writeinfile_withoutclosing(char *filename,char *comments,char *buf,long long int *size_in_bytes,FILE **f)
{
  long long int size = *size_in_bytes;
  FILE *ft = fopen(filename,"w");
  *f = ft;
  if (ft == NULL)
    return _CANT_OPEN_FILE;

  long long int size_comm= strlen(comments)+sizeof(long long int);  // tag on 8 bytes + comments
  if (fwrite(&size_comm,sizeof(long long int),1,ft) != 1)
    return _CANT_WRITE_SIZE_COMMENTS;

  long long int tag = _TAG;
  if (fwrite(&tag,sizeof(long long int),1,ft) != 1)
    return _CANT_WRITE_TAG;

  int i = strlen(comments);
  if ((int)fwrite(comments,sizeof(char),i,ft) != i)
    return _CANT_WRITE_COMMENTS;

  if ((int)fwrite(buf,sizeof(char),size,ft) != size)
    return _CANT_WRITE_DATA;

  return _RETURN_OK;
}      // FIN writeinfile_withoutclosing()
// *********************************************************************

static char *can(char *st)
//  replace the sign =  by " = "
{
  int i,j=0;
  int n = strlen(st);
  char *cano = (char *)malloc(2*n);
  if (cano==NULL)
    return cano;
  // replace the sign =  by " = "
  for(i=0;i<n;i++)
    {
      if (st[i] != '=')
	cano[j++] = st[i];
      else
	{
	  cano[j++] = ' ';
	  cano[j++] = st[i];
	  cano[j++] = ' ';
	}
    }
  return cano;
}     // FIN can()
// ****************************************************************

static int get_value_from_string(char *string,char *name,void *r,int *LL)
{
  // canononise
  char *canonise = can(string);
  
  char **Para;
  int Na = 0;
  Para = (char **)malloc(sizeof(char *)*strlen(canonise)/2); // at most that many 
  char *pt = strtok (canonise," \t");
  Para[Na++] = pt;
  for(;;)
    {
      if (pt==NULL)
	break;
      pt = strtok (NULL," \t");
      Para[Na++] = pt;
     }
  Na--;

  int i;

  for(i=0;i<Na-2;i++)
    {
      if (strncmp(Para[i],name,strlen(name))==0)
	{
	  if (strncmp(Para[i+1],"=",1)==0)
	    {
	      if (*LL==1)
		i = sscanf(Para[i+2],"%lld",(long long int *)r);
	      else if (*LL==0)
		i = sscanf(Para[i+2],"%lf",(double *)r);
	      else if (*LL==2)
                {
                  char *aux = (char*)malloc(strlen(Para[i+2])*sizeof(char));
                  if (aux==NULL)
                    return 1; // pour dire une erreur 
                  strcpy(aux,Para[i+2]);
                  *((char **)r)  = aux;
                  i = 1;
                }
	      return i != 1;
	    }
	}
    }
  free(Para);
  free(canonise);
  return 1;  
}     //  FIN get_value_from_string()
// *********************************************************************

int get_string_from_string(char *string,char *name,char **r)
{
  int i = 2;
  return get_value_from_string(string,name,(void *)r,&i);
}     //  FIN get_LL_froms_tring()
// *********************************************************************

int get_LL_from_string(char *string,char *name,long long int *r)
{
  int i = 1;
  return get_value_from_string(string,name,(void *)r,&i);
}     //  FIN get_LL_froms_tring()
// *********************************************************************

int get_double_from_string(char *string,char *name,double *r)
{
  int i = 0;
  return get_value_from_string(string,name,(void *)r,&i); 
}      // FIN getdoublefromstring()
// *********************************************************************

int appendtofile(char *filename,char *buf,long long int *size_in_bytes)
{
  long long int size = *size_in_bytes;
  FILE *ft = fopen(filename,"a");
  if (ft == NULL)
    return _CANT_OPEN_FILE;

  if ((int)fwrite(buf,sizeof(char),size,ft) != size)
    return _CANT_WRITE_DATA;

  fclose(ft);

  return _RETURN_OK;

}     // FIN appendtofile()
// *********************************************************************

