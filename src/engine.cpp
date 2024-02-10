#include "engine.h"
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


void check_status_shader_program(uint shader_program){
   int res; 
   char info[512];
   glGetProgramiv(shader_program, GL_LINK_STATUS, &res);
   if (!res){
      glGetProgramInfoLog(shader_program, 512, NULL, info);
      utils::error("shaders", info);
   } else utils::log("linked shader program", std::to_string(shader_program));
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

