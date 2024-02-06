#ifndef ENGINE_H
#define ENGINE_H

#include "glm/ext/matrix_float4x4.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <string>
#include <sys/types.h>
#include <unordered_map>
#define LEN(x) sizeof(x)/sizeof(x[0])

void check_status_shader_program(uint shader_program);
void frame_buffer_size(GLFWwindow* window, int width, int height);
GLFWwindow* init_window(const int WIDTH, const int HEIGHT);

enum Types {
   PNG,
   JPG
};

struct fvec4{
   float x;
   float y;
   float z;
   float w;
};

class Shader {
   public:
      Shader(const char src_vertex[], const char src_fragment[]);
      ~Shader();

   public:
      uint ID;
      void use();

      void set_float(const char name[], float x);
      void set_int(const char name[], int x);
      void set_matrix4fv(const char name[], glm::mat4 data);
      void set_vec4(const char name[], fvec4 vec);

   private:
      std::unordered_map<std::string, int> cached_locations;

      void create_shader(uint* shader, const char src[], GLuint type);
      int get_location(const char name[]);
};

class Texture {
   public:
      Texture(const char texture_path[], int type);
      ~Texture();
   public:
      uint ID;
      void use(uint ID2);
      void use();
};

class Input {
   public:
      static void get_input(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
