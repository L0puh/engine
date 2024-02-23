#version 330 core
out vec4 FragColor;

in vec2 aTex;
in vec3 aNormal;
in vec3 FragPos;

uniform sampler2D my_texture;
uniform vec3 obj_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
   vec3 ambient = 0.4 * light_color;
   vec3 norm = normalize(aNormal);
   
   vec3 view_dir = normalize(view_pos - FragPos);
   vec3 light_dir = normalize(light_pos - FragPos);
   vec3 reflect_dir = reflect(-light_dir, norm);

   float diff = max(dot(norm, light_dir), 0.0);
   float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);

   vec3 specular = 0.5 * spec * light_color;
   vec3 diffuse = diff * light_color;

   FragColor = vec4(obj_color * (ambient + diffuse + specular), 1.0f) * texture(my_texture, aTex);
}
