#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sys/types.h>
#define LEN(x) sizeof(x)/sizeof(x[0])

void frame_buffer_size(GLFWwindow* window, int width, int height);
void check_status_shader_program(uint shader_program);

struct fvec4{
   float r;
   float g;
   float b;
   float apacity;
};

class Shader {
   public:
      Shader(const char src_vertex[], const char src_fragment[]);
      ~Shader();

   public:
      uint ID;
      void use(fvec4 color);
      void use();

   private:
      void create_shader(uint* shader, const char src[], GLuint type);
};


#endif
