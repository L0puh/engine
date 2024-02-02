#include <cstdio>
#include <sys/types.h>
#include "engine.h"
#include "utils.h"

#define LEN(x) sizeof(x)/sizeof(x[0])
const int WIDTH = 400;
const int HEIGHT = 400;


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

   Shader s1("../shaders/shader.vert", "../shaders/shader.frag");
   Shader s2("../shaders/shader.vert", "../shaders/shader2.frag");


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
      0.3f, -0.3f, 0.0f,   //0
      0.0f, -0.6f, 0.0f,   //1
      -0.6f, -0.6f, 0.0f,  //2
      -0.3f, 0.3f, 0.0f,   //3
      0.3f, 0.3f, 0.0f,    //4
      -0.6f, 0.0f, 0.0f,   //5
      0.0f, 0.0f, 0.0f     //6
   };

   uint indices[] = {
      3, 4, 5,
      5, 4, 6,
      5, 2, 1,
      5, 6, 1,
      
   };
   uint indices2[] = {
      6, 1, 0,
      6, 4, 0
   };
   uint VBOs[4], VAOs[4], EBOs[2];
   glGenVertexArrays(4, VAOs);
   glGenBuffers(4, VBOs);
   glGenBuffers(2, EBOs);
   
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

   // CUBE 
   glBindVertexArray(VAOs[2]);
   glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) , indices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   glBindVertexArray(VAOs[3]);
   glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2) , indices2, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); 
   glEnableVertexAttribArray(0);

   utils::log("setup openGl");

   glClearColor(0.25f, 0.4f, 0.5f, 1.0f);
   bool pressed = false, pressed2 = false, filled = true;

   while(!glfwWindowShouldClose(win)){
      if (glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS){
         utils::log("triangle");
         pressed = true;
         pressed2 = false;
      } 
      if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
         utils::log("cude");
         pressed = false;
         pressed2 = true;
      }
      if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS){
         glfwSetWindowShouldClose(win, true);
         utils::log("close the program");
      } 
      if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS){
         if (filled) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            filled = false;
         } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            filled=true;
            utils::log("fill");
         }
      }  

      if (pressed) {
         //draw a triangle
         s1.use();
         glBindVertexArray(VAOs[0]);
         glDrawArrays(GL_TRIANGLES, 0, 3);
         glBindVertexArray(VAOs[1]);
         glDrawArrays(GL_TRIANGLES, 0, 3);
         glBindVertexArray(0);

      } else if (pressed2) {
         //draw a square
         s2.use();
         glBindVertexArray(VAOs[2]);
         glDrawElements(GL_TRIANGLES, LEN(indices), GL_UNSIGNED_INT, 0);
         s1.use();
         glBindVertexArray(VAOs[3]);
         glDrawElements(GL_TRIANGLES, LEN(indices), GL_UNSIGNED_INT, 0);
         glBindVertexArray(0);
      }
      glfwPollEvents();
      glfwSwapBuffers(win);
      glClear(GL_COLOR_BUFFER_BIT);
   }
   glfwDestroyWindow(win);
   glfwTerminate();

   return 0;
}
