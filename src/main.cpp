#include <cassert>
#include <string>
#include <sys/types.h>

#include "verticies.h"

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
   Shader s5("../shaders/shader_color.vert",         "../shaders/shader.frag");
   Shader hand_shader("../shaders/shader_hand.vert", "../shaders/shader_hand.frag");

// TEXTURES:

   Texture tx  ("../textures/wall.jpg", JPG, GL_REPEAT);
   Texture tx2 ("../textures/face.png", PNG, GL_REPEAT);
   Texture tx3 ("../textures/wall2.jpg", JPG, GL_REPEAT);
   Texture hand_texture ("../textures/hand.png", PNG, GL_CLAMP_TO_BORDER);


// MAP (0 is empty, 1 is a block)
   uint map[MAP_HEIGHT][MAP_WIDTH] = {
      {6, 0, 0, 1, 1, 0, 0, 0, 1, 0},
      {6, 0, 0, 0, 1, 0, 0, 0, 1, 0},
      {6, 0, 2, 0, 1, 0, 0, 0, 0, 1},
      {6, 0, 0, 1, 0, 0, 0, 1, 1, 0},
      {6, 0, 0, 1, 0, 0, 0, 1, 0, 1},
      {6, 0, 0, 1, 1, 0, 0, 0, 0, 1},
      {6, 0, 0, 0, 0, 0, 1, 1, 0, 1},
      {6, 0, 0, 0, 1, 0, 0, 0, 0, 1},
      {6, 0, 0, 1, 0, 0, 1, 0, 0, 1},
      {0, 0, 0, 1, 1, 0, 0, 0, 0, 1},
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
   hand.create_VBO(hand_verticies, sizeof(hand_verticies));
   hand.create_EBO(indices1, sizeof(indices1));
   hand.add_buffer(data);

   //FLOOR 

   Layout data_cube;
   data_cube.pushf(3); // position
   data_cube.pushf(2); // texture
   data_cube.pushf(3); // normal
   
   Vertex_array floor;
   floor.create_VBO(vertices4, sizeof(vertices4));
   floor.create_EBO(indices1, sizeof(indices1));
   floor.add_buffer(data_cube);


   // CUBES
   Vertex_array cube;
   cube.create_VBO(vertices_cube, sizeof(vertices_cube));
   cube.add_buffer(data); 


   Vertex_array cube2;
   cube2.create_VBO(vertices_cube2, sizeof(vertices_cube2));
   cube2.add_buffer(data_cube); 

   float rotate = 0;

   bool tg=0, cb=0, fl=0, hands = 1;
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
   
   for (int j = 0; j != MAP_HEIGHT; j++){
      for (int i = 0; i != MAP_WIDTH; i++){
         if (map[j][i]) {
            for (int k = 0; k < map[j][i]; k++){
               renderer.add_object(std::to_string(j) + std::to_string(i) + std::to_string(k) + " scene", GL_TRIANGLES, &cube2, &s2, &tx3, 36, BUFFER);
            }
         } else {
            renderer.add_object(std::to_string(j) + std::to_string(i) + " floor", GL_TRIANGLES, &floor, &s2, &tx, LEN(indices1), INDICES);
         }
      }
   }

   renderer.add_object("obj",   GL_TRIANGLES, &cube2, &s2, &tx, 36, BUFFER);
   renderer.add_object("light", GL_TRIANGLES, &cube,  &s5, nullptr, 36, BUFFER);
   bool target = true, move_light = false;
   int count = 0;
   glm::vec3 obj_color = {0.9, 0.8, 0.5};
   glm::vec3 light_color = {0.8, 0.8, 0.7};
   glm::vec3 light_pos = {-10.0, 10.0, 0.0};
   glm::vec3 color = {0.8f, 0.8f, 0.8f};
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
         float y_pos;
         glm::mat4 model= glm::mat4(1.0f), projection = glm::mat4(1.0f);
         std::pair<int, int> view_point = utils::get_view_point(win);

         projection = glm::perspective(glm::radians(fov), (float)view_point.first/view_point.second, 0.1f, 100.f);
         for (int j = 0; j != MAP_HEIGHT; j++){
            for (int i = 0; i != MAP_WIDTH; i++){
               y_pos = 0.0f;
               glm::vec3 pos = {x_pos, y_pos, z_pos};
               glm::mat4 model= glm::mat4(1.0f);

               if (map[j][i]) {
                  for (int k = 0; k != map[j][i]; k++){
                     glm::mat4 model= glm::mat4(1.0f);
                     model = glm::translate(model, {pos.x, y_pos, pos.z});
                     renderer.transform_object(std::to_string(j) + std::to_string(i) + std::to_string(k) + " scene", model, camera.get_view(), projection, {pos, {size.x/1.5, size.y/1.5, size.z/1.5}});
                     renderer.draw(std::to_string(j) + std::to_string(i) + std::to_string(k) + " scene");
                     y_pos+=1.0;
                  }
                   printf("\n");
               } else {
                  glm::vec3 sz = {1.0, 0.0, 1.0};
                  model = glm::translate(model, {pos.x, -0.5f,  pos.z});
                  model = glm::scale(model, sz);
                  model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
      
                  renderer.transform_object(std::to_string(j) + std::to_string(i) + " floor", model, camera.get_view(), projection, {{pos.x, -0.5f, pos.z}, sz});
                  renderer.draw(std::to_string(j) + std::to_string(i) + " floor");
               }
               z_pos += size.z;
               y_pos = -0.5;
            }
            x_pos += size.x;
            z_pos = 0.0f;
         }
      }

      // light
      ImGui::Begin("edit color", 0, 0);
      {
         glm::mat4 model, projection;
         model = projection = glm::mat4(1.0f);
         model = glm::translate(model, {0.0f, 0.0f, 0.0f});
         std::pair<int, int> view_point = utils::get_view_point(win);
         projection = glm::perspective(glm::radians(fov), (float)view_point.first/view_point.second, 0.1f, 100.f);
         renderer.transform_object("obj", model, camera.get_view(), projection, {cube_pos, {size.x/1.5, size.y/1.5, size.z/1.5}});
         
         
         ImGui::Text("edit color:");
         float col[3] =  {light_color.x, light_color.y, light_color.z}; 
         float col2[3] = {obj_color.x, obj_color.y, obj_color.z};
         ImGui::ColorEdit3("light", col);
         ImGui::ColorEdit3("object", col2);
         ImGui::Checkbox("move light:",  &move_light);
         light_color = {col[0], col[1], col[2]};
         obj_color = {col2[0], col2[1], col2[2]};
         if (!move_light) { 
            float pos[3] = {light_pos.x, light_pos.y, light_pos.z};
            ImGui::SliderFloat3("pos_ligh", pos, -20.0f, 20.0, "%.5f", 0);
            light_pos = {pos[0], pos[1], pos[2]};
         } else {
            light_pos.x = 1.0f + cos(glfwGetTime()) * 8.0f;
            light_pos.z = cos(glfwGetTime()) * 5.0f;
            light_pos.y = sin(glfwGetTime() / 2.0f) * 5.0f;
         }

         s2.use();
         s2.set_vec3("obj_color",  obj_color);
         s2.set_vec3("light_color", light_color) ;
         s2.set_vec3("light_pos", light_pos);
         s2.set_vec3("view_pos", camera.pos);
         s5.use();
         s5.set_vec3("color", light_color) ;
         
         model = glm::mat4(1.0f);
         model = glm::scale(model, {0.3f, 0.3f, 0.3f});
         model = glm::translate(model, light_pos);
         renderer.transform_object("light", model, camera.get_view(), projection, {cube_pos, {size.x/1.5, size.y/1.5, size.z/1.5}});
         
         renderer.draw("obj");
         renderer.draw("light");
      }
      ImGui::End();

      camera.proccess_keyboard(win, deltatime, mode, renderer.get_objects());
      auto obl = renderer.get_objects();
      camera.proccess_mouse(win, &x, &y);
      #ifdef DEBUG_MODE
      glm::vec3 camera_pos = camera.pos;
      ImGui::Begin("camera", 0, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("CAMERA: %.2f %.2f %.2f", camera_pos.x, camera_pos.y, camera_pos.z);
      ImGui::End();
      glm::vec3 pos;
      if (Input::is_pressed(win, GLFW_KEY_C)){
         glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
         debug_console.draw(&tg, &cb, &fl, &mode, &fov, &cube_pos, camera.pos, &color); 
         /* camera.pos = {2.0f, 9.0f, 2.0f}; */
         /* camera.pitch = -90.0f; */
         /* camera.yaw = -180.0f; */
         if (Input::is_pressed_mouse(win, GLFW_MOUSE_BUTTON_LEFT) && !count) {
            double x_pos, y_pos;
            glfwGetCursorPos(win, &x_pos, &y_pos);
            std::pair<int, int> view_point = utils::get_view_point(win);
            pos = {x_pos, 0.0f, y_pos};
            renderer.add_object(std::to_string(count) + " test", GL_TRIANGLES, &cube, &s4, &tx2, 36, BUFFER);
            count = 1;
         }
      } 
      /* if (Input::is_pressed(win, GLFW_KEY_M)){ */
      else if (!debug_console.is_hovered() || !debug_console.is_clicked()){
         glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
      glClearColor(color.x, color.y, color.z, 1.0f);
      if (count && false) {
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
         model = glm::translate(model, glm::vec3(1.5f, -0.5f, 0.0f));

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


