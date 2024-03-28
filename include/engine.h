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

class Camera;
class Texture;
class Shader;
class Vertex_array;

void check_status_shader_program(uint shader_program);
GLFWwindow* init_window(const int WIDTH, const int HEIGHT);

/*******************************************************************************/

const int 
WIDTH  = 300,
HEIGHT = 300,
MAP_WIDTH  = 10,
MAP_HEIGHT = 10;

const float 
SPEED = 2.5f, 
SENSITIVITY = 0.05f,
PITCH = 0.0f,
YAW = -90.0f,
FOV = 55.0f;

const std::string FILENAME = "map.txt";

/*******************************************************************************/

enum Types {
   PNG,
   JPG,
   BUFFER,
   INDICES,
};

class Shader {
   public:
      Shader(const char src_vertex[], const char src_fragment[]);
      ~Shader();

   public:
      uint ID;
      void use();
      void unuse();

      void set_float(const char name[], float x);
      void set_vec3(const char name[], glm::vec3 vec);
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
      Texture(const char texture_path[], int type, GLenum mode);
      ~Texture();
   public:
      uint ID;
      void use(uint ID2);
      void use();
      void unuse();
};

/*******************************************************************************/
enum modes{
   FLY,
   WALK,
};

struct Object {
   glm::vec3 pos;
   glm::vec3 size;
};

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
      void process_keyboard(GLFWwindow *window, float deltatime, bool mode, std::vector<Object> objects);
      void process_mouse(GLFWwindow *window, float *last_x, float *last_y);
      static void zoom(float scale, float *fov);

   private:
};

class Input {
   public:
      static bool is_pressed_mouse(GLFWwindow* window, int key);
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
      void draw(GLenum mode, size_t size);
      void draw_buffer(GLenum mode, size_t size);
      void create_VBO(const void* data, size_t size);
      void create_EBO(const void* data, size_t size);
      void add_buffer(Layout& layout);
      uint get_id() { return VAO; };
   private: 
      uint VAO, VBO, EBO; 
};

/*******************************************************************************/

struct Pos {
      glm::mat4 model;
      glm::mat4 view;
      glm::mat4 projection;
};
struct Entity {
   GLenum mode;
   Vertex_array *buffer;
   Shader *shader;
   Texture *texture;
   Types type;
   size_t size;
   Pos pos;
   Object obj;
};

class Renderer {
   std::unordered_map<std::string, Entity> entities;
   public:
      //OBJECTS
      Renderer();
      void transform_object(std::string label, glm::mat4 model, glm::mat4 view, glm::mat4 projection, Object obj);
      void add_object(std::string label, GLenum mode, Vertex_array *buffer, Shader *sh, Texture *tx, size_t size, Types type);
      void draw(std::string label);
      std::vector<Object> get_objects();
      bool object_exists(std::string label);
   public:
      //MAP
      int process_map(GLFWwindow* win, float fov, Camera camera, glm::vec3 size);
      std::vector<std::vector<int>> get_map(std::string filename);
      std::string get_map_string(std::string filename);
      int change_map(std::string filename, std::vector<std::vector<int>> map);
};


#endif
