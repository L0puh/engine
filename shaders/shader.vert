#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 my_color;
uniform float move;

void main()
{
   gl_Position = vec4(aPos.x+move, aPos.yz, 1.0);
   my_color = aColor;
}
