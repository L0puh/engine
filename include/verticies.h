#include <sys/types.h>
// VERTICIES: 

float vertices_cube[] = {
   //x      y  z       texture:    
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 
  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 
 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 
 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 

 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 
 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 
 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 

 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 

 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 
 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 
 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 

 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  
};

float vertices[] = {
//  x     y       z    colors 
   0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  
   0.9f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 
   0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f   
};

float vertices3[] = {
   // x   y     z          texture
    0.0f, 0.8f, 0.0f,   2.0f, 0.0f,
 -0.8f,-0.8f, 0.0f,     0.0f, 0.0f,
  0.8f,-0.8f, 0.0f,     0.0f, 2.0f,
};

float hand_verticies[] = {
   // x   y     z          texture
  0.5f,  0.5f, 0.0f,    1.0f, 1.0f, // top right      0
  0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right   1
  -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left    2
  -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left       3
};


float vertices4[] = {
  0.5f,   0.5f,  0.0f,  1.0f, 1.0f, 0.0, 0.0, -1.0f, // top right      0
  0.5f,  -0.5f,  0.0f,  1.0f, 0.0f, 0.0, 0.0, -1.0f, // bottom right   1
  -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0, 0.0, -1.0f, // bottom left    2
  -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0, 0.0, -1.0f  // top left       3
};


uint indices1[] = {
  0, 1, 3, 
  1, 2, 3  
};

float vertices_cube2[] = {

   //x      y  z       texture:     normal:
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 
 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f, 
 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 

 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,  
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f, 
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f, 
 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f, 
 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f, 

 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f, 
 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f
};
