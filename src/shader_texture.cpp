#include "engine.h"
#include "utils.h"
#include <glm/gtc/type_ptr.hpp>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
   } else utils::log(src, "load shader");
}

int Shader::get_location(const char name[]){
   int location;
   if (cached_locations[name])
      return cached_locations[name];

   location = glGetUniformLocation(ID, name);
   if (location == -1){
      utils::error("location", name);
      assert(location != -1);
   }
   cached_locations[name] = location;
   return location;
}

void Shader::use(){
   glUseProgram(ID);
}

void Shader::set_float(const char name[], float x){
   int location = get_location(name);
   glUniform1f(location, x); 

}

void Shader::set_int(const char name[], int x){
   int location = get_location(name);
   glUniform1i(location, x); 
}

void Shader::set_vec4(const char name[], glm::vec4 vec){
   int location = get_location(name);
   glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::set_matrix4fv(const char name[], glm::mat4 data){
   int location = get_location(name);
   glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

/******************************************************/
//                    TEXTURE                         //
/******************************************************/

Texture::Texture(const char texture_path[], int img_type, GLenum mode){
   glGenTextures(1, &ID);
   glBindTexture(GL_TEXTURE_2D, ID);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   int width, height, color_channels;

   stbi_set_flip_vertically_on_load(1);
   unsigned char *data = stbi_load(texture_path, &width, &height, &color_channels, 0);
   if (!data){ 
      utils::error(texture_path, "couldn't load data");
      assert(data);
   } 

   switch(img_type){
      case PNG:
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
         break;
      case JPG:
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
         break;
   }
   glGenerateMipmap(GL_TEXTURE_2D);
   stbi_image_free(data);
   utils::log(texture_path,"load texture");
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

void check_status_shader_program(uint shader_program){
   int res; 
   char info[512];
   glGetProgramiv(shader_program, GL_LINK_STATUS, &res);
   if (!res){
      glGetProgramInfoLog(shader_program, 512, NULL, info);
      utils::error("shaders", info);
   } else utils::log("linked shader program", std::to_string(shader_program));
}

