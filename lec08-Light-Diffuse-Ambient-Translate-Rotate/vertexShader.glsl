#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uNormalMatrix;

out vec3 surfaceColor;
out vec3 Normal;

void main(void)
{
    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPos, 1.0);
    surfaceColor = aColor;
    Normal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1.0)));
}