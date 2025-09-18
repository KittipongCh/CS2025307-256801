#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 uModelMatrixLightCube;
uniform mat4 uViewMatrixLightCube;
uniform mat4 uProjMatrixLightCube;

void main(void)
{
    gl_Position = uProjMatrixLightCube * uViewMatrixLightCube * uModelMatrixLightCube * vec4(aPos, 1.0);
}