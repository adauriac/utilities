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
