#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec4 uTranslate;

out vec3 ourColor;

void main(void)
{
    gl_Position = vec4(aPos, 1.0) + uTranslate;
    ourColor = aColor;
}