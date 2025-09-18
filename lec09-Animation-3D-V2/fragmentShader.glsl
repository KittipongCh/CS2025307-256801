#version 330
out vec4 FragColor;

in vec4 objectColor;

void main(void)
{
    FragColor = objectColor;
}