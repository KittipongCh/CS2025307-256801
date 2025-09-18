#version 330
out vec4 FragColor;

uniform vec3 uLightColor;
uniform vec3 uLightDirection;
uniform vec3 uAmbientLight;

in vec3 surfaceColor;
in vec3 Normal;

void main(void)
{
    // Ambient Reflection
    vec3 ambientReflection  = uAmbientLight * surfaceColor;

    // Diffuse Reflection
    vec3 lightDirection     = normalize(uLightDirection);
    float diff              = max(dot(lightDirection, Normal), 0.0);
    vec3 diffuseReflection  = uLightColor * surfaceColor * diff;

    // Result
    vec3 result = diffuseReflection + ambientReflection;
    FragColor = vec4(result, 1.0);
}