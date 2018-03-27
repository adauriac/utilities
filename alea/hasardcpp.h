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
