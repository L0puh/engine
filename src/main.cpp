#include <cassert>
#include <sys/types.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.h"
#include "utils.h"

#define DEBUG_MODE 

int main(){

// WINDOW: 
   GLFWwindow* win =  init_window(WIDTH, HEIGHT);
   glEnable(GL_DEPTH_TEST);
   glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   assert(win != 0);

   #ifdef DEBUG_MODE
      glEnable(GL_DEBUG_OUTPUT);
      glDebugMessageCallback(utils::message_callback, 0);
      utils::log("DEBUG MODE ON");
      utils::Debug debug_console(win);
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

   float vertices_cube[] = {
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
   };

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
   Layout data;
   data.pushf(3);
   data.pushf(2);
   
   uint VBOs[5], VAOs[5], EBOs[3];
   glGenVertexArrays(5, VAOs);
   glGenBuffers(5, VBOs);
   glGenBuffers(3, EBOs);
  
   //TRIANGLE 1
   Vertex_array triangle;
   triangle.create_VBO(vertices3, sizeof(vertices3));
   triangle.add_buffer(data);

   
   //FLOOR 
   Vertex_array floor;
   floor.create_VBO(vertices4, sizeof(vertices4));
   floor.create_EBO(indices1, sizeof(indices1));
   floor.add_buffer(data);


   // ANOTHER CUBE 
   Vertex_array cube;
   cube.create_VBO(vertices_cube, sizeof(vertices_cube));
   cube.add_buffer(data); 

   glClearColor(0.25f, 0.4f, 0.5f, 1.0f);
   float move = 0, rotate = 0;

   bool tg = 0, cb = 1, fl= 0;
   s3.use();
   s3.set_int("my_texture", 0);
   s3.set_int("my_texture2", 1);

//CAMERA:
   float deltatime=0.0f, lastframe=0.0f;
   float fov = FOV;
   float x =  WIDTH / 2.0f, y = HEIGHT / 2.0f;
   bool mode = 1, hovered = 0;

   Camera camera;

// MAIN LOOP:
   while(!glfwWindowShouldClose(win)){

      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      #ifdef DEBUG_MODE 
       debug_console.new_frame();
      #endif

      float current_frame = glfwGetTime();
      deltatime = current_frame - lastframe;
      lastframe = current_frame;

      camera.proccess_keyboard(win, deltatime, mode);
      camera.proccess_mouse(win, &x, &y);
      
      //moving  
      if (Input::is_pressed(win, GLFW_KEY_UP)){
         rotate++;
      } 
      if (tg ) {
         //MATrices (rotate an object over time)
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

      } 
      if (cb) {
         //draw a cube 
         
         glm::mat4 model, view, projection;
         
         model = view = projection = glm::mat4(1.0f);
         model = glm::rotate(model, (float)glm::radians(rotate), glm::vec3(0.0, 1.0, 0.0f));
         model = glm::scale(model, glm::vec3(1.0, 1.0, 1.0));
         view = glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
         std::pair<int, int> view_point = utils::get_view_point(win);
         projection = glm::perspective(glm::radians(fov), (float)view_point.first/view_point.second, 0.1f, 100.f);

         tx.use();
         s4.use();
         
         s4.set_float("move", move);
         s4.set_matrix4fv("model", model);
         s4.set_matrix4fv("view", view);
         s4.set_matrix4fv("projection", projection);
         
         cube.bind();
         glDrawArrays(GL_TRIANGLES, 0, 36);
         glBindVertexArray(0);

      } 
      if (fl){
         // COORDINATES
         glm::mat4 model, view, projection;
         model = view = projection = glm::mat4(1.0f);
         model = glm::translate(model, glm::vec3(-0.4f, 0.0f, 0.0f));
         model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(1.0f, 0.0f, 0.0f));
         model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.0f));
         view = camera.get_view(); 
         projection = glm::perspective(glm::radians(fov), (float)WIDTH/HEIGHT, 0.1f, 100.f);

         // draw floor 
         tx.use();
         s4.use();
         s4.set_float("move", move);
         s4.set_matrix4fv("model", model);
         s4.set_matrix4fv("view", view);
         s4.set_matrix4fv("projection", projection);
         floor.bind();
         glDrawElements(GL_TRIANGLES, LEN(indices1), GL_UNSIGNED_INT, 0);
         glBindVertexArray(0);
      }

      #ifdef DEBUG_MODE
      if (Input::is_pressed(win, GLFW_KEY_C)){
         glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
         debug_console.draw(&tg, &cb, &fl, &mode, &fov); 
      } else if (!debug_console.is_hovered() || !debug_console.is_clicked()){
         glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
      debug_console.render(); 
      #endif
      glfwSetKeyCallback(win, Input::key_callback);
      glfwSwapBuffers(win);
   }

   glfwDestroyWindow(win);
   glfwTerminate();

   return 0;
}

