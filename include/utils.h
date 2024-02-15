#ifndef UTILS_H
#define UTILS_H
#include "glm/ext/vector_float3.hpp"
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>

namespace utils{

   void frame_buffer_size(GLFWwindow* window, int width, int height);
   std::pair<int, int> get_view_point(GLFWwindow *window);
   std::string get_source_from_file(const char filename[]);
   void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLuint severity,
                                   GLsizei length, const GLchar* message, const GLvoid* userParam );

   inline void log(std::string x){ printf("[+] %s\n", x.c_str()); }
   inline void log(std::string x, std::string y){ printf("[+] %s %s\n", x.c_str(), y.c_str()); }
   inline void error(std::string x){printf("[-] error: %s\n", x.c_str());}
   inline void error(std::string y, std::string x){printf("[-] error in %s: %s\n",y.c_str(), x.c_str());}

   class Debug {
      
      public:
         Debug(GLFWwindow *window);
         ~Debug();
         void new_frame();
         void render();
         void draw(bool *tg, bool *cb, bool *fl, bool *mode, float *fov, glm::vec3 *pos, glm::vec3 camera_pos);
         bool is_clicked();
         bool is_hovered();
      private:
         bool clicked, hovered;
   };
}
#endif
