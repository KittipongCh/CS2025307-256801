#version 330
out vec4 FragColor;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform vec3 uLightDirection;
uniform vec3 uViewPosition;
uniform vec3 uAmbientLight;

void main(void)
{
    FragColor = vec4(uLightColor * uObjectColor, 1.0);
}