#include <cstdio>
#include "engine.h"

int main(){
   GLFWwindow* win;
   if (!glfwInit()){
      return -1;
   }
   win = glfwCreateWindow(540, 540, "win", 0, 0); 
   glfwMakeContextCurrent(win);
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      printf("can't load openGL\n");
      glfwTerminate();
      return -1;
   }
   glClearColor(0.25f, 0.4f, 0.5f, 1.0f);

   while(!glfwWindowShouldClose(win)){
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT);
      glfwSwapBuffers(win);
   }
   glfwTerminate();
   return 0;
}
