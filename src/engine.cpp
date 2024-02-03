#include "engine.h"
#include "utils.h"
#include <cassert>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void check_status_shader_program(uint shader_program){
   int res; 
   char info[512];
   glGetProgramiv(shader_program, GL_LINK_STATUS, &res);
   if (!res){
      glGetProgramInfoLog(shader_program, 512, NULL, info);
      utils::error("shaders", info);
   } else utils::log("linked shader program", std::to_string(shader_program));
}


void frame_buffer_size(GLFWwindow* window, int width, int height){
   glViewport(0, 0, width, height);
   utils::log("changed view port");
}

/******************************************************/
//                    SHADER                          //
/******************************************************/

Shader::Shader(const char src_vertex[], const char src_fragment[]){
   uint vertex, fragment;
   create_shader(&vertex, src_vertex, GL_VERTEX_SHADER);
   create_shader(&fragment, src_fragment, GL_FRAGMENT_SHADER);

   ID = glCreateProgram();
   glAttachShader(ID, vertex);
   glAttachShader(ID, fragment);
   glLinkProgram(ID);
   check_status_shader_program(ID);
   glDeleteShader(vertex);
   glDeleteShader(fragment);
}

Shader::~Shader(){
   glDeleteProgram(ID);
   utils::log("deleted shader", std::to_string(ID));
}

void Shader::create_shader(uint* shader, const char src[], GLuint type){
   int res; char info[512];
   *shader = glCreateShader(type); 
   std::string shader_code= utils::get_source_from_file(src);
   const char* source = shader_code.c_str();
   glShaderSource(*shader, 1, &source, NULL); 
   glCompileShader(*shader);
   glGetShaderiv(*shader, GL_COMPILE_STATUS, &res);
   if (!res) {
      glGetShaderInfoLog(*shader, 512, NULL, info);;
      utils::error(src, info);
   } else utils::log(src, "created shader");
}

void Shader::use(fvec4 color){
   int location = glGetUniformLocation(ID, "color");
   assert(location != -1);
   glUseProgram(ID);
   glUniform4f(location, color.r, color.g, color.b, color.apacity);
}

void Shader::use(){
   glUseProgram(ID);
}

void Shader::set_float(std::string &name, float x){
   int location = glGetUniformLocation(ID, name.c_str());
   assert(location != -1);
   glUniform1f(location, x); 

}
void Shader::set_int(std::string &name, int x){
   int location = glGetUniformLocation(ID, name.c_str());
   assert(location != -1);
   glUniform1i(location, x); 
}


/******************************************************/
//                    TEXTURE                         //
/******************************************************/

Texture::Texture(const char texture_path[]){
   glGenTextures(1, &ID);
   glBindTexture(GL_TEXTURE_2D, ID);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   int width, height, color_channels;
   unsigned char *data = stbi_load(texture_path, &width, &height, &color_channels, 0);
   if (!data){ 
      utils::error(texture_path, "couldn't load data");
      assert(data);
   }
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
   glGenerateMipmap(GL_TEXTURE_2D);
   stbi_image_free(data);
   utils::log(texture_path,"created texture");
}

Texture::~Texture(){
   glDeleteTextures(1, &ID);
   utils::log("deleted texture", std::to_string(ID));
}

void Texture::use(uint ID2){
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, ID);

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, ID2);
   
}
void Texture::use(){
   glBindTexture(GL_TEXTURE_2D, ID);
}

/******************************************************/
//                    INPUT                           //
/******************************************************/

void Input::get_input(GLFWwindow* window, int key, int scancode, int action, int mods){
   switch(key){
      case GLFW_KEY_ESCAPE:
         glfwSetWindowShouldClose(window, true);
         utils::log("close the program");
         break;
      case GLFW_KEY_P:
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         break;
      case GLFW_KEY_F:
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         break;
   }
}

