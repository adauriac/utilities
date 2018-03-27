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
