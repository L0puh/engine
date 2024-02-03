#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <string>
#include <sys/types.h>
#define LEN(x) sizeof(x)/sizeof(x[0])

void check_status_shader_program(uint shader_program);
void frame_buffer_size(GLFWwindow* window, int width, int height);

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

      void set_float(std::string &name, float x);
   private:
      void create_shader(uint* shader, const char src[], GLuint type);
};

class Texture {
   public:
      Texture(const char texture_path[]);
      ~Texture();
   public:
      uint ID;
      void use();
};

class Input {
   public:
      static void get_input(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
