#include <cassert>
#include <string>
#include <sys/types.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "imgui/imgui.h"
#include "utils.h"

#define DEBUG_MODE 



int main(){


// WINDOW: 
   GLFWwindow* win =  init_window(WIDTH, HEIGHT);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

   glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   assert(win != 0);

   #ifdef DEBUG_MODE
      glEnable(GL_DEBUG_OUTPUT);
      glDebugMessageCallback(utils::message_callback, 0);
      utils::log("DEBUG MODE ON");
      utils::Debug debug_console(win);
   #endif

// SHADERS: 

   Shader s1("../shaders/shader.vert",               "../shaders/shader.frag");            // with uniform
   Shader s2("../shaders/shader.vert",               "../shaders/shader2.frag");    
   Shader s3("../shaders/shader2.vert",              "../shaders/shader_tex.frag");        // with texture
   Shader s4("../shaders/shader_floor.vert",         "../shaders/shader_floor.frag");
   Shader hand_shader("../shaders/shader_hand.vert", "../shaders/shader_hand.frag");

// TEXTURES:

   Texture tx  ("../textures/wall.jpg", JPG, GL_REPEAT);
   Texture tx2 ("../textures/face.png", PNG, GL_REPEAT);
   Texture tx3 ("../textures/wall2.jpg", JPG, GL_REPEAT);
   Texture hand_texture ("../textures/hand.png", PNG, GL_CLAMP_TO_BORDER);


// MAP (0 is empty, 1 is a block)
   uint map[5][5] = {
      {0, 0, 1, 1, 1},
      {1, 0, 0, 0, 1},
      {1, 0, 1, 0, 0},
      {1, 1, 0, 0, 1},
      {0, 0, 1, 1, 1},
   };


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

   float hand_verticies[] = {
      // x   y     z          texture
     0.5f,  0.5f, 0.0f,    1.0f, 1.0f, // top right      0
     0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right   1
     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left    2
     -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left       3
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
   
   //TRIANGLE 1
   Vertex_array triangle;
   triangle.create_VBO(vertices3, sizeof(vertices3));
   triangle.add_buffer(data);

  
   //HAND 
   Vertex_array hand;
   hand.create_VBO(vertices4, sizeof(hand_verticies));
   hand.create_EBO(indices1, sizeof(indices1));
   hand.add_buffer(data);
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
   float rotate = 0;

   bool tg = 0, cb = 1, fl= 1, hands = 1;
   s3.use();
   s3.set_int("my_texture", 0);
   s3.set_int("my_texture2", 1);
   s3.unuse();

//CAMERA:
   Camera camera;
   float deltatime=0.0f, lastframe=0.0f;
   float fov = FOV;
   float x =  WIDTH / 2.0f, y = HEIGHT / 2.0f;
   bool mode = 1, hovered = 0;
   
   glm::vec3 size = {1.0, 1.0, 1.0}, cube_pos;
  
   Renderer renderer;
   renderer.add_object("cube", GL_TRIANGLES, &cube, &s4, &tx, 36, BUFFER);
   renderer.add_object("triangle", GL_TRIANGLES, &triangle, &s4, &tx2, 3, BUFFER);
   
   for (int j = 0; j != 5; j++){
      for (int i = 0; i != 5; i++){
         if (map[j][i]) {
            renderer.add_object(std::to_string(j) + std::to_string(i) + " scene", GL_TRIANGLES, &cube, &s4, &tx3, 36, BUFFER);
         } else {
            renderer.add_object(std::to_string(j) + std::to_string(i) + " floor", GL_TRIANGLES, &floor, &s4, &tx, LEN(indices1), INDICES );
         }
      }
   }

  bool target = true; 
  int count = 0;
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
      
      if (tg) {
         //matrices(rotate an object over time) 
         glm::mat4 trans = glm::mat4(1.0f), projection = glm::mat4(1.0f);
         trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -0.3f));
         trans = glm::scale(trans, glm::vec3(0.2, 0.3, 0.3));
         trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
         
         std::pair<int, int> view_point = utils::get_view_point(win);
         projection = glm::perspective(glm::radians(fov), (float)view_point.first/view_point.second, 0.1f, 100.f);

         renderer.transform_object("triangle", trans, camera.get_view(), projection, {glm::vec3(0.3f), glm::vec3(0.3f)});
         renderer.draw("triangle");

      } 
      if (cb) {
         //draw a cube 
         
         glm::mat4 model, projection;
         model = projection = glm::mat4(1.0f);
         model = glm::translate(model, cube_pos);
         model = glm::rotate(model, (float)glm::radians(rotate), glm::vec3(0.0, 1.0, 0.0f));
         model = glm::scale(model, glm::vec3(1.0, 1.0, 1.0));

         std::pair<int, int> view_point = utils::get_view_point(win);
         projection = glm::perspective(glm::radians(fov), (float)view_point.first/view_point.second, 0.1f, 100.f);
         renderer.transform_object("cube", model, camera.get_view(), projection, {cube_pos, {size.x/1.5, size.y/1.5, size.z/1.5}});
         renderer.draw("cube");

      } 
      if (fl) {
         float x_pos = 0.0f;
         float z_pos = 0.0f;
         for (int j = 0; j != 5; j++){
            for (int i = 0; i != 5; i++){
                  glm::mat4 model= glm::mat4(1.0f), projection = glm::mat4(1.0f);
                  glm::vec3 pos = {x_pos, 0.0f, z_pos};

                  std::pair<int, int> view_point = utils::get_view_point(win);
                  projection = glm::perspective(glm::radians(fov), (float)view_point.first/view_point.second, 0.1f, 100.f);

               if (map[j][i]) {
                  model = glm::translate(model, pos);
                  renderer.transform_object(std::to_string(j) + std::to_string(i) + " scene", model, camera.get_view(), projection, {pos, {size.x/1.5, size.y/1.5, size.z/1.5}});
                  renderer.draw(std::to_string(j) + std::to_string(i) + " scene");
               } else {
                  model = glm::translate(model, {pos.x, -0.5f,  pos.z});
                  model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
                  renderer.transform_object(std::to_string(j) + std::to_string(i) + " floor", model, camera.get_view(), projection, {{pos.x, -0.5f, pos.z}, glm::vec3(0.1)});
                  renderer.draw(std::to_string(j) + std::to_string(i) + " floor");
               }
               z_pos += size.z;
            }
            x_pos += size.x;
            z_pos = 0.0f;
         }
      }
      
      camera.proccess_keyboard(win, deltatime, mode, renderer.get_objects());
      auto obl = renderer.get_objects();


      camera.proccess_mouse   (win, &x, &y);
      #ifdef DEBUG_MODE
      glm::vec3 camera_pos = camera.pos;
      ImGui::Begin("camera", 0, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("CAMERA: %.2f %.2f %.2f", camera_pos.x, camera_pos.y, camera_pos.z);
      ImGui::End();
      glm::vec3 pos;
      if (Input::is_pressed(win, GLFW_KEY_C)){
         glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
         debug_console.draw(&tg, &cb, &fl, &mode, &fov, &cube_pos, camera.pos); 
         camera.pos = {2.0f, 9.0f, 2.0f};
         camera.pitch = -90.0f;
         camera.yaw = -180.0f;
         if (Input::is_pressed_mouse(win, GLFW_MOUSE_BUTTON_LEFT) && !count) {
            double x_pos, y_pos;
            glfwGetCursorPos(win, &x_pos, &y_pos);
            std::pair<int, int> view_point = utils::get_view_point(win);
            pos = {x_pos, 0.0f, y_pos};
            renderer.add_object(std::to_string(count) + " test", GL_TRIANGLES, &cube, &s4, &tx2, 36, BUFFER);
            count = 1;
         }
      } else if (!debug_console.is_hovered() || !debug_console.is_clicked()){
         glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
      if (count) {
         glm::mat4 model, projection;
         model = projection = glm::mat4(1.0f);
         std::pair<int, int> view_point = utils::get_view_point(win);
         projection = glm::perspective(glm::radians(fov), (float)view_point.first/view_point.second, 0.1f, 100.f);
         pos = glm::normalize(pos); //FIXME: normalize over the projection 
         model = glm::translate(model, pos);

         renderer.transform_object(std::to_string(0) + " test", model, camera.get_view(), projection, {pos, size});
         renderer.draw(std::to_string(0) + " test");
      }

      if (hands){
         glm::mat4 model = glm::mat4(1.0f);;
         model = glm::scale(model, glm::vec3(0.5, 1.0, 0.0));
         model = glm::translate(model, glm::vec3(1.5f, -0.5f, 0.0));

         hand_texture.use();
         hand_shader.use();
         hand_shader.set_matrix4fv("model", model);
         hand.draw(GL_TRIANGLES, LEN(indices1));
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


