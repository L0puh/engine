#include "engine.h"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <string>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/******************************************************/

GLFWwindow* init_window(const int WIDTH, const int HEIGHT){
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
      utils::error("init window");
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
   utils::log("init window");
   return win;
}


void check_status_shader_program(uint shader_program){
   int res; 
   char info[512];
   glGetProgramiv(shader_program, GL_LINK_STATUS, &res);
   if (!res){
      glGetProgramInfoLog(shader_program, 512, NULL, info);
      utils::error("shaders", info);
   } else utils::log("linked shader program", std::to_string(shader_program));
}


void frame_buffer_size(GLFWwindow* window, int width, int height){
   glViewport(0, 0, width, height);
   utils::log("changed view port");
}

/******************************************************/
//                    SHADER                          //
/******************************************************/

Shader::Shader(const char src_vertex[], const char src_fragment[]){
   uint vertex, fragment;
   create_shader(&vertex, src_vertex, GL_VERTEX_SHADER);
   create_shader(&fragment, src_fragment, GL_FRAGMENT_SHADER);

   ID = glCreateProgram();
   glAttachShader(ID, vertex);
   glAttachShader(ID, fragment);
   glLinkProgram(ID);
   check_status_shader_program(ID);
   glDeleteShader(vertex);
   glDeleteShader(fragment);
}

Shader::~Shader(){
   glDeleteProgram(ID);
   utils::log("deleted shader", std::to_string(ID));
}

void Shader::create_shader(uint* shader, const char src[], GLuint type){
   int res; char info[512];
   *shader = glCreateShader(type); 
   std::string shader_code= utils::get_source_from_file(src);
   const char* source = shader_code.c_str();
   glShaderSource(*shader, 1, &source, NULL); 
   glCompileShader(*shader);
   glGetShaderiv(*shader, GL_COMPILE_STATUS, &res);
   if (!res) {
      glGetShaderInfoLog(*shader, 512, NULL, info);;
      utils::error(src, info);
   } else utils::log(src, "load shader");
}

int Shader::get_location(const char name[]){
   int location;
   if (cached_locations[name])
      return cached_locations[name];

   location = glGetUniformLocation(ID, name);
   if (location == -1){
      utils::error("location", name);
      assert(location != -1);
   }
   cached_locations[name] = location;
   return location;
}

void Shader::use(){
   glUseProgram(ID);
}

void Shader::set_float(const char name[], float x){
   int location = get_location(name);
   glUniform1f(location, x); 

}

void Shader::set_int(const char name[], int x){
   int location = get_location(name);
   glUniform1i(location, x); 
}

void Shader::set_vec4(const char name[], glm::vec4 vec){
   int location = get_location(name);
   glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::set_matrix4fv(const char name[], glm::mat4 data){
   int location = get_location(name);
   glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

/******************************************************/
//                    TEXTURE                         //
/******************************************************/

Texture::Texture(const char texture_path[], int img_type){
   glGenTextures(1, &ID);
   glBindTexture(GL_TEXTURE_2D, ID);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   int width, height, color_channels;

   unsigned char *data = stbi_load(texture_path, &width, &height, &color_channels, 0);
   if (!data){ 
      utils::error(texture_path, "couldn't load data");
      assert(data);
   } 

   switch(img_type){
      case PNG:
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
         break;
      case JPG:
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
         break;
   }

   glGenerateMipmap(GL_TEXTURE_2D);
   stbi_image_free(data);
   utils::log(texture_path,"load texture");
}

Texture::~Texture(){
   glDeleteTextures(1, &ID);
   utils::log("deleted texture", std::to_string(ID));
}

void Texture::use(uint ID2){
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, ID);

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, ID2);
   
}
void Texture::use(){
   glBindTexture(GL_TEXTURE_2D, ID);
}

/******************************************************/
//                    CAMERA                          //
/******************************************************/

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

void Camera::proccess_keyboard(GLFWwindow *window, float deltatime){
   float velocity = speed * deltatime;
   if (Input::is_pressed(window, GLFW_KEY_W)){
      glm::vec3 forward = pos + (velocity * front);
      if (mode == FLY && (forward.y > 0 || forward.y < 0))
         pos = forward;
      else if (forward.y > 0 || forward.y < 0){
         forward.y = 0;
         pos = forward;
      } else pos = forward;
      
   }
   if (Input::is_pressed(window, GLFW_KEY_S)){
      glm::vec3 backward = pos - (velocity * front);
      if (mode == FLY && (backward.y < 0||backward.y))
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

void Camera::zoom(float *fov){
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


/******************************************************/
//                    INPUT                           //
/******************************************************/

bool Input::is_pressed(GLFWwindow* window, int key){
   return glfwGetKey(window, key) == GLFW_PRESS;
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
   switch(key){
      case GLFW_KEY_ESCAPE:
         glfwSetWindowShouldClose(window, true);
         utils::log("close the program");
         break;
      case GLFW_KEY_P:
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         break;
      case GLFW_KEY_F:
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         break;
   }
}

/******************************************************/
//                    VERTEX ARRAY                    //
/******************************************************/

Vertex_array::Vertex_array(){
   glGenVertexArrays(1, &VAO);
}

Vertex_array::~Vertex_array(){
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
}

void Vertex_array::create_VBO(const void* data, size_t size){
   bind();
   glGenBuffers(1, &VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Vertex_array::create_EBO(const void* data, size_t size){
   bind();
   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Vertex_array::add_buffer(Layout& layout){
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   const std::vector<Layout_element> elements = layout.get_elements(); 
   size_t offset = 0;
   for (int i = 0; i != elements.size(); i++){
      Layout_element el = elements.at(i);
      glVertexAttribPointer(i, el.count, el.type, el.normalised, layout.get_stride(), (void*)offset);
      glEnableVertexAttribArray(i);
      offset += el.count * sizeof(el.type);
   }
}
void Vertex_array::bind(){
   glBindVertexArray(VAO);
}

