#version 330 core
#
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 Tex;
uniform float move;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(aPos.x + move, aPos.yz, 1.0);
   Tex = aTex;
}
