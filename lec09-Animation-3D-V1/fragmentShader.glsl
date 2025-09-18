#version 330
out vec4 FragColor;

in vec3 objectColor;

void main(void)
{
    FragColor = vec4(objectColor, 1.0);
}