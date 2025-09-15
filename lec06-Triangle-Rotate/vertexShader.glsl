#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float uCos_Phi;
uniform float uSin_Phi;

out vec3 ourColor;

void main(void)
{
    gl_Position.x = aPos.x * uCos_Phi - aPos.y * uSin_Phi;
	gl_Position.y = aPos.x * uSin_Phi + aPos.y * uCos_Phi;
	gl_Position.z = aPos.z;
	gl_Position.w = 1.0;
    ourColor = aColor;
}