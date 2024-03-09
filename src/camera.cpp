#include "engine.h"
#include "glm/geometric.hpp"

#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch, int mode): 
   front({0.0, 0.0, -1.0}), speed(SPEED), 
   sensitivity(SENSITIVITY), pos(pos), 
   up(up), yaw(yaw), pitch(pitch), mode(mode) 
{ 
   update_vectors(); 
}

glm::mat4 Camera::get_view(){
   return glm::lookAt(pos, pos+front, up);
}

void Camera::update_vectors(){
   glm::vec3 fr;

   fr.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   fr.y = sin(glm::radians(pitch));
   fr.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

   front = glm::normalize(fr);
   right = glm::normalize(glm::cross(front, up));  
   up    = glm::normalize(glm::cross(right, front));
}

bool collision_detection(glm::vec3 pos, glm::vec3 size, glm::vec3 pos2){
   bool
   coll_x = pos.x + size.x >= pos2.x && pos.x - size.x <= pos2.x,
   coll_y = pos.y + size.y >= pos2.y && pos.y - size.y <= pos2.y,
   coll_z = pos.z + size.z >= pos2.z && pos.z - size.z <= pos2.z;
   return coll_x && coll_y && coll_z;
}

bool ground(glm::vec3 pos, glm::vec3 size, glm::vec3 camera){
   bool
   coll_x = pos.x + size.x >= camera.x && camera.x >= pos.x,
   coll_y = pos.y < camera.y,
   coll_z = pos.z + size.z >= camera.z && camera.z >= pos.z;
   if (coll_z && coll_x) printf("%.3f %.3f - %.3f %.3f\n", pos.x + size.x, pos.z + size.z, camera.x, camera.z);
   return coll_z && coll_x && coll_y;
}

void Camera::proccess_keyboard(GLFWwindow *window, float deltatime, bool fly, std::vector<Object> objects){

   float velocity = speed * deltatime;
   bool object_exists = false;
   if (Input::is_pressed(window, GLFW_KEY_W)){
      glm::vec3 forward = pos + (velocity * front);
      if (!fly) forward.y = 0;
      for (int i = 0; i != objects.size(); i++){
         if (collision_detection(objects[i].pos, objects[i].size, forward)){
            object_exists = true;
            if (!collision_detection(objects[i].pos, objects[i].size, {pos.x, pos.y, pos.z+0.01f}))
               pos.z+=0.01;
            else pos.z-=0.01;
         }
         if (!fly && ground(objects[i].pos, objects[i].size, pos)) {
             forward.y = objects[i].pos.y + 0.7; 
         }
      }
      if (fly && forward.y != 0 )
         pos = forward;
      else if (forward.y != 0 && !object_exists){
         pos = forward;
      } 
      else if (!object_exists) pos = forward;
      
   }
   object_exists = false;
   if (Input::is_pressed(window, GLFW_KEY_S)){
      glm::vec3 backward = pos - (velocity * front);
      if (!fly) backward.y = 0;
      for (int i = 0; i != objects.size(); i++){
         if (collision_detection(objects[i].pos, objects[i].size, backward)){
            object_exists = true;
            if (!collision_detection(objects[i].pos, objects[i].size, {pos.x, pos.y, pos.z+0.01f}))
               pos.z+=0.01;
            else 
               pos.z-=0.01;
         }
         if (!fly && ground(objects[i].pos, objects[i].size, pos)) {
            backward.y = objects[i].pos.y + 0.7; 
         }
       }
      if (fly && (backward.y < 0 || backward.y))
         pos = backward;
      else if ((backward.y < 0 || backward.y > 0) && !object_exists){
         pos = backward;
      } else if (!object_exists) pos = backward;

   }
   object_exists = false;
   if (Input::is_pressed(window, GLFW_KEY_A)){
      glm::vec3 left = pos - glm::normalize(glm::cross(front, up)) * velocity;
      for (int i = 0; i != objects.size(); i++){
         if (collision_detection(objects[i].pos, objects[i].size, left)){
            object_exists = true;
            if (collision_detection(objects[i].pos, objects[i].size, {pos.x, pos.y, pos.z+0.01f}))
               pos.z-=0.01;
            else pos.z+=0.01;
         }
       }
      if (!object_exists)
         pos = left;
   }
   object_exists = false;
   if (Input::is_pressed(window, GLFW_KEY_D)){
      glm::vec3 back = pos + glm::normalize(glm::cross(front, up)) * velocity;
      for (int i = 0; i != objects.size(); i++){
         if (collision_detection(objects[i].pos, objects[i].size, back)){
            object_exists = true;
            if (collision_detection(objects[i].pos, objects[i].size, {pos.x, pos.y, pos.z+0.01f}))
               pos.z-=0.01;
            else pos.z+=0.01;
         }
       }
      if (!object_exists)
         pos = back;
   }
   object_exists = false;
   if (Input::is_pressed(window, GLFW_KEY_M)){
      mode = WALK;
   }
}

void Camera::zoom(float zoom_scale, float *fov){
   *fov -= (float)zoom_scale;
   if (*fov < 1.0f)  *fov = 1.0f;
   if (*fov > 45.0f) *fov = 45.0f;
   zoom_scale += 0.2f;
   if (zoom_scale > 5.0f){ 
      zoom_scale = 0;
      *fov = FOV;
   }
}

void Camera::proccess_mouse(GLFWwindow *window, float *last_x, float *last_y){
   double xpos, ypos;
   glfwGetCursorPos(window, &xpos, &ypos);
   
   float offset_x = xpos - *last_x;
   float offset_y = *last_y - ypos;

   *last_x = xpos;
   *last_y = ypos;
   
   offset_x *= sensitivity;
   offset_y *= sensitivity;
   yaw += offset_x;
   pitch += offset_y;
   
   if (pitch > 89.0f)   pitch = 89.0f;
   if (pitch < -89.0f)  pitch = -89.0f;

   glm::vec3 directicion;
   directicion.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   directicion.y = sin(glm::radians(pitch));
   directicion.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   front = glm::normalize(directicion);
}

;
