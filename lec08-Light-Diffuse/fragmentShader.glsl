#version 330
out vec4 FragColor;

uniform vec3 uLightColor;
uniform vec3 uLightDirection;

in vec3 outColor;
in vec3 Normal;

void main(void)
{
    // Diffuse
    vec3 lightDirection     = normalize(uLightDirection);
    float diff              = max(dot(lightDirection, Normal), 0.0);
    vec3 diffuseReflection  = uLightColor * outColor * diff;
    FragColor = vec4(diffuseReflection, 1.0);
}