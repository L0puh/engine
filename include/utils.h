#ifndef UTILS_H
#define UTILS_H
#include <string>

namespace utils{
   std::string get_source_from_file(const char filename[]);
   inline void log(std::string x){printf("[+] %s\n", x.c_str());}
   inline void log(std::string x, std::string y){printf("[+] %s %s\n", x.c_str(), y.c_str());}
   inline void error(std::string x){printf("[-] error: %s\n", x.c_str());}
   inline void error(std::string y, std::string x){printf("[-] error in %s: %s\n",y.c_str(), x.c_str());}
}
#endif
