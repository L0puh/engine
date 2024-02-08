#include "engine.h"
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

   Shader s1("../shaders/shader.vert",       "../shaders/shader.frag");             // with uniform
   Shader s2("../shaders/shader.vert",       "../shaders/shader2.frag");    
   Shader s3("../shaders/shader2.vert",      "../shaders/shader_tex.frag");        // with texture
   Shader s4("../shaders/shader_floor.vert", "../shaders/shader_floor.frag");

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
      0.3f, -0.3f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //0
      0.0f, -0.6f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //1
      -0.6f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //2
      -0.3f, 0.3f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  //3
      0.3f, 0.3f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  //4
      -0.6f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//5
      0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f, 1.0f //6
   };

   uint indices[] = {
      3, 4, 5,
      5, 4, 6,
      5, 2, 1,
      5, 6, 1,
      6, 1, 0,
      6, 4, 0
   };

   float vertices4[] = {
     0.5f,  0.5f, 0.0f,    1.0f, 1.0f, // top right      0
     0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right   1
     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left    2
     -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left       3
   };

   
   uint indices1[] = {
     0, 1, 3, 
     1, 2, 3  
   };
  
// BUFFERS: 
   uint VBOs[5], VAOs[5], EBOs[3];
   glGenVertexArrays(5, VAOs);
   glGenBuffers(5, VBOs);
   glGenBuffers(3, EBOs);
  
   //TRIANGLE 1
   Vertex_array triangle;
   triangle.create_VBO(vertices3, sizeof(vertices3));
   Layout data;
   data.pushf(3);
   data.pushf(2);
   triangle.add_buffer(data);

   // CUBE 
   Vertex_array cube;
   cube.create_VBO(vertices2, sizeof(vertices2));
   cube.create_EBO(indices, sizeof(indices));
   Layout data2;
   data2.pushf(3);
   data2.pushf(3);
   data2.pushf(2);
   cube.add_buffer(data2);
   
   //FLOOR 
   Vertex_array floor;
   floor.create_VBO(vertices4, sizeof(vertices4));
   floor.create_EBO(indices1, sizeof(indices1));
   floor.add_buffer(data);

   glClearColor(0.25f, 0.4f, 0.5f, 1.0f);
   float move = 0;

   bool tg = 0, cb = 1, fl= 0;
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
         if (move+0.04f < 1.0f)
            move+=0.04f;
      }
      if (tg) {
         //matricies (rotate an object over time)
         glm::mat4 trans = glm::mat4(1.0f);
         trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -0.3f));
         trans = glm::scale(trans, glm::vec3(0.2, 0.3, 0.3));
         trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));

         //draw triangles (with two textures)
         tx.use(tx2.ID);
         s3.use();
         s3.set_matrix4fv("transform", trans);
         triangle.bind();
         glDrawArrays(GL_TRIANGLES, 0, 3);
         glBindVertexArray(0);

      } else if (cb) {
         //draw a cube 
         tx.use();
         s2.use();
         s2.set_float("move", move);
         cube.bind();
         glDrawElements(GL_TRIANGLES, LEN(indices), GL_UNSIGNED_INT, 0);
         glBindVertexArray(0);

      } else if (fl){
         // COORDINATES
         glm::mat4 model, view, projection;
         model = view = projection = glm::mat4(1.0f);
         model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(1.0f, 0.0f, 0.0f));
         model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.0f));
         view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
         projection = glm::perspective(glm::radians(45.0f), (float)WIDTH/HEIGHT, 0.1f, 100.f);

         // draw floor 
         tx.use();
         s4.use();
         s4.set_matrix4fv("model", model);
         s4.set_matrix4fv("view", view);
         s4.set_matrix4fv("projection", projection);
         floor.bind();
         glDrawElements(GL_TRIANGLES, LEN(indices1), GL_UNSIGNED_INT, 0);
         glBindVertexArray(0);
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

