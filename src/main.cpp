#include "engine.h"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <sys/types.h>
#include "utils.h"

const int WIDTH = 300;
const int HEIGHT = 300;


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

   Shader s1("../shaders/shader.vert", "../shaders/shader.frag");     // with uniform
   Shader s2("../shaders/shader.vert", "../shaders/shader2.frag");    
   Shader s3("../shaders/shader2.vert", "../shaders/shader_tex.frag"); //with texture

// TEXTURES:

   Texture tx("../textures/wall.jpg");
   Texture tx2("../textures/face.png");

// VERTICIES: 

   float vertices[] = {
   //  x     y       z    colors 
      0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
      0.9f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left 
      0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top
   };

   float vertices3[] = {
      // x   y     z          texture
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   
        0.5f, -0.5f, 0.0f,   1.0f, -1.0f,    // FIXME
        -0.5f, -0.5f, 0.0f,  -1.0f, -1.0f,   
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

   utils::log("setup openGl");

   glClearColor(0.25f, 0.4f, 0.5f, 1.0f);
   bool triangle_pressed = false, cube_pressed = true;
   float move = 0;
   std::string name = "move";

   std::string tx_name = "my_texture", tx_name2 = "my_texture2";
   s3.use();
   s3.set_int(tx_name, 0);
   s3.set_int(tx_name2, 1);
  

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
         //draw triangles
         tx.use(tx2.ID);
         
         s3.use();
         glBindVertexArray(VAOs[1]);
         glDrawArrays(GL_TRIANGLES, 0, 3);
         glBindVertexArray(0);
         
         /* s2.set_float(name, move); */
         /* s2.use(); */
         /* glBindVertexArray(VAOs[0]); */
         /* glDrawArrays(GL_TRIANGLES, 0, 3); */

      } else if (cube_pressed) {
         //draw a cube 
         s2.set_float(name, move);
         s2.use();
         glBindVertexArray(VAOs[2]);
         glDrawElements(GL_TRIANGLES, LEN(indices), GL_UNSIGNED_INT, 0);

         s1.set_float(name, move);
         s1.use({0.0f, 0.0f, 0.0f, 1.0f});
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

