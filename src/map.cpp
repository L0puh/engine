#include "engine.h"
#include "utils.h"

#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <fstream>
#include <iostream>


int Renderer::process_map(GLFWwindow* win, float fov, Camera camera, glm::vec3 size){ 
   //TODO: cache later? 
   std::vector<std::vector<int>> map = get_map(FILENAME);

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
               transform_object(std::to_string(j) + std::to_string(i) + std::to_string(k) + " scene", model, camera.get_view(), projection, {pos, {size.x/1.5, size.y/1.5, size.z/1.5}});
               draw(std::to_string(j) + std::to_string(i) + std::to_string(k) + " scene");
               y_pos+=1.0;
            }
             printf("\n");
         } else {
            glm::vec3 sz = {1.0, 0.0, 1.0};
            model = glm::translate(model, {pos.x, -0.5f,  pos.z});
            model = glm::scale(model, sz);
            model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));

            transform_object(std::to_string(j) + std::to_string(i) + " floor", model, camera.get_view(), projection, {{pos.x, -0.5f, pos.z}, sz});
            draw(std::to_string(j) + std::to_string(i) + " floor");
         }
         z_pos += size.z;
         y_pos = -0.5;
      }
      x_pos += size.x;
      z_pos = 0.0f;
   }
   return 0;
}

std::vector<std::vector<int>> Renderer::get_map(std::string filename){
   std::vector<std::vector<int>> map(MAP_HEIGHT, std::vector<int>(MAP_WIDTH));
   freopen(filename.c_str(), "r", stdin);
   for (int i = 0; i != MAP_HEIGHT; i++){
      for (int j = 0; j != MAP_WIDTH; j++){
         int num;
         std::scanf("%d ", &num);
         map[i][j] = num;
      }
   }
   return map;
}

std::string Renderer::get_map_string(std::string filename){
   //FIXME: ifstream?
   std::string map;
   freopen(filename.c_str(), "r", stdin);
   for (int i = 0; i != MAP_HEIGHT; i++){
      for (int j = 0; j != MAP_WIDTH; j++){
         char num;
         std::scanf("%c ", &num);
         map = map + num + " ";
      }
   }
   return map;
}

int Renderer::change_map(std::string filename, std::vector<std::vector<int>> map){
    std::ofstream file;
    file.open (filename);
    for (int i = 0; i != MAP_HEIGHT; i++){
      for (int j = 0; j != MAP_WIDTH; j++){
         file << map[i][j] << ' ';
         printf("%d ", map[i][j]);
      }
      file << '\n';
      putchar('\n');
   }
   file.close();
   return 0;
}
