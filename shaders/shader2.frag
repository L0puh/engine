#version 330 core
out vec4 FragColor;

in vec3 my_color;
in vec2 aTex;

uniform sampler2D my_texture;

void main()
{
   FragColor = vec4(my_color, 1.0f) * texture(my_texture, aTex);
}
