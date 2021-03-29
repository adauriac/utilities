#ifndef _JC
#define _JC
#include <iostream>
using namespace std;
#ifndef _ENREGH
#define _ENREGH

#define _LINE_MAX 1024
#define _BLOCK_SIZE (10*_LINE_MAX)
#define _NBFIELDS_MAX 50

class enreg
{
public:
  static const int MAXLENGTLINE = 2048;
  static const int BLOCKSIZE = 4096;
  int ok;
  string status;
  string comments;
  int nline;
  int ncol;
  void **cols;
  string type[MAXLENGTLINE];
  string filename;
  
  enreg(char *format,char *filename,int n=0,int fakefile=0);
  ~enreg();
  void realloue(int n);
  // toutes les colonnes sont reallouees a la nouvelle valeur n
};

// format : i8,i16,i32,i64,f32,f64
// i/f ==> int/float, 8,16,32,64 nombre de bits
// filename = nom de la file a lire 
//            la chaine stdin veut dire l'entree standard !
// n numero de l'enregistrement a considerer un enregistement est
// fini losrque l'on rencontre deux lines vide de suite. 
// les commentaire inclus TOUS les commentaires precedents

#endif
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _FILAGE_H
#define _FILAGE_H

#define _TAG 654646

#define _RETURN_OK                  0
#define _INVALID_TAG                1
#define _CANT_READ_SIZE_COMMENTS    2
#define _CANT_READ_TAG              3
#define _CANT_OPEN_FILE             4
#define _CANT_WRITE_SIZE_COMMENTS   5
#define _CANT_WRITE_TAG             6
#define _CANT_WRITE_COMMENTS        7
#define _CANT_WRITE_DATA            8
#define _CANT_ALLOCATE_COMM         9
#define _CANT_READ_COMMENTS         10
#define _CANT_STATE_FILE            11
#define _CANT_ALLOCATE_BUF          12
#define _CANT_READ_DATA             13

int writeinfile(char *filename,char *comments,char *buf,long long int *size_in_bytes);
int writeinfile_withoutclosing(char *filename,char *comments,char *buf,long long int *size_in_bytes,FILE **f);
int readfromfile(char *filename,char **comments,char **buf,long long int *sizebuf_in_bytes);
int readcommentsfromfile(char *filename,char **comments);
int appendtofile(char *filename,char *buf,long long int *size_in_bytes);

 int isbinaryfile(char *filename);  // 1 only the secunf long long ust the tag
int get_string_from_string(char *string,char *name,char **r);
int get_double_from_string(char *string,char *name,double *r);
int get_LL_from_string(char *string,char *name,long long int *r);
// usage: if (get_double_from_string(mystring,"x",&r))exit(0) ;
//        lit une variable x (...x= 8.1...) et sort si pas la

#endif

#ifdef __cplusplus
};
#endif

/*
ecrit dans le fichier filename, precede des commentaires comments,
le buffer buf de taille size_in_bytes (on passe l'adresse pour compatibilite 
fortran)

lit dans le fichier filename les commentaires comments, puis
le buffer buf, et affacte sa taille size_in_bytes (on passe l'adresse pour compatibilite 
fortran)
*/
#ifndef _FILAGECPP
#define _FILAGECPP
#include  <string>
#include  <stdio.h>
using namespace std;
string getCommFromAsciiFile(string filename);
#endif
/* INCLUDED: rand() cgl() R250() R1279() VERSION FONCTION
(JC Angles d'Auriac Decembre 1993, Voir rapport de convention pour les details)
*/
// modifier par seprartion .c et .h le mar oct  8 11:08:57 CEST 2002  
// modifieMon Jun 29 10:56:46 CEST 2009: utilise /dev/random et rend la vameur utilisee 
#ifdef __cplusplus
extern "C" {
#endif

unsigned int R250(void);
void initR250(int *seed); // -1 or 0  means read /dev/urandom
void initR1279(int *seed); // -1 or 0  means read /dev/urandom
void initR1279_2(int *seed1,int *seed2); //  0  means read /dev/urandom
// soit on initialise avec initR1279(seed) soit avec initR1279_2(seed1,seed2)
unsigned int R1279(void);
void initcgl(unsigned int x);
unsigned int cgl(void);
double Gauss();
double U1279();
int equivR1279(int x);
unsigned int uniform1279(unsigned int borneSup); // uniforme \in [0,borneSup[
unsigned int *tabR1279Addresse();
// initR1279(seed) et inintR1279(equiv(seed)) sont equivalent
//initR1279_2(seed1,seed2 equivalent a initR1279_2(equiv(seed1),equiv(seed2))
// ainsi qu'a initR1279_2(equiv(seed1),seed2) et initR1279_2(seed1,equiv(seed2))
#define _XRND   (((double)R1279())/((double)((unsigned int)(-1))))

void shuffleR1279(unsigned int *a,unsigned int n);
// entree a est un tableau de n entiers
// en sortie a[i] est l'image de i par la permutation

#ifdef __cplusplus
};
#endif

#ifndef _HASARDCPP
#define _HASARDCPP
class rndclass
{
public:
  unsigned int tabR1279[1279];
  unsigned int seed;
  unsigned int cpt;
  unsigned int VarCgl;
  unsigned int Var1R1279;
  unsigned int Var2R1279;

  rndclass(unsigned int seed,int prestep=0);
  void reset(unsigned int seed,int prestep=0);
  unsigned int R1279();
  unsigned int cgl();
};

#endif // _HASARDCPP
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
#endif
