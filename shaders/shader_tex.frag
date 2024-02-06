#version 330 core

out vec4 FragColor;
in vec2 Tex;

uniform sampler2D my_texture;
uniform sampler2D my_texture2;
void main()
{
   FragColor = mix(texture(my_texture, Tex), texture(my_texture2, Tex), 0.4);
}
