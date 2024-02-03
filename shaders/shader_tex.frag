#version 330 core

out vec4 FragColor;

in vec2 Tex;
uniform sampler2D my_texture;

void main()
{
   FragColor = texture(my_texture, Tex) * vec4(0.3, 0.0, 0.0, 1.0);
}
