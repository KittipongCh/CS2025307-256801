#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 umvMatrix;
uniform mat4 uProjMatrix;

out vec4 objectColor;

void main(void)
{
    gl_Position = uProjMatrix * umvMatrix * vec4(aPos, 1.0);
    objectColor = aColor;
}