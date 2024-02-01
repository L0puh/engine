#include "engine.h"
#include "utils.h"
#include <fstream>
#include <sstream>

void frame_buffer_size(GLFWwindow* window, int width, int height){
   glViewport(0, 0, width, height);
   utils::log("changed view port");
}

static std::string get_source_from_file(const char filename[]){
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
   return source;
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
}
void Shader::create_shader(uint* shader, const char src[], GLuint type){
   int res; char info[512];
   *shader = glCreateShader(type); 
   std::string shader_code= get_source_from_file(src);
   const char* source = shader_code.c_str();
   glShaderSource(*shader, 1, &source, NULL); 
   glCompileShader(*shader);
   glGetShaderiv(*shader, GL_COMPILE_STATUS, &res);
   if (!res) {
      glGetShaderInfoLog(*shader, 512, NULL, info);;
      utils::error(src, info);
   } else utils::log(src);
}

void Shader::use(){
   glUseProgram(ID);
}



