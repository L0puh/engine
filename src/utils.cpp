#include "utils.h"

#include <cassert>
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
         utils::error("open file", filename);
      }
      return source;
   }
   void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLuint severity,
                                   GLsizei length, const GLchar* message, const GLvoid* userParam ){
     if (type == GL_DEBUG_TYPE_ERROR){
        utils::error(message, std::to_string(type));
     }
     assert(type != GL_DEBUG_TYPE_ERROR);
   }
}
