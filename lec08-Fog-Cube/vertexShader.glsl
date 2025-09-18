#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform vec4 uEye;

out vec3 outColor;
out float Distance;

void main(void)
{
    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPos, 1.0);
    outColor = aColor;
    Distance = distance(uModelMatrix * vec4(aPos, 1.0), uEye);
}