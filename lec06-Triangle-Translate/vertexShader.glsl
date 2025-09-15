#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aTranslate;

out vec3 ourColor;

void main(void)
{
    gl_Position = vec4(aPos, 1.0) + vec4(aTranslate, 0.0);
    ourColor = aColor;
}