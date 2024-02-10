#ifndef ENGINE_H
#define ENGINE_H 

#include <glm/ext/matrix_float4x4.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

#define LEN(x) sizeof(x)/sizeof(x[0])

void check_status_shader_program(uint shader_program);
void frame_buffer_size(GLFWwindow* window, int width, int height);
GLFWwindow* init_window(const int WIDTH, const int HEIGHT);

/*******************************************************************************/

enum Types {
   PNG,
   JPG
};

class Shader {
   public:
      Shader(const char src_vertex[], const char src_fragment[]);
      ~Shader();

   public:
      uint ID;
      void use();

      void set_float(const char name[], float x);
      void set_int(const char name[], int x);
      void set_matrix4fv(const char name[], glm::mat4 data);
      void set_vec4(const char name[], glm::vec4 vec);

   private:
      std::unordered_map<std::string, int> cached_locations;

      void create_shader(uint* shader, const char src[], GLuint type);
      int get_location(const char name[]);
};


class Texture {
   public:
      Texture(const char texture_path[], int type);
      ~Texture();
   public:
      uint ID;
      void use(uint ID2);
      void use();
};

/*******************************************************************************/
enum modes{
   FLY,
   WALK,
};

const float SPEED = 2.5f, 
            SENSITIVITY = 0.05f,
            PITCH = 0.0f,
            YAW = -90.0f,
            FOV = 55.0f;

class Camera {
   public: 
      glm::vec3 pos, front,  up, right;
      float speed, sensitivity, zoom_scale = 0;
      float pitch, yaw;
      int mode;
   public:
      Camera(glm::vec3 position = {0.0, 0.0, 0.0}, glm::vec3 up = {0.0, 1.0, 0.0}, 
            float yaw = YAW, float pitch = PITCH, int mode = FLY);
      void update_vectors();
      glm::mat4 get_view();
      void proccess_keyboard(GLFWwindow *window, float deltatime, bool mode);
      void proccess_mouse(GLFWwindow *window, float *last_x, float *last_y);
      void zoom(float *fov);

   private:
};

class Input {
   public:
      static bool is_pressed(GLFWwindow* window, int key);
      static void mouse_callback(GLFWwindow *window, double x, double y);      
      static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};


/*******************************************************************************/

struct Layout_element {
   uint type;
   uint count;
   uint normalised;
};

class Layout {
   public:
      void pushf(uint count){
         elements.push_back({GL_FLOAT, count, GL_FALSE});
         stride += sizeof(float) * count;
      }
      void pushui(uint count){
         elements.push_back({GL_FLOAT, count, GL_FALSE});
         stride += sizeof(uint) * count;
      }
      std::vector<Layout_element> get_elements() { return elements; };
      uint get_stride() { return stride; };

   private:
      uint stride=0;
      std::vector<Layout_element> elements;
};

class Vertex_array {
   public:
      Vertex_array();
      ~Vertex_array();
   public:
      void bind();
      void create_VBO(const void* data, size_t size);
      void create_EBO(const void* data, size_t size);
      void add_buffer(Layout& layout);
      uint get_id() { return VAO; };
   private: 
      uint VAO, VBO, EBO; 
};


#endif
