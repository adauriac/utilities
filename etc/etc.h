#ifndef _ETC_H
#define _ETC_H

#include <malloc.h>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <map>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h> 

using namespace std;

struct coord2
{
  short int x;
  short int y;
};

struct coord3
{
  short int x;
  short int y;
  short int z;
};

vector<string> findAllInString(string st,string variableName);
// trouve les occurences de name=XXX ou nameXXX dans findInString
// retourne le vecteur des XXX
// examples findAllInString("L=13 L= 13 L=14 blabla autreL=toto ","L=") retournera "13", "14", "toto"
// attention il faut donc plutot findAllInString("L=13 L= 13 L=14 blabla autreL=toto "," L=")
// mais ds ce cas le PREMIER sera saute ... (en general 1er= #)

int appendtofileblocking(char *filename,char *data,int timeout_insec);
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

vector<long long int> getLLs(char *data,char *cible);
// retourne un vecteur contenant toutes les valeurs de cible trouvees dans data
// data="seed=1 seed=  2 seed = 3 seed =4 seed=4 seed=2"
// cible ="seed"
// donne le vecteur 1,2,3,4
//  vector<long long int> V = getLLs("seed=1 seed=  2 seed = 3 seed =4 seed=4 seed=2",(char *)"seed");
 
int dicho_search(int *t,int n,int k);
int dicho_search(long long int *t,int n,long long k);
int dicho_search(double *t,int n,double k);
// t est un tablau de taille n croissant t[0] <= t[1] <= ... <= t[n-1]
// retourne le rang de k dans t et -1 si absent

int locate(int *t,int n,int k);
int locate(long long int *t,int n,long long k);
int locate(double *t,int n,double k);
// t est un tablau de taille n croissant t[0] <= t[1] <= ... <= t[n-1]
//                     ****************************
// retourne i tel que  *    t[i] <= k < t[i+1]    *
//                     ****************************
// on considere t[-1] = -infinity t[n] = infinity
// ****************************************************************


/*
  D2 myD2(L1,L2,L3,1);  
  for(d2=0;d2<myD2.m;d2++)
    {
      int k = myD2.ou(d2);
      if (k==-1)
        continue;
      printf("d2= %d N= %d\n",d2,myD2.N[k]);
      cout <<  myD2.comment(i) << endl;
    }
*/

class D2
{
public:
  int L1;
  int L2;
  int L3;
  int L4;
  int dim;
  int *D2pos;// k=D2pos[d2] signifie d2 est le kieme element par ordre croissant
  int *N;// n= N[d2] veut dire il y a n points a distance d2
  int m;

  D2(int L1,int L2,int L3,int withN,int dontcreatefile);  
// L2=-1->L2=L1 (idem L3) et withN decide si on calcule aussi N
// si dontcreatefile==0 on cree ds le directory courant un fichier binaire
// qui pourra etre relu
  D2(int L1,int L2,int L3,int L4,int withN,int dontcreatefile);  
// L2=-1->L2=L1 (idem L3,L4) et withN decide si on calcule aussi N
// si dontcreatefile==0 on cree ds le directory courant un fichier binaire
// qui pourra etre relu
  ~D2();
  int ou(int x);
  // trouve le rang de x dans D2pos
  string comment(int d2,int all=1);
  // retourne toutes les manieres de faire d**2 avec (all=0) ou sans (all=1) equivalence 
private:
  string comment3(int d2,int all);
  string comment4(int d2,int all);
};

int cmpint(int *i,int *j);
long long int question_lli(string Q);
// repose la question jusqu'a une reponse OK ATTENTION 3.6 --> 3 
double question_d(string Q);
// repose la question jusqu'a une reponse OK 
string question_str(string Q); // ' ' n est PAS une fin de chaine
string question_str(string Q,vector<string> choix,int def=-1);
// repose la question jusqu'a ce que la reponse soit un choix
// si on repond "\n" et que "def" est un numero de choix celui-ci est retourne 
// ' ' n est PAS une fin de chaine

//  vector<string> choix;
//  choix.push_back("oui");
//  choix.push_back("non");
//  string ans = question_str("alors ? ",choix,0); (si return est tape alors oui)

// JE NE SAIS PAS POURQUOI, MAIS CES 2 SURCHARGES D'OPERATEURS NE PEUVENT
// ETRE MISES DANS libetc.cpp

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

//inline bool operator<(const coord2& lhs, const coord2& rhs);
//inline bool operator<(const coord3& lhs, const coord3& rhs);
int *labelise_alahk(vector<coord3> coords,int L1,int L2,int L3,int per);
int *labelise_alahk(vector<coord2> coords,int L1,int L2,int per);
void labelise_alahk(int *site,int L1,int L2,int per);
void labelise_alahk(int *site,int L1,int L2,int L3,int per);
vector< vector<int> > structamas(int *labels,int V);


// retourne un vecteur de string dont chaque element est obtenu 
// en coupant la string  par le separateur (par defaut le caractere '_'):
// "str1_str2_str3" retourne  un vectuer de 3 elements "str1","str2","str3" 
// ___str1_x=8___comm_lasr__ retourne un vecteur de 4 string:
// xtr1    x=8  commm last
vector<string> split(string line,char sep='_');
vector<string> split_sep32(string str);
unsigned int splitIntBracketedString(string data,int **des);
// lit data qui doit contenir [ x x .. x ] ou x sont des entiers
// affecte des tel que des[i] = le i ieme entier
// retourne le nombre de champs, 0 si erreur

int sendStringToServer(char *buffer, int portno=51720);

int ouvreFileNoClobber(char *nameout,FILE **ftout);
// IL FAUT QUE nameout PUISSE ACCOMODER 5 CARACTERES DE PLUS !!!
// ouvre en ecriture et si existe deja rajoute un suffixe 
// pas plus de 1000 suffixe 
// retourn e 0:Ok  1,2:pas pu ouvrir en ecriture  3: plus de suffixe disponible

void ouvreFileNoClobberForceExit(char *nameout,FILE **ftout,int force);
// IL FAUT QUE nameout PUISSE ACCOMODER 5 CARACTERES DE PLUS !!!
// ouvre en ecriture et si existe deja rajoute un suffixe 
// pas plus de 1000 suffixe
// si force on ne tente pas d'ajouter de suffixe (uniquement pour ecriture compacte)
// si erreur: exit

int getMem();  // returm the Virtual Memory Size in Kb

class distribution
{
public:
  int ntotalObs;
  double base;
  int nValeur;
  map<double,int> obsF; // obs[x]=k veut dire x a ete vu k fois
  map<int,int> obsI; // obs[x]=k veut dire x a ete vu k fois
  distribution(double basep);
  // basep=0 veut dire on observe des entiers
  // sinon on observe des reels que l'on a normalise par (int)(x/base)*base
  ~distribution();
  int insert(double x,int kfois=1);
  void out(FILE *ft=stdout);
};  // fin class  distribution

// retourne la taille d'un fichier en octets et -1 si erreur
int sizeOfFileInBytes(char *filename);

// retourne un pointeur vers une zone memoire image
// du fichier, la taille est mise dans sizeOfFile
// si erreur retourne NULL et sizeOfFile indetermine
char *imageOfFile(char *filename,int *sizeOfFile);

//************************************************************************
//                      MESURE DU TEMSP ECOULE : class tempeur
//************************************************************************
// mesure le temps cpu/reel ecoule depuis l'instsntation de la classe
// ou son reset
// tempeur my; blalba ...; tempeur.print();
// temper.reset();blalba ...; double tps=tempeur.getInSecond()
class tempeur
{
 public:
   struct tms timebufbegin;
   double conversion;

   tempeur();
   ~tempeur(){};
   void print(FILE*ft=stdout);
   void reset();
   double getInSecond(); // tps user + system
};

vector<string> tokenize(char *input,char *delim);
// split en token qui sont separes par des separateur.
// les separateurs sont tous les caracteres de delim
// les separateurs en debut ou fin sont ignores
// plusieurs separeteurs consecutifs dont merge en un seul

string strReplace(string st,string target,string repl);
// retourne une string ou LA PREMIERE OCCURENCE de target dans st est remplacee par repl

void strReplaceInPlace(string &st,string target,string repl);
// remplace LA PREMIERE OCCURENCE de target dans st par repl

// Rmax a la valeur 256 par defaut et peut etre omise
// si R>Rmax utilise la formule asymptotiquement vraie 4*Pi*R*R
// retourne le nombre de point du cube a coordonnes entieres dont la distance
// au centre est : R-1/2 < d <  R+1/2.
// place maximum prise = Rmax doubles par defaut 256 doubles
// le temps pour creer le vecteur pour 256 est ~0.6 secondes

double nbPtsEntiersCubique(uint R,uint Rmax=256);
double nbPtsEntiersCarre(uint R,uint Rmax=4096);
// retourne le nombre de point du cube/carre a coordonnes entieres dont
// la distance au centre est : R-1/2 < d <  R+1/2.
// si R<Rmax denombre, sinon 4*Pi*R**2 ou 2*Pi*R
// place maximum prise = Rmax doubles 
// le temps pour creer le vecteur 3d pour 256 est ~0.6 secondes
// le temps pour creer le vecteur 2d pour 4096 est ~0.3 secondes

int nbPtsEntiersCubeExact(int R2,int R2maxp=0);
int nbPtsEntiersCarreExact(int R2,int R2maxp=0);
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

#endif
