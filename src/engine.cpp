#include "engine.h"
#include "utils.h"

void frame_buffer_size(GLFWwindow* window, int width, int height){
   glViewport(0, 0, width, height);
   utils::log("changed view port");
}
