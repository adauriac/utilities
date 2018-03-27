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

