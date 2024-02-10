#include "engine.h"

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

void Camera::proccess_keyboard(GLFWwindow *window, float deltatime, bool mode){
   float velocity = speed * deltatime;
   if (Input::is_pressed(window, GLFW_KEY_W)){
      glm::vec3 forward = pos + (velocity * front);
      if (mode && (forward.y > 0 || forward.y < 0))
         pos = forward;
      else if (forward.y > 0 || forward.y < 0){
         forward.y = 0;
         pos = forward;
      } else pos = forward;
      
   }
   if (Input::is_pressed(window, GLFW_KEY_S)){
      glm::vec3 backward = pos - (velocity * front);
      if (mode && (backward.y < 0||backward.y))
         pos = backward;
      else if (backward.y < 0 || backward.y > 0 ){
         backward.y = 0;
         pos = backward;
      } else pos = backward;

   }
   if (Input::is_pressed(window, GLFW_KEY_A)){
      pos -= glm::normalize(glm::cross(front, up)) * velocity;
   }
   if (Input::is_pressed(window, GLFW_KEY_D)){
      pos += glm::normalize(glm::cross(front, up)) * velocity;
   }
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
   
   if (pitch > 89.0f)   pitch= 89.0f;
   if (pitch < -89.0f)  pitch = -89.0f;

   
   glm::vec3 directicion;
   directicion.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   directicion.y = sin(glm::radians(pitch));
   directicion.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   front = glm::normalize(directicion);
}


