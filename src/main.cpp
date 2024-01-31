#include <cstdio>
#include <sys/types.h>
#include "engine.h"
#include "utils.h"

const int WIDTH = 400;
const int HEIGHT = 400;

const char *v_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *f_shader_source = "#version 330 core\n"
   "out vec4 FragColor;\n"
   "void main()\n"
   "{\n"
   "  FragColor=vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
   "}\0";


int main(){
   GLFWwindow* win;
   if (!glfwInit()){
      utils::error("init glfw");
      return 0;
   }

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   win = glfwCreateWindow(WIDTH, HEIGHT, "win", 0, 0); 
   if (win == NULL){
      utils::error("open window");
      glfwTerminate();
      return 0;
   }
   glfwMakeContextCurrent(win);
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      utils::error("init glad");
      glfwTerminate();
      return 0;
   }

   glfwSetFramebufferSizeCallback(win, frame_buffer_size);
   glClearColor(0, 0, 0, 0);


   float vertices[] = {
      // x     y      z 
      -0.5f,  -0.5f,  0.0f,
       0.5f,  -0.5f,  0.0f,
       0.0f,   0.5f,  0.0f
   };
 

   //vertext buffer objest (manage memory)
   uint VBO, VAO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);

   glBindVertexArray(VAO);
   // bind vbo to array_buffer type
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   //copy vericies to the buffer 
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


   uint vertex_shader;
   vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   //attach the shader to object
   glShaderSource(vertex_shader, 1, &v_shader_source, NULL);
   glCompileShader(vertex_shader);

   int res;
   char info[512];
   glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &res);
   if (!res){
      glGetShaderInfoLog(vertex_shader, 512, NULL, info);
      utils::log(info);
   } else utils::log("complied vertex shader");


   uint fragment_shader;
   fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment_shader, 1, &f_shader_source, NULL);
   glCompileShader(fragment_shader);

   glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &res);
   if (!res){
      glGetShaderInfoLog(vertex_shader, 512, NULL, info);
      utils::error(info);
   } else utils::log("complied fragment shader");

   uint shader_program;
   shader_program = glCreateProgram();
   glAttachShader(shader_program, vertex_shader);
   glAttachShader(shader_program, fragment_shader);
   glLinkProgram(shader_program);
   
   glGetProgramiv(shader_program, GL_LINK_STATUS, &res);
   if (!res){
      glGetProgramInfoLog(shader_program, 512, NULL, info);
      utils::error(info);
   } else utils::log("linked shader program");


   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   utils::log("setup openGl");

   glClearColor(0.25f, 0.4f, 0.5f, 1.0f);
   bool pressed = false;

   while(!glfwWindowShouldClose(win)){
      if (glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS){
         utils::log("enter pressed");
         pressed = true;
      } 
      if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
         pressed = false;
      }
      if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS){
         glfwSetWindowShouldClose(win, true);
         utils::log("close the program");
      } 
      if (pressed) {
         glUseProgram(shader_program);
         glBindVertexArray(VAO);
         glDrawArrays(GL_TRIANGLES, 0, 3);
      }
      glfwPollEvents();
      glfwSwapBuffers(win);
      glClear(GL_COLOR_BUFFER_BIT);
   }
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader);
   glfwDestroyWindow(win);
   glfwTerminate();

   return 0;
}
