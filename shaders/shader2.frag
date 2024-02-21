#version 330 core
out vec4 FragColor;
in vec2 aTex;

uniform sampler2D my_texture;
uniform vec3 obj_color;
uniform vec3 light_color;

void main()
{
   FragColor = vec4(obj_color * light_color , 1.0f) * texture(my_texture, aTex);
}
