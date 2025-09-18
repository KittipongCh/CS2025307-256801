#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

//uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

out vec3 outColor;
out vec3 Normal;

void main(void)
{
    gl_Position = uProjMatrix * uViewMatrix * vec4(aPos, 1.0);
    outColor = aColor;
    Normal = normalize(aNormal);
}