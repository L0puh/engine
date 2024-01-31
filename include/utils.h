#ifndef UTILS_H
#define UTILS_H

#include <string>
namespace utils{
   inline void log(std::string x){printf("[+] %s\n", x.c_str());}
   inline void error(std::string x){printf("[-] error: %s\n", x.c_str());}
   inline void error(std::string y, std::string x){printf("[-] error in %s: %s\n",y.c_str(), x.c_str());}
}
#endif
