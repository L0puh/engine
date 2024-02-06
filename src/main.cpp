#include "engine.h"
#include "glm/ext/matrix_transform.hpp"
#include "utils.h"

#include <GLFW/glfw3.h>
#include <cassert>
#include <string>
#include <sys/types.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DEBUG_MODE 

const int WIDTH  = 300;
const int HEIGHT = 300;


int main(){

// WINDOW: 
   GLFWwindow* win =  init_window(WIDTH, HEIGHT);
   assert(win != 0);

   #ifdef DEBUG_MODE
      glEnable(GL_DEBUG_OUTPUT);
      glDebugMessageCallback(utils::message_callback, 0);
      utils::log("DEBUG MODE ON");
   #endif

// SHADERS: 

   Shader s1("../shaders/shader.vert", "../shaders/shader.frag");      // with uniform
   Shader s2("../shaders/shader.vert", "../shaders/shader2.frag");    
   Shader s3("../shaders/shader2.vert", "../shaders/shader_tex.frag"); // with texture

// TEXTURES:

   Texture tx  ("../textures/wall.jpg", JPG);
   Texture tx2 ("../textures/face.png", PNG);

// VERTICIES: 

   float vertices[] = {
   //  x     y       z    colors 
      0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  
      0.9f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 
      0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f   
   };

   float vertices3[] = {
      // x   y     z          texture
       0.0f, 0.8f, 0.0f,   2.0f, 0.0f,
    -0.8f,-0.8f, 0.0f,     0.0f, 0.0f,
     0.8f,-0.8f, 0.0f,     0.0f, 2.0f,
   };

   float vertices2[] = {
     // x   y     z        R     G    B
      0.3f, -0.3f, 0.0f,  0.0f, 0.0f, 1.0f,  //0
      0.0f, -0.6f, 0.0f,  0.0f, 0.0f, 1.0f,  //1
      -0.6f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  //2
      -0.3f, 0.3f, 0.0f,  1.0f, 0.0f, 0.0f,  //3
      0.3f, 0.3f, 0.0f,   1.0f, 0.0f, 0.0f,  //4
      -0.6f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  //5
      0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,  //6
   };

   uint indices[] = {
      3, 4, 5,
      5, 4, 6,
      5, 2, 1,
      5, 6, 1,
      6, 1, 0,
      6, 4, 0
   };



// BUFFERS: 
   uint VBOs[4], VAOs[4], EBOs[2];
   glGenVertexArrays(4, VAOs);
   glGenBuffers(4, VBOs);
   glGenBuffers(2, EBOs);
   
   //TRIANGLE 1
   glBindVertexArray(VAOs[0]);
   glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   //position
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   //color 
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
   glEnableVertexAttribArray(1);

   //TRIANGLE 2
   glBindVertexArray(VAOs[1]);
   glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
   //position
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   //texture
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
   glEnableVertexAttribArray(1);

   // CUBE 
   glBindVertexArray(VAOs[2]);
   glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) , indices, GL_STATIC_DRAW);
   // pos
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   //color 
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
   glEnableVertexAttribArray(1);


   glClearColor(0.25f, 0.4f, 0.5f, 1.0f);
   bool triangle_pressed = false, cube_pressed = true;
   float move = 0;

   s3.use();
   s3.set_int("my_texture", 0);
   s3.set_int("my_texture2", 1);
  

// MAIN LOOP:
   while(!glfwWindowShouldClose(win)){
      //moving  
      if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS){
         if (move-0.04f > -1.0f)
            move-=0.04f;
      } else if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS){
         if (move+0.04f < 1.0f){
            move+=0.04f;
         }
      }
      if (triangle_pressed) {

         //matricies (rotate an object over time)
         glm::mat4 trans = glm::mat4(1.0f);
         trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -0.3f));
         trans = glm::scale(trans, glm::vec3(0.2, 0.3, 0.3));
         trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));

         //draw triangles (with two textures)
         tx.use(tx2.ID);

         s3.use();
         s3.set_matrix4fv("transform", trans);
         glBindVertexArray(VAOs[1]);
         glDrawArrays(GL_TRIANGLES, 0, 3);
         glBindVertexArray(0);
         

      } else if (cube_pressed) {
         //draw a cube 
         s2.use();
         s2.set_float("move", move);

         glBindVertexArray(VAOs[2]);
         glDrawElements(GL_TRIANGLES, LEN(indices), GL_UNSIGNED_INT, 0);

         s1.use();
         s1.set_vec4("color", {0.0f, 0.0f, 0.0f, 1.0f});
         s1.set_float("move", move);
         glDrawElements(GL_LINE_LOOP, LEN(indices), GL_UNSIGNED_INT, 0);
      }

      if (glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS){
         if (triangle_pressed) {
            triangle_pressed = false;
            cube_pressed = true;
         }
      } else if (glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS){
         if (cube_pressed) {
            triangle_pressed = true;
            cube_pressed = false;
         }
      }
      glfwSetKeyCallback(win, Input::get_input);
      glfwPollEvents();
      glfwSwapBuffers(win);
      glClear(GL_COLOR_BUFFER_BIT);
   }

   glfwDestroyWindow(win);
   glfwTerminate();

   return 0;
}

