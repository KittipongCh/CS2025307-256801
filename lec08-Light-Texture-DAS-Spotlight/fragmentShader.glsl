#version 330
out vec4 FragColor;

uniform vec3 uLightColor;
uniform vec3 uLightDirection;
uniform vec3 uViewPosition;

// Light Properties
uniform vec3 uAmbientLight;
uniform vec3 uDiffuseLight;
uniform vec3 uSpecularLight;

// LightCutOff
uniform float uLightCutOff;

// LightOuterCutOff
uniform float uLightOuterCutOff;


// Attenuation
uniform float uKConstant;
uniform float uKLinear;
uniform float uKQuadratic;

// Texture
uniform sampler2D uTexture;

in vec3 surfaceColor;
in vec3 Normal;
in vec3 surfacePosition;
in vec2 TextureCoord;

void main(void)
{
    // Ambient Reflection
    vec3 ambientReflection  = uAmbientLight * texture(uTexture, TextureCoord).rgb;

    // Diffuse Reflection
    vec3 lightDirection     = normalize(uLightDirection - surfacePosition);
    float diff              = max(dot(lightDirection, Normal), 0.0);
    vec3 diffuseReflection  = uDiffuseLight * diff * texture(uTexture, TextureCoord).rgb;

    // Specular
    vec3 viewDirection      = normalize(uViewPosition - surfacePosition);
    vec3 reflectDirection   = reflect(-lightDirection, Normal);
    float spec = pow(max(dot(reflectDirection, uViewPosition), 0.0), 32.0);
    vec3 specular = uSpecularLight * spec * texture(uTexture, TextureCoord).rgb;

    // Attenuation
    float distance = length(uViewPosition - surfacePosition);
    float attenuation = 1.0 / (uKConstant + (uKLinear * distance) + (uKQuadratic * distance));
    diffuseReflection *= attenuation;
    ambientReflection *= attenuation;
    specular *= attenuation;

    // Spotlight - Smooth
    float theta = dot(lightDirection, normalize(-uLightDirection));
    float epsilon = uLightCutOff - uLightOuterCutOff;
    float intensity = clamp((theta - uLightOuterCutOff)/epsilon, 0.0, 1.0);
    diffuseReflection *= intensity;
    specular *= intensity;

    // Result
    vec3 result = diffuseReflection + ambientReflection + specular;
    FragColor = vec4(result, 1.0);
}