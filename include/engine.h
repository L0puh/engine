#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sys/types.h>


void frame_buffer_size(GLFWwindow* window, int width, int height);
void check_status_shader_program(uint shader_program);

class Vertex_shader {
   public:
      uint vertex_shader;
      char* filename;
      Vertex_shader(const char source_file[]);
      ~Vertex_shader();
      void create_shader(uint shader_program);


};

class Fragment_shader {
   public:
      uint fragment_shader;
      char* filename;
      Fragment_shader(const char source_file[]);
      ~Fragment_shader();
      void create_shader(uint shader_program);

};

#endif
