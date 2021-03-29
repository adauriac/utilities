
#include "filagecpp.h"

string getCommFromAsciiFile(string filename)
{
  FILE *ft=fopen((char*)filename.c_str(),"r");
  if (ft==NULL)
    return "";
  string ans="";
  for(;;)
    {
      char line[1024];
      if (fgets(line,1023,ft)==NULL)
        break;
      if (line[0]!='#')
        break;
      ans += string(line);
    }
  fclose(ft);
  return ans;
}     // FIN getCommFromAsciiFile(string filename)
// **********************************************************
