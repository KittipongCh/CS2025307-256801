#version 330
out vec4 FragColor;

uniform vec3 uLightColor;
uniform vec3 uLightDirection;
uniform vec3 uViewPosition;

uniform vec3 uAmbientLight;
uniform vec3 uDiffuseLight;
uniform vec3 uSpeculaLight;

uniform vec3 uMaterialAmbient;
uniform vec3 uMaterialDiffuse;
uniform vec3 uMaterialSpecular;
uniform float uMaterialShininess;

in vec3 surfaceColor;
in vec3 Normal;
in vec3 surfacePosition;

void main(void)
{
    // Ambient Reflection
    vec3 ambientReflection  = uLightColor * uMaterialAmbient;

    // Diffuse Reflection
    vec3 lightDirection     = normalize(uLightDirection - surfacePosition);
    float diff              = max(dot(lightDirection, Normal), 0.0);
    vec3 diffuseReflection  = uLightColor * (diff * uMaterialDiffuse);

    // Specular
    vec3 viewDirection      = normalize(uViewPosition - surfacePosition);
    vec3 reflectDirection   = reflect(lightDirection, Normal);
    float spec              = pow(max(dot(reflectDirection, viewDirection), 0.0), uMaterialShininess);
    vec3 specular           = uSpeculaLight * (spec * uMaterialSpecular);

    // Result
    vec3 result = diffuseReflection + ambientReflection + specular;
    FragColor = vec4(result, 1.0);
}