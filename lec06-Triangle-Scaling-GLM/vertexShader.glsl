#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 uScaling;

out vec3 ourColor;

void main(void)
{
    gl_Position = uScaling * vec4(aPos, 1.0);
    ourColor = aColor;
}