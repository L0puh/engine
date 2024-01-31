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
   "  FragColor=vec4(1.0f, 0.2f, 0.2f, 1.0f);\n"
   "}\0";

const char *f_shader_source2 = "#version 330 core\n"
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

// SHADERS: 
   uint vertex_shader;
   vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertex_shader, 1, &v_shader_source, NULL);
   glCompileShader(vertex_shader);

   int res;
   char info[512];
   glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &res);
   if (!res){
      glGetShaderInfoLog(vertex_shader, 512, NULL, info);
      utils::log(info);
   } else utils::log("complied vertex shader");


   uint fragment_shader, fragment_shader2;
   fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment_shader, 1, &f_shader_source, NULL);
   glCompileShader(fragment_shader);

   fragment_shader2 = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment_shader2, 1, &f_shader_source2, NULL);
   glCompileShader(fragment_shader2);

   glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &res);
   if (!res){
      glGetShaderInfoLog(vertex_shader, 512, NULL, info);
      utils::error(info);
   } else utils::log("complied fragment shader");

   uint shader_program, shader_program2;
   shader_program = glCreateProgram();
   glAttachShader(shader_program, vertex_shader);
   glAttachShader(shader_program, fragment_shader);
   glLinkProgram(shader_program);
  
   shader_program2 = glCreateProgram();
   glAttachShader(shader_program2, vertex_shader);
   glAttachShader(shader_program2, fragment_shader2);
   glLinkProgram(shader_program2);
   glGetProgramiv(shader_program2, GL_LINK_STATUS, &res);
   if (!res){
      glGetProgramInfoLog(shader_program, 512, NULL, info);
      utils::error(info);
   } else utils::log("linked shader program");

// VERTICIES: 
   float vertices[] = {
      0.0f, -0.5f, 0.0f,  // left
      0.9f, -0.5f, 0.0f,  // right
      0.45f, 0.5f, 0.0f   // top
   };
   float vertices3[] = {
     -0.9f, -0.5f, 0.0f,  // left
     -0.0f, -0.5f, 0.0f,  // right
     -0.45f, 0.5f, 0.0f,  // top
   };
   float vertices2[] = {
      0.5f,  0.5f,  0.0f,
      0.5f, -0.5f,  0.0f,
     -0.5f, -0.5f,  0.0f,
     -0.5f,  0.5f,  0.0f,
   };

   uint indices[] = {
      0, 1, 3,
      1, 2, 3
   };

   uint VBOs[3], VAOs[3], EBO;
   glGenVertexArrays(3, VAOs);
   glGenBuffers(3, VBOs);
   glGenBuffers(1, &EBO);
   
   //TRIANGLE 1
   glBindVertexArray(VAOs[0]);
   glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   //TRIANGLE 2
   glBindVertexArray(VAOs[1]);
   glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   // SQUARE
   glBindVertexArray(VAOs[2]);
   glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);


   utils::log("setup openGl");

   glClearColor(0.25f, 0.4f, 0.5f, 1.0f);
   bool pressed = false, pressed2 = false;


   while(!glfwWindowShouldClose(win)){
      if (glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS){
         utils::log("triangle");
         pressed = true;
         pressed2 = false;
      } 
      if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
         utils::log("square");
         pressed = false;
         pressed2 = true;
      }
      if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS){
         glfwSetWindowShouldClose(win, true);
         utils::log("close the program");
      } 
      if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS){
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         utils::log("fill");
      } 

      if (pressed) {
         //draw a triangle
         glUseProgram(shader_program);
         glBindVertexArray(VAOs[0]);
         glDrawArrays(GL_TRIANGLES, 0, 3);
         glBindVertexArray(VAOs[1]);
         glDrawArrays(GL_TRIANGLES, 0, 3);
         glBindVertexArray(0);
      

      } else if (pressed2) {
         //draw a square
         glUseProgram(shader_program2);
         glBindVertexArray(VAOs[2]);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
         glBindVertexArray(0);
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
