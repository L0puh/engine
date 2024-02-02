#include "utils.h"
#include <fstream>
#include <sstream>

namespace utils{
   std::string get_source_from_file(const char filename[]){
      std::string source;
      std::ifstream file;
      file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      try {
         file.open(filename);
         std::stringstream stream_file; 
         stream_file << file.rdbuf();
         file.close();
         source = stream_file.str();
      }
      catch(std::ifstream::failure e){
         utils::error("cant' open the file", filename);
      }
      return source;
   }
}
