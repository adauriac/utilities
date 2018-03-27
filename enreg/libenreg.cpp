#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <iostream>
using namespace std;

#include <macros.h>
#include "enreg.h"
#include <vector>


// format : i8,i16,i32,i64,f32,f64
// i/f ==> int/float, 8,16,32,64 nombre de bits
// filename = nom de la file a lire
// n numero de l'enregistrement a considerer un enregistement est
// fini losrque l'on rencontre deux lines vide de suite. 
// les commentaire inclus TOUS les commentaires precedents
#define _FINI(statusP) {                        \
    status += statusP;                          \
    filename= NULL;                             \
    nline = 0;                                  \
    ncol = 0;                                   \
    ok = 0;                                     \
    cols = NULL;                                \
    return;                                     \
  }

enreg::enreg(char *format,char *filename,int n0,int fakeFile)
{
  // analyse du format
  int i,n = strlen(format);
  ncol=0;
  status = "";
  ok = 1;
  for(i=0;i<n;i++)
    {
      if (format[i]==' ')
        continue;
      if (format[i]=='i')
        {
          if (format[i+1]=='8')
            type[ncol++] = "i8";
          else if ((format[i+1]=='1')&&(format[i+2]=='6'))
            type[ncol++] = "i16";
          else if ((format[i+1]=='3')&&(format[i+2]=='2'))
            type[ncol++] = "i32";
          else if ((format[i+1]=='6')&&(format[i+2]=='4'))
            type[ncol++] = "i64";
          else
            _FINI("erreur de format sur i");
        }
      else if (format[i]=='f')
        {
          if ((format[i+1]=='3')&&(format[i+2]=='2'))
            type[ncol++] = "f32";
          else if ((format[i+1]=='6')&&(format[i+2]=='4'))
            type[ncol++] = "f64";
          else
            _FINI("erreur de format sur f");
        }
      else
        _FINI("erreur de format sur ni i ni f");
      i += 2;
    }
  _ALLOUE(cols,ncol,void *);
  int nreserved = 0;;
  for(i=0;i<ncol;i++)
    cols[i] = NULL;
  // en avant simone, on traite...  
  FILE *ft=NULL;  // si vrai fichier
  vector<char*> lines;  // les lignes (NULL terminated)
  if (!fakeFile)  // on a un VRAI fichier
    {
      if (string(filename)=="stdin")
        ft = stdin;
      else
        ft= fopen(filename,"r");
      if (ft==NULL)
        {
          char aux[256];
          sprintf(aux,"je n'ai pu ouvrir le fichier %s",filename);
          _FINI(string(aux));
        }
    }
  else
    {
      // Ce n'est pas un vrai fichier:
      // On va remplacer les "_" par des " "
      // puis les deux caracteres '\' et 'n' par '\n' et 0
      // tout en gardant l'adresse du debut de ligne
      int n=strlen(filename);
      char *file;
      _ALLOUE(file,n,char);
      lines.push_back(file);
      for(i=0;i<n;i++)
        {
          if (filename[i]=='_')
            file[i] = ' ';
          else if ((i==n-1)||((filename[i]=='\\')&&(filename[i+1]=='n')))
            {
              file[i] = '\n';
              i++; // donc filename[i]=n
              file[i] = 0;
              // c'est la string qui commence 
              if (filename[i+1]) // sans ga il y aurait une ligne vide (sans cr) a la fin
                lines.push_back(file+i+1);
            }
          else
            file[i]=filename[i];
        }
    }
  nline = 0;
  int lignevide = 0,enregnumer = 0;
  unsigned int nextline = 0; // utilise si fakeLine==1
  for (;;)
    {
      char line[MAXLENGTLINE];
      if (!fakeFile)
        {
          if (fgets(line,MAXLENGTLINE,ft) == NULL)
            break;
        }
      else
        {
          if (nextline==lines.size())
            break;
          strcpy(line,lines[nextline++]);
          // printf("TRAITE : |%s",line);
        }
      // ici ligne est OK que ce soit file ou fakefile
      if (line[0]=='\n')
        lignevide++;
      if (lignevide==2)
        {
	  if (nline!=0)
	    enregnumer++;// On incremente que si enreg non vide
          if (enregnumer==n0+1)
	    break;
          lignevide = 0;
	  nline = 0;
          continue;
        }
      if (lignevide==1)
        continue;
      lignevide = 0;
      if (line[0]=='#')
        {
          comments += string(line);
          continue;
        }
      if (enregnumer<n0)
        {
	  nline = -1;  // pour noter que l'enreg. courant n'est pas vide
	  continue; // car c'est une data d'un enregistrement non voulu
	}
      char *pt = line;
      while(*pt==' ') // si line commence par ' ' ca merde ...
        pt++;
      int realloue = (nline==nreserved);
      if (realloue)
        {
          nreserved += BLOCKSIZE;
          //printf("Realloue nline= %d\n",nline);
          //fflush(stdout);
        }
      for (i=0;i<ncol;i++)
        {
          if (type[i]=="i32")
            {
              int x,*point; // valeur lue et pointeur ou la ranger
              if (sscanf(pt,"%d",&x)!=1)
                _FINI("probleme avec une ligne pour un i32");
              if (realloue)
                {
                  _REALLOUE(cols[i],nreserved,int);
                }
              point = (int *)cols[i];
              point[nline] = x;
              while(*pt==' ')
                pt++;
              while(*pt!=' ')
                pt++;
            }
          else if (type[i]=="i8")
            {
              char x,*point;
              if (sscanf(pt,"%hhd",&x)!=1)
                _FINI("probleme avec une ligne pour un i8");
              if (realloue)
                {
                  _REALLOUE(cols[i],nreserved,char);
                }
              point = (char *)cols[i];
              point[nline] = x;
              while(*pt==' ')
                pt++;
              while(*pt!=' ')
                pt++;
            }
          else if (type[i]=="i16")
            {
              short x,*point;
              if (sscanf(pt,"%hd",&x)!=1)
                _FINI("probleme avec une ligne pour un i16");
              if (realloue)
                {
                  _REALLOUE(cols[i],nreserved,short int);
                }
              point = (short *)cols[i];
              point[nline] = x;
              while(*pt==' ')
                pt++;
              while(*pt!=' ')
                pt++;
            }
          else if (type[i]=="i64")
            {
              long long int x,*point;
              if (sscanf(pt,"%lld",&x)!=1)
                _FINI("probleme avec une ligne pour un i64");
              if (realloue)
                {
                  _REALLOUE(cols[i],nreserved,long long int);
                }
              point = (long long int *)cols[i];
              point[nline] = x;
              while(*pt==' ')
                pt++;
              while(*pt!=' ')
                pt++;
            }
          else if (type[i]=="f32")
            {
              float x,*point;
              if (sscanf(pt,"%f",&x)!=1)
                _FINI("probleme avec une ligne pour un f32");
              if (realloue)
                {
                  _REALLOUE(cols[i],nreserved,float);
                }
              point = (float *)cols[i];
              point[nline] = x;
              while(*pt==' ')
                pt++;
              while(*pt!=' ')
                pt++;
            }
          else if (type[i]=="f64")
            {
              double x,*point;
              if (sscanf(pt,"%lf",&x)!=1)
                {
                  char st[128];
                  sprintf(st,"probleme avec ligne %d |%s| champ %d |%s| pour un f64",
                          nline,line,i,pt);
                  _FINI(st);
                }
              if (realloue)
                {
                  _REALLOUE(cols[i],nreserved,double);
                }
              point = (double *)cols[i];
              point[nline] = x;
              while(*pt==' ')
                pt++;
              while(*pt!=' ')
                pt++;
            }
          else if (type[i]=="%s")
            {
              _STOP("format %%s pas implemente : vazy!");
            }
        } // for(i=0;i<ncol;i++)
      nline++;
    } // for(;;){ ...
  if (!fakeFile)
    fclose(ft);
  ft = NULL;
  if (nline==-1)      // on a rien trouve
    _FINI("pas d'enregistrement");
    // REALLOUE FINALE
  nreserved = nline+1;
  for (i=0;i<ncol;i++)
    {
      if (type[i]=="i8")
        {
          _REALLOUE(cols[i],nreserved,char);
        }
      else if (type[i]=="i16")
        {
          _REALLOUE(cols[i],nreserved,short int);
        }
      else if (type[i]=="i32")
        {
          _REALLOUE(cols[i],nreserved,int);
        }
      else if (type[i]=="i64")
        {
          _REALLOUE(cols[i],nreserved,long long int);
        }
      else if (type[i]=="f32")
        {
          _REALLOUE(cols[i],nreserved,float);
        }
      else if (type[i]=="f64")
        {
          _REALLOUE(cols[i],nreserved,double);
        }
      else if (type[i]=="%s")
        {
          _STOP("format %%s pas implemente : vazy!\n");
        }
      else
        {
          _STOP("Erreur interne impossible\n");
        }
    } // for(i=0;i<ncol;i++)

  this->filename = string(filename);
}     // FIN enreg::enreg(string filename)
// **********************************************************************

enreg::~enreg()
{
  int i;
  if (cols==NULL)
    return;
  for(i=0;i<ncol;i++)
    {
      if (cols[i])
        free(cols[i]);
    }
  free(cols);
  cols = NULL;
}     // FIN enreg::enreg(char *format,char *filename,int n0,int fakeFile)
// **********************************************************************

void enreg::realloue(int n)
// toutes les colonnes sont reallouees a la nouvelle valeur n
{
  int i;
  for (i=0;i<ncol;i++)
    {
      if (type[i]=="i32")
        {
          _REALLOUE(cols[i],n,int);
        }
      else if (type[i]=="i8")
        {
          _REALLOUE(cols[i],n,char);
        }
      else if (type[i]=="i16")
        {
          _REALLOUE(cols[i],n,char);
        }
      else if (type[i]=="i64")
        {
          _REALLOUE(cols[i],n,long long int);
        }
      else if (type[i]=="f32")
        {
          _REALLOUE(cols[i],n,float);
        }
      else if (type[i]=="f64")
        {
          _REALLOUE(cols[i],n,double);
        }
      else if (type[i]=="%s")
        {
          _STOP("format %%s pas implemente : vazy!");
        }
    }
  nline = n;
}     // FIN void enreg::realloue(int n)
// **********************************************************************
 
