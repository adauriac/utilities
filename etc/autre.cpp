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

#if 0
inline bool operator<(const coord2& lhs, const coord2& rhs)
{
  if (lhs.x == rhs.x)
    return lhs.y < rhs.y;
  return lhs.x  < rhs.x;
}     // FIN inline bool operator<(const coord2& lhs, const coord2& rhs)
// **********************************************************************

inline bool operator<(const coord3& lhs, const coord3& rhs)
{
  if (lhs.x == rhs.x)
    {
      if (lhs.y == rhs.y)
        return lhs.z < rhs.z;
      return lhs.y  < rhs.y;
    }
  return lhs.x  < rhs.x;
}     // FIN inline bool operator<(const coord3& lhs, const coord3& rhs)
// ***************************************************************************
#endif
static int label_orig(int *t,int k)
{
  int i;
  for(i=k;;)
    if (t[i]==i)
      return i;
    else
      i = t[i];
}     // FIN int label_orig(int *t,int k)
// ***************************************************************************
static int cmp_int(int *i,int *j){return (*i) > (*j);}
#define _ETEINT -2
#define _ALLUME_SANS_LABEL -1
//#define _STATE_LABEL(k1,k2,k3) ((k1==0)+2*(k1==L1-1)+4*(k2==0)+8*(k2==L2-1)+16*(k3==0)+32*(k3==L3-1))

void labelise_alahk(int *site,int L1,int L2,int per)
// recoit la 
{
  int i,*labels;
  int V = L1*L2;
  labels = site; // pour ressembler a la version avec vecteur
  for(i=0;i<V;i++)
    labels[i] = labels[i]==0 ? _ETEINT : _ALLUME_SANS_LABEL;

  int *labeldelabel = NULL;
  int r_labeldelabel=0,n_labeldelabel=0;
  //  char *statelabel = NULL;
  for(i=0;i<V;i++)
    {
      if (labels[i] == _ETEINT)
        continue;
      if (labels[i] != _ALLUME_SANS_LABEL)
        continue;
      // ici on a un site allume mais pas labelise
      int x1,x2;
      _UNINDEX_SITE_2(i,x1,x2);
      int v[4],k=0;
      int l0p=-1,l0m=-1,l1p=-1,l1m=-1;
      if (per)
        {
          l0p = labels[_INDEX_SITE_2((x1+1)%L1,x2)];
          l0m = labels[_INDEX_SITE_2((L1+x1-1)%L1,x2)];
          l1p = labels[_INDEX_SITE_2(x1,(x2+1)%L2)];
          l1m = labels[_INDEX_SITE_2(x1,(L2+x2-1)%L2)];
        }
      else
        {
          if (x1!=L1-1)
            l0p = labels[_INDEX_SITE_2(x1+1,x2)];
          if (x1!=0)
            l0m = labels[_INDEX_SITE_2(x1-1,x2)];
          if (x2!=L2-1)
            l1p = labels[_INDEX_SITE_2(x1,x2+1)];
          if (x2!=0)      
            l1m = labels[_INDEX_SITE_2(x1,x2-1)];
        }
      if (l0p>=0) // car label>=0 veut dire deja numerote
        v[k++] =  label_orig(labeldelabel,l0p);
      if (l0m>=0)
        v[k++] =  label_orig(labeldelabel,l0m);
      if (l1p>=0)
        v[k++] =  label_orig(labeldelabel,l1p);
      if (l1m>=0)
        v[k++] =  label_orig(labeldelabel,l1m);
      if (k==0)
        {          // a new CC starts ...
          if (r_labeldelabel==n_labeldelabel)
            {
              r_labeldelabel += 4096;
              _REALLOUE(labeldelabel,r_labeldelabel,int);
              //_REALLOUE(statelabel,r_labeldelabel,char);
            }
          labeldelabel[n_labeldelabel] = n_labeldelabel;
          //statelabel[n_labeldelabel] = _STATE_LABEL(x1,x2,x3);
          labels[i] = n_labeldelabel++;
        }
      else if (k==1)
        {
          labels[i] = v[0];
          //statelabel[v[0]] |= _STATE_LABEL(x1,x2,x3);
        }
      else
        {
          qsort(v,k,sizeof(int),_PROTO_QSORT cmp_int);
          int j;
          labels[i] = v[0];
          //int newlabel =_STATE_LABEL(x1,x2,x3);
          for(j=0;j<k;j++)
            {
              labeldelabel[v[j]] = labeldelabel[v[0]];
              _UNINDEX_SITE_2(v[j],x1,x2);
              //newlabel |= statelabel[v[j]];
            }
          //statelabel[v[0]] = newlabel;
        }  //fin k>1
    } // for(i=0
  for (i=0;i<V;i++)
    if (labels[i]>=0)
      labels[i] = label_orig(labeldelabel,labels[i]);
  free(labeldelabel);
  // free(statelabel);
  return ;
}     // FIN void *labelise_alahk(int *site,int L1,int L2,int per)
// *******************************************************************

void labelise_alahk(int *site,int L1,int L2,int L3,int per)
// recoit la liste des coordonnees des points,et la taille du cube et les conditions de bord
// retourne un tableau d'entiers de taille L1*L2*L3, l'entier i est decompose en coordonnees
// avec _UNINDEX_SITE_3(i,x1,x2,x3), et en i le tableau contient le label de ce site.
// ATTENTION on melange coordonnes et numero de site.
{
  int i,*labels;
  int L23 = L2*L3;
  int V = L1*L23;
  labels = site; // pour meme notation que avec vecteur
  for(i=0;i<V;i++)
    labels[i] = labels[i]==0 ? _ETEINT : _ALLUME_SANS_LABEL;

  int *labeldelabel = NULL;
  int r_labeldelabel=0,n_labeldelabel=0;
  //  char *statelabel = NULL;
  for(i=0;i<V;i++)
    {
      if (labels[i] == _ETEINT)
        continue;
      if (labels[i] != _ALLUME_SANS_LABEL)
        continue;
      // ici on a un site allume mais pas labelise
      int x1,x2,x3;
      _UNINDEX_SITE_3(i,x1,x2,x3);
      int v[6],k=0;
      int l0p=-1,l0m=-1,l1p=-1,l1m=-1,l2p=-1,l2m=-1;
      if (per)
        {
          l0p = labels[_INDEX_SITE_3((x1+1)%L1,x2,x3)];
          l0m = labels[_INDEX_SITE_3((L1+x1-1)%L1,x2,x3)];
          l1p = labels[_INDEX_SITE_3(x1,(x2+1)%L2,x3)];
          l1m = labels[_INDEX_SITE_3(x1,(L2+x2-1)%L2,x3)];
          l2p = labels[_INDEX_SITE_3(x1,x2,(x3+1)%L3)];
          l2m = labels[_INDEX_SITE_3(x1,x2,(L3+x3-1)%L3)];
        }
      else
        {
          if (x1!=L1-1)
            l0p = labels[_INDEX_SITE_3(x1+1,x2,x3)];
          if (x1!=0)
            l0m = labels[_INDEX_SITE_3(x1-1,x2,x3)];
          if (x2!=L2-1)
            l1p = labels[_INDEX_SITE_3(x1,x2+1,x3)];
          if (x2!=0)      
            l1m = labels[_INDEX_SITE_3(x1,x2-1,x3)];
          if (x3!=L3-1)
            l2p = labels[_INDEX_SITE_3(x1,x2,x3+1)];
          if (x3!=0)
            l2m = labels[_INDEX_SITE_3(x1,x2,x3-1)];
        }
      if (l0p>=0) // car label>=0 veut dire deja numerote
        v[k++] =  label_orig(labeldelabel,l0p);
      if (l0m>=0)
        v[k++] =  label_orig(labeldelabel,l0m);
      if (l1p>=0)
        v[k++] =  label_orig(labeldelabel,l1p);
      if (l1m>=0)
        v[k++] =  label_orig(labeldelabel,l1m);
      if (l2p>=0)
        v[k++] =  label_orig(labeldelabel,l2p);
      if (l2m>=0)
        v[k++] =  label_orig(labeldelabel,l2m);
      if (k==0)
        {          // a new CC starts ...
          if (r_labeldelabel==n_labeldelabel)
            {
              r_labeldelabel += 4096;
              _REALLOUE(labeldelabel,r_labeldelabel,int);
              //_REALLOUE(statelabel,r_labeldelabel,char);
            }
          labeldelabel[n_labeldelabel] = n_labeldelabel;
          //statelabel[n_labeldelabel] = _STATE_LABEL(x1,x2,x3);
          labels[i] = n_labeldelabel++;
        }
      else if (k==1)
        {
          labels[i] = v[0];
          //statelabel[v[0]] |= _STATE_LABEL(x1,x2,x3);
        }
      else
        {
          qsort(v,k,sizeof(int),_PROTO_QSORT cmp_int);
          int j;
          labels[i] = v[0];
          //int newlabel =_STATE_LABEL(x1,x2,x3);
          for(j=0;j<k;j++)
            {
              labeldelabel[v[j]] = labeldelabel[v[0]];
              _UNINDEX_SITE_3(v[j],x1,x2,x3);
              //newlabel |= statelabel[v[j]];
            }
          //statelabel[v[0]] = newlabel;
        }  //fin k>1
    } // for(i=0
  for (i=0;i<V;i++)
    if (labels[i]>=0)
      labels[i] = label_orig(labeldelabel,labels[i]);
  free(labeldelabel);
  // free(statelabel);
  return ;
}     // FIN  void labelise_alahk(int *site,int L1,int L2,int L3,int per)
// *******************************************************************

int *labelise_alahk(vector<coord2> coords,int L1,int L2,int per)
// recoit la liste des coordonnees des points,et la taille du cube et les conditions de bord
// retourne un tableau d'entiers de taille L1*L2*L3, l'entier i est decompose en coordonnees
// avec _UNINDEX_SITE_2(i,x1,x2), et en i le tableau contient le label de ce site.
// ATTENTION on melange coordonnes et numero de site.
{
  int i,*labels;
  int V = L1*L2;
  _ALLOUE(labels,V,int);
  for(i=0;i<V;i++)
    labels[i]= _ETEINT;
  for(i=0;i<(int)coords.size();i++)
    labels[_INDEX_SITE_2(coords[i].x,coords[i].y)] = _ALLUME_SANS_LABEL;
  int *labeldelabel = NULL;
  int r_labeldelabel=0,n_labeldelabel=0;
  //  char *statelabel = NULL;
  for(i=0;i<V;i++)
    {
      if (labels[i] == _ETEINT)
        continue;
      if (labels[i] != _ALLUME_SANS_LABEL)
        continue;
      // ici on a un site allume mais pas labelise
      int x1,x2;
      _UNINDEX_SITE_2(i,x1,x2);
      int v[4],k=0;
      int l0p=-1,l0m=-1,l1p=-1,l1m=-1;
      if (per)
        {
          l0p = labels[_INDEX_SITE_2((x1+1)%L1,x2)];
          l0m = labels[_INDEX_SITE_2((L1+x1-1)%L1,x2)];
          l1p = labels[_INDEX_SITE_2(x1,(x2+1)%L2)];
          l1m = labels[_INDEX_SITE_2(x1,(L2+x2-1)%L2)];
        }
      else
        {
          if (x1!=L1-1)
            l0p = labels[_INDEX_SITE_2(x1+1,x2)];
          if (x1!=0)
            l0m = labels[_INDEX_SITE_2(x1-1,x2)];
          if (x2!=L2-1)
            l1p = labels[_INDEX_SITE_2(x1,x2+1)];
          if (x2!=0)      
            l1m = labels[_INDEX_SITE_2(x1,x2-1)];
        }
      if (l0p>=0) // car label>=0 veut dire deja numerote
        v[k++] =  label_orig(labeldelabel,l0p);
      if (l0m>=0)
        v[k++] =  label_orig(labeldelabel,l0m);
      if (l1p>=0)
        v[k++] =  label_orig(labeldelabel,l1p);
      if (l1m>=0)
        v[k++] =  label_orig(labeldelabel,l1m);
      if (k==0)
        {          // a new CC starts ...
          if (r_labeldelabel==n_labeldelabel)
            {
              r_labeldelabel += 4096;
              _REALLOUE(labeldelabel,r_labeldelabel,int);
              //_REALLOUE(statelabel,r_labeldelabel,char);
            }
          labeldelabel[n_labeldelabel] = n_labeldelabel;
          //statelabel[n_labeldelabel] = _STATE_LABEL(x1,x2,x3);
          labels[i] = n_labeldelabel++;
        }
      else if (k==1)
        {
          labels[i] = v[0];
          //statelabel[v[0]] |= _STATE_LABEL(x1,x2,x3);
        }
      else
        {
          qsort(v,k,sizeof(int),_PROTO_QSORT cmp_int);
          int j;
          labels[i] = v[0];
          //int newlabel =_STATE_LABEL(x1,x2,x3);
          for(j=0;j<k;j++)
            {
              labeldelabel[v[j]] = labeldelabel[v[0]];
              _UNINDEX_SITE_2(v[j],x1,x2);
              //newlabel |= statelabel[v[j]];
            }
          //statelabel[v[0]] = newlabel;
        }  //fin k>1
    } // for(i=0
  for (i=0;i<V;i++)
    if (labels[i]>=0)
      labels[i] = label_orig(labeldelabel,labels[i]);
  free(labeldelabel);
  // free(statelabel);
  return labels;
}     // FIN int *labelise_alahk(vector<coord2> coords,int L1,int L2,int per)
// *******************************************************************

int *labelise_alahk(vector<coord3> coords,int L1,int L2,int L3,int per)
// recoit la liste des coordonnees des points,et la taille du cube et les conditions de bord
// retourne un tableau d'entiers de taille L1*L2*L3, l'entier i est decompose en coordonnees
// avec _UNINDEX_SITE_3(i,x1,x2,x3), et en i le tableau contient le label de ce site.
// ATTENTION on melange coordonnes et numero de site.
{
  int i,*labels;
  int L23 = L2*L3;
  int V = L1*L23;
  _ALLOUE(labels,V,int);
  for(i=0;i<V;i++)
    labels[i]= _ETEINT;
  for(i=0;i<(int)coords.size();i++)
    labels[_INDEX_SITE_3(coords[i].x,coords[i].y,coords[i].z)] = _ALLUME_SANS_LABEL;
  int *labeldelabel = NULL;
  int r_labeldelabel=0,n_labeldelabel=0;
  //  char *statelabel = NULL;
  for(i=0;i<V;i++)
    {
      if (labels[i] == _ETEINT)
        continue;
      if (labels[i] != _ALLUME_SANS_LABEL)
        continue;
      // ici on a un site allume mais pas labelise
      int x1,x2,x3;
      _UNINDEX_SITE_3(i,x1,x2,x3);
      int v[6],k=0;
      int l0p=-1,l0m=-1,l1p=-1,l1m=-1,l2p=-1,l2m=-1;
      if (per)
        {
          l0p = labels[_INDEX_SITE_3((x1+1)%L1,x2,x3)];
          l0m = labels[_INDEX_SITE_3((L1+x1-1)%L1,x2,x3)];
          l1p = labels[_INDEX_SITE_3(x1,(x2+1)%L2,x3)];
          l1m = labels[_INDEX_SITE_3(x1,(L2+x2-1)%L2,x3)];
          l2p = labels[_INDEX_SITE_3(x1,x2,(x3+1)%L3)];
          l2m = labels[_INDEX_SITE_3(x1,x2,(L3+x3-1)%L3)];
        }
      else
        {
          if (x1!=L1-1)
            l0p = labels[_INDEX_SITE_3(x1+1,x2,x3)];
          if (x1!=0)
            l0m = labels[_INDEX_SITE_3(x1-1,x2,x3)];
          if (x2!=L2-1)
            l1p = labels[_INDEX_SITE_3(x1,x2+1,x3)];
          if (x2!=0)      
            l1m = labels[_INDEX_SITE_3(x1,x2-1,x3)];
          if (x3!=L3-1)
            l2p = labels[_INDEX_SITE_3(x1,x2,x3+1)];
          if (x3!=0)
            l2m = labels[_INDEX_SITE_3(x1,x2,x3-1)];
        }
      if (l0p>=0) // car label>=0 veut dire deja numerote
        v[k++] =  label_orig(labeldelabel,l0p);
      if (l0m>=0)
        v[k++] =  label_orig(labeldelabel,l0m);
      if (l1p>=0)
        v[k++] =  label_orig(labeldelabel,l1p);
      if (l1m>=0)
        v[k++] =  label_orig(labeldelabel,l1m);
      if (l2p>=0)
        v[k++] =  label_orig(labeldelabel,l2p);
      if (l2m>=0)
        v[k++] =  label_orig(labeldelabel,l2m);
      if (k==0)
        {          // a new CC starts ...
          if (r_labeldelabel==n_labeldelabel)
            {
              r_labeldelabel += 4096;
              _REALLOUE(labeldelabel,r_labeldelabel,int);
              //_REALLOUE(statelabel,r_labeldelabel,char);
            }
          labeldelabel[n_labeldelabel] = n_labeldelabel;
          //statelabel[n_labeldelabel] = _STATE_LABEL(x1,x2,x3);
          labels[i] = n_labeldelabel++;
        }
      else if (k==1)
        {
          labels[i] = v[0];
          //statelabel[v[0]] |= _STATE_LABEL(x1,x2,x3);
        }
      else
        {
          qsort(v,k,sizeof(int),_PROTO_QSORT cmp_int);
          int j;
          labels[i] = v[0];
          //int newlabel =_STATE_LABEL(x1,x2,x3);
          for(j=0;j<k;j++)
            {
              labeldelabel[v[j]] = labeldelabel[v[0]];
              _UNINDEX_SITE_3(v[j],x1,x2,x3);
              //newlabel |= statelabel[v[j]];
            }
          //statelabel[v[0]] = newlabel;
        }  //fin k>1
    } // for(i=0
  for (i=0;i<V;i++)
    if (labels[i]>=0)
      labels[i] = label_orig(labeldelabel,labels[i]);
  free(labeldelabel);
  // free(statelabel);
  return labels;
}     // FIN int *labelise_alahk(vector<coord3> coords,int L1,int L2,int L3,int per)
// *******************************************************************

bool mycmp (vector<int> i,vector<int> j) { return (i.size()>j.size()); }

vector< vector<int> > structamas(int *labels,int V)
// labels est le tableau des labels de tous les sites
// qui sont _ETEINT ou avec un label >=0
// retourne le vecteur des amas classes du plus grand au plus petit:
// structamas[0] est le vecteur du plus grand amas
// structamas[1] est le vecteur de l'avant plus grand amas etc
{
  int i;
  vector< vector<int> > ans;
  map< int,vector<int> > lab;
  for(i=0;i<V;i++)
    {
      if (labels[i]==_ETEINT)
        continue;
      if (labels[i]==_ALLUME_SANS_LABEL)
        _STOP("un site sans label apres hk !!!\n");
      // ici in site labelise
      lab[labels[i]].push_back(i);
    }
  for( map< int,vector<int> >::iterator it=lab.begin();it!=lab.end();++it)
    ans.push_back(it->second);
  sort(ans.begin(), ans.end(), mycmp);
  return ans;
}     // FIN vector< vector<int> > structamas(int *labels,int L1,int L2,int L3)
// *******************************************************************

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
