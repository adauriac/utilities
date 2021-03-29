#include <math.h>
#include <stdlib.h>
#include <string>
#include <macros.h>
#include <vector>
#include <sstream>
using namespace std;
#include "etc.h"
#include "../filage/filage.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

double nbPtsEntiersCubique(uint R,uint Rmax)
// Rmax a la valeur 256 par defaut et peut etre omise
// si R>Rmax utilise la formule asymptotiquement vraie 4*Pi*R*R
// retourne le nombre de point du cube a coordonnes entieres dont la distance
// au centre est : R-1/2 < d <  R+1/2.
// place maximum prise = Rmax doubles par defaut 256 doubles
// le temps pour creer le vecteur pour 256 est ~0.6 secondes
{
  if (R>Rmax)
    return  M_PI*4*R*R;
  static vector<double> norme;
  if (R>=norme.size())
    {
      int n = (int)R+5;
      norme.clear();
      norme.resize(n,0);
      for(int ix=-n+1;ix<n;ix++)
	for(int iy=-n+1;iy<n;iy++)
	  for(int iz=-n+1;iz<n;iz++)
	    {
	      double d2 = (ix)*(ix) + (iy)*(iy) + (iz)*(iz);
	      int k = (uint)round(sqrt(d2));
	      if (k<n)
		norme[k]++;
	    }
    }
  return norme[R];
}     // FIN double nbPtsEntiersCubique(uint R,uint Rmax)
// ************************************************************************

double nbPtsEntiersCarre(uint R,uint Rmax)
// Rmax a la valeur 4096 par defaut et peut etre omise
// si R>Rmax utilise la formule asymptotiquement vraie 2*Pi*R
// retourne ne nombre de point du carre a coordonnes entieres dont la distance
// au centre est : R-1/2 < d <  R+1/2.
// place maximum prise = Rmax doubles par defaut 4096 doubles
// le temps pour creer le vecteur pour 4096 est ~0.3 secondes
{
  if (R>Rmax)
    return  2*M_PI*R;
  static vector<double> norme;
  if (R>=norme.size())
    {
      int n = (int)R+20;
      norme.clear();
      norme.resize(n,0);
      for(int ix=-n+1;ix<n;ix++)
	for(int iy=-n+1;iy<n;iy++)
	  {
	    double d2 = (ix)*(ix) + (iy)*(iy);
	    int k = (uint)round(sqrt(d2));
	    if (k<n)
	      norme[k]++;
	  }
    }
  return norme[R];
}     // FIN double nbPtsEntiersCarre(uint R,uint Rmax)
// ************************************************************************

int nbPtsEntiersCubeExact(int R2,int R2maxp)
// retourne le nombre de points du cube a coordonnees entieres
// dont le carre de la distance a l'origine est R2.
// au premier appel on calcule par enumeration lle table :
//   i) soit on a donne un deuxieme argument qui est la longueur max
//      que l'on va utiliser, on utiliser alors (max(128,cette valeur))
//   ii) si pas fourni on prend _R2MAXDEF
// si R2 demande le necessite ON REFAIT TOUT AVEC le nouveau R2Max
// EN RESUME:
// si on sait que l'on va utiliser avec la plus grande valeur RM
// on commence par un appel nbPtsEntiersCubeExact(x,RM);
// puis nbPtsEntiersCubeExact(y);
#define _R2MAXDEF3D 4096
{
  static map<int,int> A;
  static int R2max = 0;
  if ((R2>R2max) || (R2max==0))
    {
      // printf("Entering wit R2max= %d ",R2max);
      R2max = _MAX(_MAX(R2maxp,R2),_R2MAXDEF3D);
      int cote = (int)(sqrt(R2max)+0.5);
      A.clear();
      for (int x=-cote;x<=cote;x++)
	for (int y=-cote;y<=cote;y++)
	  for (int z=-cote;z<=cote;z++)
	    {
	      int d2 = x*x + y*y + z*z;
	      if (d2>cote*cote)
		continue;
	      A[d2] += 1;
	    }
      // printf("leaving with R2max= %d cote= %d  A.size= %lu\n",R2max,cote,A.size());
    }
  return A[R2];
}     // FIN int nbPtsEntiersCubeExact(int R2)
// **********************************************************************

int nbPtsEntiersCarreExact(int R2,int R2maxp)
// retourne le nombre de points du cube a coordonnees entieres
// dont le carre de la distance a l'origine est R2.
// au premier appel on calcule par enumeration lle table :
//   i) soit on a donne un deuxieme argument qui est la longueur max
//      que l'on va utiliser, on utiliser alors (max(128,cette valeur))
//   ii) si pas fourni on prend _R2MAXDEF
// si R2 demande le necessite ON REFAIT TOUT AVEC le nouveau R2Max
// EN RESUME:
// si on sait que l'on va utiliser avec la plus grande valeur RM
// on commence par un appel nbPtsEntiersCubeExact(x,RM);
// puis nbPtsEntiersCubeExact(y);
#define _R2MAXDEF2D 16384
{
  static map<int,int> A;
  static int R2max = 0;
  if ((R2>R2max) || (R2max==0))
    {
      //printf("Entering wit R2max= %d ",R2max);
      R2max = _MAX(_MAX(R2maxp,R2),_R2MAXDEF2D);
      int cote = (int)(sqrt(R2max)+0.5);
      A.clear();
      for (int x=-cote;x<=cote;x++)
	for (int y=-cote;y<=cote;y++)
	  {
	    int d2 = x*x + y*y;
	    if (d2>cote*cote)
	      continue;
	    A[d2] += 1;
	  }
      //printf("leaving with R2max= %d cote= %d  A.size= %lu\n",R2max,cote,A.size());
    }
  return A[R2];
}     // FIN int nbPtsEntiersCarreExact(int R2)
// **********************************************************************

vector<string> findAllInString(string st,string variableName)
// trouve les occurences de name=XXX ou nameXXX dans findInString
// retourne le vecteur des XXX
// examples findAllInString("L=13 L= 13 L=14 blabla autreL=toto ","L=") retournera "13", "14", "toto"
// attention il faut donc plutot findAllInString("L=13 L= 13 L=14 blabla autreL=toto "," =")
// mais ds ce cas le PREMIER sera saute ... (en general 1er= #)
{
  vector<string> ans;
  //  char stc[ans.size()];
  uint found = -1;
  for(;;)
    {
      found = st.find(variableName,found+1);
      if (found==(uint)(-1))
	break;
      found += variableName.size();
      if (st[found]==' ')
	found++; // on enleve l'eventuel ' ' apres =
      uint found1 = found;
      for(;;)
	{
	  found1++;
	  if (found1==ans.size())
	    break;
	  if ((st[found1]==' ')||(st[found1]=='\t'))
	    break;
	}
      string res = st.substr(found,found1-found);
      ans.push_back(res);
    }
  set<string> ansS;
  for(uint i=0;i<ans.size();i++)
    ansS.insert(ans[i]);
  ans.clear();
  for(set<string>::iterator it=ansS.begin();it!=ansS.end();++it)
    ans.push_back(*it);
  return ans;
}     // FIN vector<string> findAllInString(string st,string variableName)
// **********************************************************************

int appendtofileblocking(char *filename,char *data,int timeout_insec)
/*               PERMET DES ACCES CONCURENTS AU MEME FICHIER SANS TIMER
 Si filename n'est pas bloque:
         bloque le fichier
         ecrit la chaine data a la fin du fichier filename
	 debloque le fichier
 Sinon (donc filename est bloque):
         attend 1 seconde et re-essaie, mais pas plus de timeout_insec fois

 Return value:
    0 succes
    1 pas pu ouvrire filename
    2 pas pu obtenir le verrou apres timeout_insec essais
    3 pas pu ecrire dans le fichier
    4 pas pu deverouiller le fichier
*/
{
  int i;
  int desc = open(filename,O_WRONLY|O_APPEND);
  if (desc<0)
    return 1; // pas pu ouvrir
  for(i=0;i<timeout_insec;i++)
    {
      if (!flock(desc,LOCK_NB|LOCK_EX))
        break;
      printf("waiting %d\n",i);
      fflush(stdout);
      sleep(1);
    }
  if (i==timeout_insec)
    return 2;// erreur car on a pas eu le fichier deverouille
  int s = strlen(data);
  if (write(desc,data,s)!=s)
    return 3;// on a pas pu ecrire dans le fichier
  i = flock(desc,LOCK_UN);
  close(desc);
  if (i==-1)
    return 4;      // on a pas pu verouiller le fichier
  return 0; // OK
}     // FIN int appendtofile(char *data,int timeout_insec)
// **********************************************************************

template<typename T> int dicho_search_template(T *t,int n,T k)
// t est un tablau de taille n croissant t[0] <= t[1] <= ... <= t[n-1]
// retourne le rang de k dans t et -1 si absent
{
  if (n==0)
    return -1;
  if (k<t[0])
    return -1;
  if (k>t[n-1])
    return -1;
  int i = 0;
  int a = n-1;
  for(;;)
    {
      if (t[i] == k)
        return i;
      if (t[a] == k)
        return a;
      if (a==i+1)
        return -1;
      int x = (i+a)/2;
      if (k<t[x])
        a = x;
      else
        i = x;
    }
}     // FIN dicho_search()
// ****************************************************************

template<typename T> int locate_template(T *t,int n,T k)
// t est un tablau de taille n croissant t[0] <= t[1] <= ... <= t[n-1]
//                     ****************************
// retourne i tel que  *    t[i] <= k < t[i+1]    *
//                     ****************************
// on considere t[-1] = -infinity t[n] = infinity
{
  if (n==0)
    return -1;
  if (k<t[0])
    return -1;
  if (k>=t[n-1])
    return n-1;
  int i = 0;
  int a = n-1;
  for(;;)
    {
      if (a == i+1)
        return i;
      int x = (i+a)/2;
      if (k<t[x])
        a = x;
      else
        i = x;
    }
}     // FIN locate_template()
// ****************************************************************

int dicho_search(int *T,int n,int k) {return dicho_search_template(T, n, k);}
int dicho_search(long long  *T,int n,long long int k) {return dicho_search_template(T, n, k);}
int dicho_search(double *T,int n,double k) {return dicho_search_template(T, n, k);}
int locate(int *T,int n,int k) {return locate_template(T, n, k);}
int locate(long long  *T,int n,long long int k) {return locate_template(T, n, k);}
int locate(double *T,int n,double k) {return locate_template(T, n, k);}


int cmpint(int *i,int *j){return (*i) > (*j);}

D2::D2(int L1,int L2,int L3,int withN,int dontcreatefile)  
// L2 ou L3 =-1 ==> L2 ou L3 = L1
// rend le tableau qui donne la position du nieme valeur de d**2 pour 3d
// O -> 0 (0,0,0)
// 1 -> 1 (0,0,+/-1) (0,+/-1,0) (+/-1,0,0)...
// 2 -> 2 (0,+/-1,+/-1) etc
// 3 -> (1,1,1)
// 4 -> (2,0,0)
// m est le nombre d'elemnet de ce tableau
{
  dim = 3;
  int i;
  if (L2==-1)
    L2 = L1;
  if (L3==-1)
    L3 = L1;
  this->L1 = L1;
  this->L2 = L2;
  this->L3 = L3;
  int L23=L2*L3;
  int L123=L1*L2*L3;
  double test = (L1*0.5)*(L2*0.5)*(L3*0.5);
  if (test > 2147483647.0)
    _STOP("test= %.16lg Necessite des long long int : todo !\n",test);
  int dmax = 0x7fffffff;  // distance au-dela de laquelle il manque des cibles
  if ((L1/2) && (L1/2<dmax))
    dmax = L1/2;
  if ((L2/2) && (L2/2<dmax))
    dmax = L2/2;
  if ((L3/2) && (L3/2<dmax))
    dmax = L3/2;

  char filename[1024];
  sprintf(filename,"D2_%d_%d_%d.bin",L1,L2,L3);
  FILE *ft = fopen(filename,"r");
  if (ft!=NULL)
    {
      //printf("je lis...");fflush(stdout);
      fclose(ft);
      long long int lli;
      char *comments,*buf;
      i = readfromfile(filename,&comments,&buf,&lli);
      if (i)
        _STOP("readfromfile de %s ra reourne %d\n",filename,i);
      m= lli/sizeof(int)/2;
      D2pos = (int *)buf;
      N = (int*)(buf+m*sizeof(int));
      return;
    }
  // pas pu lire le fichier binaire : je le fais et le copie
  //printf("pas pu lire\n");fflush(stdout);
  int k = 0;
  int *D2s;
  _ALLOUE(D2s,L123,int);
  for(i=0;i<L123;i++)
    {
      int x,y,z;
      _UNINDEX_SITE_3(i,x,y,z);
      int dx = x<=L1/2 ? x : L1-x;
      int dy = y<=L2/2 ? y : L2-y;
      int dz = z<=L3/2 ? z : L3-z;
      int d2 = dx*dx+dy*dy+dz*dz;
      if (d2>dmax*dmax)
        continue;
      D2s[k++]=d2;
    }
  qsort(D2s,k,sizeof(int),_PROTO_QSORT (cmpint));
  int n_D2pos = 0;
  int reserved_D2pos = 8192;
  D2pos=NULL;
  _ALLOUE(D2pos,reserved_D2pos,int);
  D2pos[n_D2pos++] = 0;
  for(i=1;i<k;i++)
    if (D2s[i]!=D2s[i-1])
      {
        if (n_D2pos==reserved_D2pos)
          {
            reserved_D2pos += 8192;
            _REALLOUE(D2pos,reserved_D2pos,int);
          }
        D2pos[n_D2pos++] = D2s[i];
      }
  _REALLOUE(D2pos,n_D2pos,int);
  free(D2s);
  m = n_D2pos;
  if (!dontcreatefile)
    {
      char comments[1024];
      strcpy(comments,"nombre de site a distance d; d(x)=x<=L/2 ? x,L-x\n");  
      long long int lli = sizeof(int)*m;
      i = writeinfile(filename,comments,(char *)D2pos,&lli);
      if (i)
        _STOP("writeinfile(%s) returned %d\n",filename,i);
    }
  N = NULL;
  if (withN)
    {
      _ALLOUE(N,m,int);
      // combien de fois chaque distance apparait
      for(i=0;i<m;i++)
        N[i] = 0;
      for(i=0;i<L123;i++)
        {
          int x,y,z;
          _UNINDEX_SITE_3(i,x,y,z);
          int dx = x<=L1/2 ? x : L1-x;
          int dy = y<=L2/2 ? y : L2-y;
          int dz = z<=L3/2 ? z : L3-z;
          int d2 = dx*dx+dy*dy+dz*dz;
          if (d2>dmax*dmax)
            continue;
          int k = ou(d2);
          if ((k>=m) || (k<0))
            _STOP("%d vs %d\n",k,m);
          N[k]++;
        }
    }
  if (!dontcreatefile)
    {
      long long int lli = sizeof(int)*m;
      i = appendtofile(filename,(char*)N,&lli);
      if (i)
        _STOP("appendtofile(%s) areourne %d\n",filename,i);
    }
  //  for(i=0;i<n_D2pos;i++)
  //    printf("%d %d\n",i,D2pos[i]);
}/* FIN D2::D2(int L1,int L2,int L3,int withN) */
// *******************************************************************

D2::D2(int L1,int L2,int L3,int L4,int withN,int dontcreatefile)  
// L2 ou L3 =-1 ==> L2 ou L3 = L1
// rend le tableau qui donne la position du nieme valeur de d**2 pour 3d
// O -> 0 (0,0,0)
// 1 -> 1 (0,0,+/-1) (0,+/-1,0) (+/-1,0,0)...
// 2 -> 2 (0,+/-1,+/-1) etc
// 3 -> (1,1,1)
// 4 -> (2,0,0)
// m est le nombre d'elemnet de ce tableau
{
  dim = 4;
  int i;
  if (L2==-1)
    L2 = L1;
  if (L3==-1)
    L3 = L1;
  if (L4==-1)
    L4 = L1;
  this->L1 = L1;
  this->L2 = L2;
  this->L3 = L3;
  this->L4 = L4;
  double test = (L1*0.5)*(L2*0.5)*(L3*0.5)*(L4*0.5);
  if (test > 2147483647.0)
    _STOP("test= %.16lg Necessite des long long int : todo !\n",test);
  char filename[1024];
  sprintf(filename,"D2_%d_%d_%d_%d.bin",L1,L2,L3,L4);
  FILE *ft = fopen(filename,"r");
  if (ft!=NULL)
    {
      //printf("je lis...");fflush(stdout);
      fclose(ft);
      long long int lli;
      char *comments,*buf;
      i = readfromfile(filename,&comments,&buf,&lli);
      if (i)
        _STOP("readfromfile de %s ra reourne %d\n",filename,i);
      m= lli/sizeof(int)/2;
      D2pos = (int *)buf;
      N = (int*)(buf+m*sizeof(int));
      return;
    }  int L34 = L4*L3;
  int L234 = L2*L34;
  int L1234 = L1*L234;
  int V = L1234;
  int k = 0;
  int *D2s;
  int dmax = 0x7fffffff;  // distance au-dela de laquelle il manque des cibles
  if ((L1/2) && (L1/2<dmax))
    dmax = L1/2;
  if ((L2/2) && (L2/2<dmax))
    dmax = L2/2;
  if ((L3/2) && (L3/2<dmax))
    dmax = L3/2;
  if ((L4/2) && (L4/2<dmax))
    dmax = L4/2;

  _ALLOUE(D2s,V,int);
  for(i=0;i<V;i++)
    {
      int x,y,z,t;
      _UNINDEX_SITE_4(i,x,y,z,t);
      int dx = x<=L1/2 ? x : L1-x;
      int dy = y<=L2/2 ? y : L2-y;
      int dz = z<=L3/2 ? z : L3-z;
      int dt = t<=L4/2 ? t : L4-t;
      int d2 = dx*dx + dy*dy + dz*dz + dt*dt;
      if (d2>dmax*dmax)
        continue;
      D2s[k++]=d2;
    }
  qsort(D2s,k,sizeof(int),_PROTO_QSORT (cmpint));
  int n_D2pos = 0;
  int reserved_D2pos = 8192;
  D2pos=NULL;
  _ALLOUE(D2pos,reserved_D2pos,int);
  D2pos[n_D2pos++] = 0;
  for(i=1;i<k;i++)
    if (D2s[i]!=D2s[i-1])
      {
        if (n_D2pos==reserved_D2pos)
          {
            reserved_D2pos += 8192;
            _REALLOUE(D2pos,reserved_D2pos,int);
          }
        D2pos[n_D2pos++] = D2s[i];
      }
  _REALLOUE(D2pos,n_D2pos,int);
  free(D2s);
  m = n_D2pos;
  if (!dontcreatefile)
    {
      char comments[1024];
      strcpy(comments,"nombre de site a distance d; d(x)=x<=L/2 ? x,L-x\n");  
      long long int lli = sizeof(int)*m;
      i = writeinfile(filename,comments,(char *)D2pos,&lli);
      if (i)
        _STOP("writeinfile returned %d\n",i);
    }
  N = NULL;
  if (withN)
    {
      _ALLOUE(N,n_D2pos,int);
      // combien de fois chaque distance apparait
      for(i=0;i<n_D2pos;i++)
        N[i] = 0;
      for(i=0;i<V;i++)
        {
          int x,y,z,t;
          _UNINDEX_SITE_4(i,x,y,z,t);
          int dx = x<=L1/2 ? x : L1-x;
          int dy = y<=L2/2 ? y : L2-y;
          int dz = z<=L3/2 ? z : L3-z;
          int dt = t<=L4/2 ? t : L3-t;
          int d2 = dx*dx+dy*dy+dz*dz+dt*dt;
          if (d2>dmax*dmax)
            continue;
          if ((x==2) && (y==2) && (z==4))
            dx = dx;
          int k = ou(d2);
          if ((k>=n_D2pos) || (k<0))
            _STOP("%d vs %d x,y,z,t= %d,%d,%d,%d dx,dy,dz,dt= %d,%d,%d,%d %d\n",
                  k,n_D2pos,x,y,z,t,dx,dy,dz,dt,d2);
          N[k]++;
        }
    }
  if (!dontcreatefile)
    {
      long long int lli = sizeof(int)*m;
      i = appendtofile(filename,(char*)N,&lli);
      if (i)
        _STOP("appendtofile(%s) areourne %d\n",filename,i);
    }
}/* FIN D2::D2(int L1,int L2,int L3,int L4,int withN) */
// *******************************************************************

string D2::comment(int d2,int all)
{
  if (dim==3)
    return comment3(d2,all);
  else if (dim==4)
    return comment4(d2,all);
  else
    return "xxx";
}     // FIN string D2::comment(int d2)
// *******************************************************************

D2::~D2()
{
  free(D2pos);
}     // FIN D2::~D2()
// *******************************************************************

string D2::comment3(int d2,int all)
// les manieres d'obtenir la valeur de d2
{
  ostringstream ans;
  int nb=0;
  // 
  int x,y,z;
  for(z=0;z<L3;z++)
    {
      // il faut faire d2-k*k;
      if (z*z>d2)
        break;
      for(y=0;y<L2;y++)
        {
          int x2 = d2-z*z-y*y;
          if (x2<0)  // augmenter y diminue x donc cet cond restera vraie
            break;
          x = rint(sqrt(x2));
          if (x*x!=x2) // pas un carre
            continue;
          // il faut voir toutes les sym.
          if (!all)
            {
              if ((z<=y) && (y<=x))
                {
                  ans << "("<<x<<" "<<y<<" "<<z<<")";
                  nb++;
                }
              continue;
            } // fin !all
          ans << "("<<x<<" "<<y<<" "<<z<<")";
          nb++;
          if (x!=0)
            {
              ans << "("<<-x<<" "<<y<<" "<<z<<")";
              nb++;
            }
          if (y!=0)
            {
              ans << "("<<x<<" "<<-y<<" "<<z<<")";
              nb++;
            }
          if (z!=0)
            {
              ans << "("<<x<<" "<<y<<" "<<-z<<")";
              nb++;
            }
          if ((x!=0)&&(y!=0))
            {
              ans << "("<<-x<<" "<<-y<<" "<<z<<")";
              nb++;
            }
          if ((y!=0)&&(z!=0))
            {
              ans << "("<<x<<" "<<-y<<" "<<-z<<")";
              nb++;
            }
          if ((x!=0)&&(z!=0))
            {
              ans << "("<<-x<<" "<<y<<" "<<-z<<")";
              nb++;
            }
          if ((x!=0)&&(y!=0)&&(z!=0))
            {
              ans << "("<<-x<<" "<<-y<<" "<<-z<<")";
              nb++;
            }
        }
    }
  ans << "   "<<nb;
  return ans.str();
}     // FIN string D2::comment3(int d2,int all)
// *******************************************************************

string D2::comment4(int d2,int all)
// les manieres d'obtenir la valeur de d2
{
  ostringstream ans;
  int nb=0;
  // 
  int x,y,z,t;
  for(t=0;t<L4;t++)
    {
      // il faut faire d2-t*t;
      if (t*t>d2)
        break;
      for(z=0;z<L3;z++)
        {
          // il faut faire d2-t*t-z*z;
          if (z*z+t*t>d2)
            break;
          for(y=0;y<L2;y++)
            {
              int x2 = d2-t*t-z*z-y*y;
              if (x2<0)  // augmenter y diminue x donc cet cond restera vraie
                break;
              x = rint(sqrt(x2));
              if (x*x!=x2) // pas un carre
                continue;
              if (!all)
                {
                  if ((t<=z) && (z<=y) && (y<=x))
                    {
                      ans << "("<<x<<" "<<y<<" "<<z<<" "<<t<<")";
                      nb++;
                    }
                  continue;
                } // fin !all
              // il faut voir toutes les sym.
              ans << "("<<x<<" "<<y<<" "<<z<<" "<<t<<")";
              nb++;
              int tab[4]={x,y,z,t};
              int i,j,k,l;
              for(i=0;i<4;i++)
                if (tab[i]!=0)
                  {
                    tab[i] = -tab[i];
                    ans << "("<<tab[0]<<" "<<tab[1]<<" "<<tab[2]<<" "<<tab[3]<<")";
                    nb++;
                    tab[i] = -tab[i];      
                  }

              for(i=0;i<4;i++)
                for(j=i+1;j<4;j++)
                  if ((tab[i]!=0) && (tab[j]!=0))
                    {
                      tab[i] = -tab[i];
                      tab[j] = -tab[j];
                      ans << "("<<tab[0]<<" "<<tab[1]<<" "<<tab[2]<<" "<<tab[3]<<")";
                      nb++;
                      tab[i] = -tab[i];
                      tab[j] = -tab[j];
                    }
              for(i=0;i<4;i++)
                for(j=i+1;j<4;j++)
                  for(k=j+1;k<4;k++)
                    if ((tab[i]!=0) && (tab[j]!=0) && (tab[k]!=0))
                      {
                        tab[i] = -tab[i];
                        tab[j] = -tab[j];
                        tab[k] = -tab[k];
                        ans << "("<<tab[0]<<" "<<tab[1]<<" "<<tab[2]<<" "<<tab[3]<<")";
                        nb++;
                        tab[i] = -tab[i];
                        tab[j] = -tab[j];
                        tab[k] = -tab[k];
                      }
              for(i=0;i<4;i++)
                for(j=i+1;j<4;j++)
                  for(k=j+1;k<4;k++)
                    for(l=k+1;l<4;l++)
                      if ((tab[i]!=0) && (tab[j]!=0) && (tab[k]!=0) && (tab[l]!=0))
                        {
                          //intf("%d %d %d %d\n",i,j,k,l);
                          tab[i] = -tab[i];
                          tab[j] = -tab[j];
                          tab[k] = -tab[k];
                          tab[l] = -tab[l];
                          ans << "("<<tab[0]<<" "<<tab[1]<<" "<<tab[2]<<" "<<tab[3]<<")";
                          nb++;
                          tab[i] = -tab[i];
                          tab[j] = -tab[j];
                          tab[k] = -tab[k];
                          tab[l] = -tab[l];
                        }
            }
        } // for (z
    } // for (t
  ans << "   "<<nb;
  return ans.str();
}     // FIN string D2::comment4(int d2,int all)
// *******************************************************************

int D2::ou(int x){return dicho_search(D2pos,m,x);}

vector<string> split_sep32(string str)
// retourne un vecteur de string qui sont
// les tokens de str separes par 32 ou \t
{
  string buf; // Have a buffer string
  stringstream ss(str); // Insert the string into a stream
  vector<string> tokens; // Create vector to hold our words
  while (ss >> buf)
    tokens.push_back(buf);
  return tokens;
}     // FIN string split_sep32(string str)
// ***********************************************************************

unsigned int splitIntBracketedString(string data,int **des)
// lit data qui doit contenir [ x x .. x ] ou x sont des entiers
// affecte des tel que des[i] = le i ieme entier
// retourne le nombre de champs
// et 0 si erreur
{
  *des = NULL;
  char *line = (char*)data.c_str();
  unsigned int i;
  for(i=0;i<strlen(line);i++)
    if (line[i]=='[')
      break;
  if (i==strlen(line))
    return 0;
  int ii = i+1;
  for(;i<strlen(line);i++)
    if (line[i]==']')
      break;
  if (i==strlen(line))
    return 0;
  int ia = i;
  //printf("ii ia = %d %d\n",ii,ia);
  string str = string(&data[ii], ia-ii);  
  //printf("%s\n",str.c_str());
  vector<string> aux = split_sep32(str);
  unsigned int n = aux.size();
  int *res;
  _ALLOUE(res,n,int);
  for (i=0;i<n;i++)
    {
      int a;
      if (sscanf((char*)aux[i].c_str(),"%d",&a)!=1)
        {
          free(res);
          return 0;
        }
      res[i] = a;
    }
  *des = res;
  return n;
}     // FIN unsigned int splitIntBracketedString(string data,int **des)
// ***********************************************************************

long long int question_lli(string Q)
// repose la question jusqu'a une reponse OK ATTENTION 3.6 --> 3 
{
  long long int x;
  char aux[512];
  for(;;)
    {
      printf("%s ",(char *)Q.c_str());
      fflush(stdout);
      if (fgets(aux,255,stdin)==NULL)
        continue;
      if (sscanf(aux,"%lld",&x)==1)
        return x;
    }
}     // FIN long long int question_lli(string Q)
// **************************************************************

double question_d(string Q)
// repose la question jusqu'a une reponse OK 
{
  double x;
  char aux[512];
  for(;;)
    {
      printf("%s ",(char *)Q.c_str());
      fflush(stdout);
      if (fgets(aux,255,stdin)==NULL)
        continue;
      if (sscanf(aux,"%lf",&x)==1)
        return x;
    }
}     // FIN long long int question_lli(string Q)
// **************************************************************

string question_str(string Q)
{
  string x;
  getline(cin,x);
  return x;
 }     // FIN long long int question_lli(string Q)
// **************************************************************

string question_str(string Q,vector<string> choix,int def)
// repose la question jusqu'a ce que la reponse soit un choix
// si on repond "\n" et que "def" est un numero de choix celui-ci est retourne 
{
  string x;
  int vraichoix = ((0<=def) && (def<(int)choix.size()));
  for(;;)
    {
      vector<string>::iterator it;
      cout << Q;
      for(it=choix.begin();it != choix.end();++it)
        {
          if (it==choix.begin())
            cout << " [";
          else
            cout << "/";
          cout << *it;
        }
      cout << "] ";
      if (vraichoix)
        cout <<" (defaut "<<choix[def]<<") ";
      getline(cin,x);
      if (x=="")
        {
          if (vraichoix)
            return choix[def];
        }
      for(it=choix.begin();it != choix.end();++it)
        if (*it==x)
          return x;
    }
 }     // FIN long long int question_lli(string Q)
// **************************************************************

vector<long long int> getLLs(char *data,char *cible)
// retourne un vecteur contenant toutes les valeurs de cible trouvees dans data
// data="seed=1 seed=  2 seed = 3 seed =4 seed=4 seed=2"
// cible ="seed"
// donne le vecteur 1,2,3,4
{
  set<long long int> ans;
  char *pc =  data;
  for(;;)
    {
      pc = strstr(pc,cible);
      if (pc==NULL)
        break;
      long long int lli;
      if (sscanf(pc+strlen(cible)," = %lld",&lli)==1)
        ans.insert(lli);
      pc+=strlen(cible);
    }
  vector< long long int> ret;
  for (set<long long int>::iterator it=ans.begin(); it!=ans.end(); ++it)
    ret.push_back(*it);

  return ret;
}     // FIN vector<char *> touve(char *data)
// **********************************************************************

vector<string> split(string line,char sep)
// retourne un vecteur de string dont chaque element est obtenu 
// en coupant la string  par le caractere separateur (par defaut '_'):
// "str1_str2_str3" retourne  un vectuer de 3 elements "str1","str2","str3" 
// ___str1_x=8___comm_lasr__ retourne un vecteur de 4 string:
// xtr1    x=8  commm last
{
  vector<string> ans;
  char *aux = (char *)line.c_str();
  unsigned int i0 = 0,i;
  for(i=0;i<strlen(aux);i++)
    {
      if (aux[i]==sep)
        {
          if (i-i0)
            {
              string cur=line.substr(i0,i-i0);
              ans.push_back(line.substr(i0,i-i0));
            }
          //printf("%d %d\n",i0,i);
          //i++;
          i0=i+1;
        }
    }
  string cur=line.substr(i0,i);
  if (cur!="")
    ans.push_back(line.substr(i0,i-i0));
       
  return ans;
}     // FIN vector<string> split(string line)
// *********************************************************************

int sendStringToServer(char *buffer,int portno)
{
  int sockfd,  n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    {
      fprintf(stderr,"ERROR opening socket");
      return 1;
    }
  server = gethostbyname((char*)"calc-maitre");
  if (server == NULL) 
    {
      fprintf(stderr,"ERROR, no such host\n");
      return 1;
    }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    {
      fprintf(stderr,"ERROR connecting");
      return 3;
    }
  //printf("Please enter the message: ");
  // bzero(buffer,256);
  //fgets(buffer,255,stdin);
  n = write(sockfd,buffer,strlen(buffer));
  if (n < 0) 
    {
      fprintf(stderr,"ERROR writing to socket");
      return 4;
    }
  // bzero(buffer,256);
  // n = read(sockfd,buffer,255);
  // if (n < 0) 
  //   error("ERROR reading from socket");
  // printf("%s\n",buffer);
  close(sockfd);
  return 0;
}     // FIN void sendStringToServer(char *st,int portno)
// **************************************************************

int sendKillToServer(int portno)
{
  char st[8];
  st[0] = 'k'-'a'+1;
  st[1] = 'i'-'a'+1;
  st[2] = 'l'-'a'+1;
  st[3] = 'l'-'a'+1;
  st[4] = 0;
  return sendStringToServer(st,portno);
}     // FIN int sendKillToServer(int portno=51720);
// **************************************************************

int ouvreFileNoClobber(char *nameout,FILE **ftout)
// IL FAUT QUE nameout PUISSE ACCOMODER 5 CARACTERES DE PLUS !!!
// ouvre en ecriture et si existe deja rajoute un suffixe 
// pas plus de 1000 suffixe 
// retourn e 0:Ok  1,2:pas pu ouvrir en ecriture  3: plus de suffixe disponible
{
  FILE *ft;
  ft = fopen(nameout,"r");
  if (ft==NULL)
    {
      ft = fopen(nameout,"w");
      if (ft==NULL)
        return 1;
      *ftout = ft;
      return 0;
    }
  // fichier existe deja
  fclose(ft);
  int n = strlen(nameout);
  for(int i=1;i<1000;i++)
    {
      sprintf(nameout+n,"_%d",i);
      ft = fopen(nameout,"r");
      if (ft==NULL)
        {
          ft = fopen(nameout,"w");
          if (ft==NULL)
            return 2;
          *ftout = ft;
          return 0;
        }
      fclose(ft);
    }
  return 3; // pas pu
}     // FIN int ouvreFileNoClobber(char *nameout,FT *ftout)
// **********************************************************************

void ouvreFileNoClobberForceExit(char *nameout,FILE **ftout,int force)
// IL FAUT QUE nameout PUISSE ACCOMODER 5 CARACTERES DE PLUS !!!
// ouvre en ecriture et si existe deja rajoute un suffixe 
// pas plus de 1000 suffixes
// si force=1 ne tenete pas de rajouter des suffixes
// exit si pas pu ouvrir
{
  FILE *ft;
  ft = fopen(nameout,"r");
  if (ft==NULL)
    {
      ft = fopen(nameout,"w");
      if (ft==NULL)
	{
	  printf("#ouvreFileNoClobberForceExit :pas pu ouvrir %s en ecriture",nameout);
	  exit(1024);
	}
      *ftout = ft;
      return ;
    }
  // fichier existe deja
  fclose(ft);
  if (force)
    {
      ft = fopen(nameout,"w");
      if (ft==NULL)
	{
	  printf("#ouvreFileNoClobberForceExit : pas pu ouvrir %s en ecriture",nameout);
	  exit(1024);
	}
      *ftout = ft;
      return ;
    }
  // fichier existe et pas force
  int n = strlen(nameout);
  for(int i=1;i<1000;i++)
    {
      sprintf(nameout+n,"_%d",i);
      ft = fopen(nameout,"r");
      if (ft==NULL)
        {
          ft = fopen(nameout,"w");
          if (ft==NULL)
	    {
	      printf("#ouvreFileNoClobberForceExit : pas pu ouvrir %s en ecriture",nameout);
	      exit(1024);
	    }
          *ftout = ft;
          return ;
        }
      fclose(ft);
    }
  printf("#ouvreFileNoClobberForceExit : %s plus de suffixe disponible",nameout);
  exit(1024);
}     // FIN void ouvreFileNoClobberExit(char *nameout,FT *ftout,int force)
// **********************************************************************

distribution::distribution(double basep){ntotalObs=0;nValeur=0;base = basep;};

distribution::~distribution(){obsF.clear();obsI.clear();}

int distribution::insert(double x,int kfois)
{
  ntotalObs += kfois;
  if (base==0)
    {
      obsI[x] += kfois;
      nValeur = (int) obsI.size();
    }
  else
    {
      obsF[((int)(x/base))*base] += kfois;
      nValeur = (int) obsF.size();
    }
  return ntotalObs;
}     // FIN int distribution::insert(double x,int kfois=1)
// **********************************************************************

void distribution::out(FILE *ft)
{
  fprintf(ft,"# nbValeurs= %d\n",nValeur);
  fprintf(ft,"# nbObservations= %d\n",ntotalObs);
  if (base==0)
    for(map<int,int>::iterator it=obsI.begin();it!=obsI.end();++it)
      fprintf(ft,"%d %d %.16lg\n",it->first,it->second,it->second/(1.0*ntotalObs));
  else
    for(map<double,int>::iterator it=obsF.begin();it!=obsF.end();++it)
      fprintf(ft,"%.16lg %d %.16lg\n",it->first,it->second,it->second/(1.0*ntotalObs));
}     // FIN void distribution::out(FILE *ft)
// **********************************************************************

int getMem()  // returm the Virtual Memory Size in Kb
{
  static int pid = 0;
  static char filename[256];
  if (pid==0)
    {
      pid_t pid = getpid();
      //printf("pid= %d\n",pid);
      sprintf(filename,"/proc/%d/status",pid);
    }
  FILE *ft = fopen(filename,"r");
  if (ft==NULL)
    return -1;
  for(;;)
    {
      char line[256];
      if (fgets(line,255,ft)==NULL)
        return 0; // on a du trouve VmSize avant
      int n;
      if (sscanf(line,"VmSize:    %d kB",&n)==1)
        return n;
    }
}     // FIN int getMem()
// **************************************************************************

int sizeOfFileInBytes(char *filename)
// retourne la taille d'un fichier en octets et -1 si erreur
{
  struct stat buf;
  int i = stat(filename,&buf);
  if (i)
    return -1;
  return (int)buf.st_size;
}     // FIN int sizeOfFileInBytes(char *filename)
// **************************************************************************

char *imageOfFile(char *filename,int *sizeOfFile)
// retourne un pointeur vers une zone memoire image
// du fichier, la taille est mise dans sizeOfFile
// si erreur retourne NULL et sizeOfFile indetermine
{
  int size = sizeOfFileInBytes(filename);
  if (size==-1)
    return NULL;
  *sizeOfFile = size;
  char *buf=(char*)malloc(size*sizeof(char));
  int fd = open(filename,O_RDONLY);
  if (fd==-1)
    {
      free(buf);
      return NULL;
    }
  int lus = read(fd,buf,size);
  if (lus != size)
  if (fd==-1)
    {
      free(buf);
      return NULL;
    }
  return buf;
}     // FIN int *imageOfFile(char *filename,int *sizeOfFile)
// **************************************************************************

//************************************************************************
//                      MESURE DU TEMSP ECOULE : class tempeur
//************************************************************************
tempeur::tempeur(){times(&timebufbegin);conversion = sysconf(_SC_CLK_TCK);}

void tempeur::print(FILE*ft)
{                                                                     
  struct tms timebufnow;                                              
  times(&timebufnow);
  int temps  = (int)(timebufnow.tms_utime - timebufbegin.tms_utime +  
		     timebufnow.tms_stime - timebufbegin.tms_stime);  
  fprintf(ft,"# duree_depuis_debut= %.15lg seconde(s)\n",             
            temps/conversion);                                          
}     // FIN void tempeur::print(FILE*ft);
// ********************************************************************

double tempeur::getInSecond()
{                                                                     
  struct tms timebufnow;                                              
  times(&timebufnow);
  int temps  = (int)(timebufnow.tms_utime - timebufbegin.tms_utime +  
		     timebufnow.tms_stime - timebufbegin.tms_stime);  
  return temps/conversion;                                          
}     // FIN double tempeur::getInSecond()
// ********************************************************************

void tempeur::reset(){times(&timebufbegin);}

vector<string> tokenize(char *input,char *delim)
// split en token qui sont separes par des separateur.
// les separateurs sont tous les caracteres de delim
// les separateurs en debut ou fin sont ignores
// plusieurs separeteurs consecutifs dont merge en un seul
{
  vector<string> rez;
  for(;;)
    {
      char *aux = strtok(input,delim);
      if (aux==NULL)
        break;
      rez.push_back(string(aux));
      input = NULL;
    }
  return rez;
}     // FIN vector<char*> tokenize(char *input,char *delim)
// **********************************************************************

string strReplace(string st,string target,string repl)
// retourne une string ou LA PREMIERE OCCURENCE de target dans st est
// remplacee par repl
{
  size_t pos = st.find(target);
  if (pos==string::npos)
    return st;
  string ans = st.substr(0,pos) + repl + st.substr(pos+target.length(),string::npos);
  return ans;
}     // FIN string strReplace(string st,string target,string repl)
// **********************************************************************

void strReplaceInPlace(string &st,string target,string repl)
// remplace LA PREMIERE OCCURENCE de target dans st par repl
{
  size_t pos = st.find(target);
  if (pos==string::npos)
    return ;
  st.replace(pos,target.length(),repl);
  return ;
}     // FIN string strReplaceInPlace(string &st,string target,string repl)
// **********************************************************************
