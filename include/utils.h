#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>

namespace utils{

   std::pair<int, int> get_view_point(GLFWwindow *window);
   std::string get_source_from_file(const char filename[]);
   void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLuint severity,
                                   GLsizei length, const GLchar* message, const GLvoid* userParam );

   inline void log(std::string x){ printf("[+] %s\n", x.c_str()); }
   inline void log(std::string x, std::string y){ printf("[+] %s %s\n", x.c_str(), y.c_str()); }
   inline void error(std::string x){printf("[-] error: %s\n", x.c_str());}
   inline void error(std::string y, std::string x){printf("[-] error in %s: %s\n",y.c_str(), x.c_str());}
}
#endif
