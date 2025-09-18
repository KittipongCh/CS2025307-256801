#version 330
out vec4 FragColor;

uniform vec3 uLightColor;
uniform vec3 uLightDirection;
uniform vec3 uViewPosition;

// Light properties
uniform vec3 uAmbientLight;
uniform vec3 uDiffuseLight;
uniform vec3 uSpecularLight;

// LightCutOff
uniform float uLightCutOff;

// Attenuation
uniform float uKConstant;
uniform float uKLinear;
uniform float uKQuadratic;

// Materials properties
uniform float uMaterialShininess;

in vec3 surfaceColor;
in vec3 Normal;
in vec3 surfacePosition;

void main(void)
{
    vec3 lightDirection     = normalize(uLightDirection - surfacePosition);
    float theta = dot(lightDirection, normalize(-uLightDirection));
    if(theta > uLightCutOff){
        // Ambient Reflection
        vec3 ambientReflection  = uAmbientLight * surfaceColor;

        // Diffuse Reflection
        float diff              = max(dot(lightDirection, Normal), 0.0);
        vec3 diffuseReflection  = uLightColor * surfaceColor * diff;

        // Specular
        float specularStrength = 0.5;
        vec3 viewDirection      = normalize(uViewPosition - surfacePosition);
        vec3 reflectDirection   = reflect(-lightDirection, Normal);
        float spec = pow(max(dot(reflectDirection, uViewPosition), 0.0), uMaterialShininess);
        //vec3 specular = specularStrength * spec * surfaceColor;
        vec3 specular = specularStrength * spec * uLightColor;

        // Attenuation
        float distance = length(uViewPosition - surfacePosition);
        float attenuation = 1.0 / (uKConstant + (uKLinear * distance) + (uKQuadratic * distance));
        diffuseReflection *= attenuation;
        ambientReflection *= attenuation;
        specular *= attenuation;

        // Result
        vec3 result = diffuseReflection + ambientReflection + specular;
        FragColor = vec4(result, 1.0);
    } else{
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
}