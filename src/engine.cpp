#include "engine.h"
#include "utils.h"
#include <string>

void frame_buffer_size(GLFWwindow* window, int width, int height){
   glViewport(0, 0, width, height);
   utils::log("changed view port");
}

void check_status_shader_program(uint shader_program){
   int res; 
   char info[512];
   glGetProgramiv(shader_program, GL_LINK_STATUS, &res);
   if (!res){
      glGetProgramInfoLog(shader_program, 512, NULL, info);
      utils::error("shaders", info);
   } else utils::log("linked shader program", std::to_string(shader_program));
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

void Shader::use(){
   glUseProgram(ID);
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



