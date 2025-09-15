#version 330
out vec4 FragColor;
in vec4 ourColor;
void main(void)
{
    FragColor = ourColor;
}