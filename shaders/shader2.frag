#version 330 core
out vec4 FragColor;
in vec3 my_color;

void main()
{
   FragColor = vec4(my_color, 1.0f);
}
