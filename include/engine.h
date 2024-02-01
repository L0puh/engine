#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sys/types.h>

void frame_buffer_size(GLFWwindow* window, int width, int height);
void check_status_shader_program(uint shader_program);

class Shader {
   public:
      Shader(const char src_vertex[], const char src_fragment[]);
      ~Shader();

   public:
      uint ID;
      void use();

   private:
      void create_shader(uint* shader, const char src[], GLuint type);
};


#endif
