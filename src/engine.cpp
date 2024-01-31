#include "engine.h"
#include "utils.h"
#include <fstream>
#include <sstream>

void frame_buffer_size(GLFWwindow* window, int width, int height){
   glViewport(0, 0, width, height);
   utils::log("changed view port");
}

static const char* get_source_from_file(const char filename[]){
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
      utils::error("cant' open the file", filename);
   }
   const char* src = source.c_str();
   utils::log(src);
   return src;
}

void check_status_shader_program(uint shader_program){
   int res; 
   char info[512];
   glGetProgramiv(shader_program, GL_LINK_STATUS, &res);
   if (!res){
      glGetProgramInfoLog(shader_program, 512, NULL, info);
      utils::error("shaders", info);
   } else utils::log("linked shader program");
}

/******************************************************/
//                   VERTEX SHADER                    //

Vertex_shader::Vertex_shader(const char source_file[]){
   filename = (char*)source_file;
   vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   const char* shader_source = get_source_from_file(source_file);
   glShaderSource(vertex_shader, 1, &shader_source, NULL);
}
Vertex_shader::~Vertex_shader(){
   glDeleteShader(vertex_shader);
}
void Vertex_shader::create_shader(uint shader_program){
   glCompileShader(vertex_shader);
   int res;
   char info[512];
   glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &res);
   if (!res){
      glGetShaderInfoLog(vertex_shader, 512, NULL, info);
      utils::error(filename, info);
   } else utils::log("complied vertex shader");
   glAttachShader(shader_program, vertex_shader);
}

/******************************************************/
//                  FRAGMENT SHADER                  //

Fragment_shader::Fragment_shader(const char source_file[]){
   filename = (char*)source_file;
   fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
   const char* shader_source = get_source_from_file(source_file);
   glShaderSource(fragment_shader, 1, &shader_source, NULL);
}
Fragment_shader::~Fragment_shader(){
   glDeleteShader(fragment_shader);
}
void Fragment_shader::create_shader(uint shader_program){
   int res; 
   char info[512];
   glCompileShader(fragment_shader);
   glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &res);
   if (!res){
      glGetShaderInfoLog(fragment_shader, 512, NULL, info);
      utils::error(filename, info);
   } else utils::log("complied fragment shader");
   glAttachShader(shader_program, fragment_shader);
}
