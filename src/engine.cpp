#include "engine.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "utils.h"
#include <string>
#include <vector>


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
   glfwSetFramebufferSizeCallback(win, utils::frame_buffer_size);
   utils::log("init window");
   return win;
}

void draw_scene(GLFWwindow* window, float *fov, Camera *camera, Texture *tx, Texture *tx2, Shader *sh, Vertex_array *buffer, size_t size){
   // COORDINATES (FIXME: clear this up)
   glm::mat4 model, view, projection;
   float offset_x = -0.4f, offset_y = 0.0f, 
         offset_z = -0.5f, offset_z2 = -0.2f;
   for (int i = 0; i != 10; i++){ 
      for (int j = 0; j != 10; j++){
         model = view = projection = glm::mat4(1.0f);
         offset_y += 0.9f;
         model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
         model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
         model = glm::translate(model, glm::vec3(offset_x, offset_y, 0.0f));
         model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.0f));
         view = camera->get_view(); 
         std::pair<int, int> view_point = utils::get_view_point(window);
         projection = glm::perspective(glm::radians(*fov), (float)view_point.first/view_point.second, 0.1f, 100.f);

         // draw floor 
         tx->use();
         sh->use();
         sh->set_matrix4fv("model", model);
         sh->set_matrix4fv("view", view);
         sh->set_matrix4fv("projection", projection);
         buffer->draw(GL_TRIANGLES, size);
      }
      offset_y=0.0f;
      offset_x+=0.9f;
   }
   offset_x = -0.4f;
   glm::vec3 model_pos, model_size;
   for (int j = 0; j != 5; j++){
      for (int i = 0; i != 10; i++){ 
         model = view = projection = glm::mat4(1.0f);
         if (j == 2){
            model_pos = {offset_x, -0.1f, offset_z-9};
            model = glm::translate(model, model_pos);
         } else if (j == 3 || j == 4){
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0f, 0.0f));
            if (j == 4){
               //FIXME
               model_pos = {offset_x, -0.1f, offset_z2+8};
               model = glm::translate(model, model_pos);
            } else{ 
               model_pos = {offset_x, -0.1f, offset_z2};
               model = glm::translate(model, model_pos);
            }
         } else{
            model_pos = {offset_x, -0.1f, offset_z};
            model = glm::translate(model, model_pos);
         }
         model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.1f));
         view = camera->get_view(); 
         std::pair<int, int> view_point = utils::get_view_point(window);
         projection = glm::perspective(glm::radians(*fov), (float)view_point.first/view_point.second, 0.1f, 100.f);

         // draw walls
         tx2->use();
         sh->use();
         sh->set_matrix4fv("model", model);
         sh->set_matrix4fv("view", view);
         sh->set_matrix4fv("projection", projection);
         buffer->draw(GL_TRIANGLES, size);

         model_size = {0.9, 0.9, 0.3};
         buffer->draw(GL_TRIANGLES, size);
         offset_x+=0.9f;
      }
      offset_x = -0.4f;
      if (j == 3 || j == 4) offset_x = 1.0f;
   }
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

void Vertex_array::draw(GLenum mode, size_t size){
   bind();
   glDrawElements(mode, size, GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);
}

void Vertex_array::draw_buffer(GLenum mode, size_t size){
   bind();
   glDrawArrays(mode, 0, size);
   glBindVertexArray(0);
}

/******************************************************/
//                    RENDERER                        //
//           (just a prototype for now)               //
/******************************************************/


Renderer::Renderer(GLenum mode, Vertex_array *buffer, Shader *sh, Texture *tx, size_t size, Types type):
   mode(mode), buffer(buffer), shader(sh), texture(tx), size(size), type(type){}

void Renderer::draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection){
   
   texture->use();
   shader->use();
   shader->set_matrix4fv("model", model);
   shader->set_matrix4fv("view", view);
   shader->set_matrix4fv("projection", projection);

   if      (type == BUFFER) buffer->draw_buffer(mode, size);
   else if (type == INDICES) buffer->draw(mode, size);

}

