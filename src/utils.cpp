#include "utils.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "imgui/imgui.h"

#include <GLFW/glfw3.h>


#include <cassert>
#include <fstream>
#include <sstream>

namespace utils{

   void frame_buffer_size(GLFWwindow* window, int width, int height){
      glViewport(0, 0, width, height);
      utils::log("changed view port");
   }

   std::pair<int, int> get_view_point(GLFWwindow *window){
      int view_point[2];
      glfwGetFramebufferSize(window, &view_point[0], &view_point[1]);
      return {view_point[0], view_point[1]};
   }
   std::string get_source_from_file(const char filename[]){
      std::string source;
      std::ifstream file;
      file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      try {
         file.open(filename);
         std::stringstream stream_file; 
         stream_file << file.rdbuf();
         file.close();
         source = stream_file.str();
      }
      catch(std::ifstream::failure e){
         utils::error("open file", filename);
      }
      return source;
   }
   void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLuint severity,
                                   GLsizei length, const GLchar* message, const GLvoid* userParam ){
     if (type == GL_DEBUG_TYPE_ERROR){
        utils::error(message, std::to_string(type));
     }
     assert(type != GL_DEBUG_TYPE_ERROR);
   }

   Debug::Debug(GLFWwindow *window){
      ImGui::CreateContext();
      ImGuiIO& IO = ImGui::GetIO();
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(window, true);
      ImGui_ImplOpenGL3_Init();
   }
   Debug::~Debug(){
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
   }
   void Debug::new_frame(){
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
   }
   bool Debug::is_hovered(){
      return hovered;
   }
   bool Debug::is_clicked(){
      return clicked;
   }
   void Debug::render(){
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   }
   void Debug::draw(bool *tg, bool *cb, bool *fl, bool *mode, float *fov){
      ImGuiWindowFlags flags = 0;
      flags |= ImGuiWindowFlags_AlwaysAutoResize;

      bool draw_cube = 0, edit_mode = 1;
      ImGui::Begin("console", 0, flags);
      {
         if (ImGui::IsItemHovered()) hovered = true;
         else hovered = false;
         if (ImGui::IsItemClicked()) clicked = true;
         else clicked = false;
         ImGui::Text("change:");
         ImGui::Checkbox("Triangle:", tg);
         ImGui::Checkbox("Cube:", cb);
         ImGui::Checkbox("Floor:", fl);
         ImGui::Checkbox("FLY:", mode);
         ImGui::Checkbox("edit_mode:", &edit_mode);
         ImGui::SliderFloat("zoom scale", fov, 0.0f, 100.0f, "%.0f", 0);
         if (edit_mode && ImGui::IsKeyDown(ImGuiKey_MouseLeft)) {
            //TODO
             ImVec2 pos = ImGui::GetMousePos();
             glm::vec2 pos2 = glm::normalize(glm::vec2(pos.x, pos.y));
             printf("%f %f\n", pos2.x, pos2.y); 
           }
      }
      ImGui::End();
   }

}
