#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform vec4 uColor;
out vec4 ourColor;
void main(void)
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = uColor;
}