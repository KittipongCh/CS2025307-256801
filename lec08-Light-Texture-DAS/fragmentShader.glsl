#version 330
out vec4 FragColor;

uniform vec3 uLightColor;
uniform vec3 uLightDirection;
uniform vec3 uViewPosition;

// Light Properties
uniform vec3 uAmbientLight;
uniform vec3 uDiffuseLight;
uniform vec3 uSpecularLight;

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
    float spec = pow(max(dot(reflectDirection, uViewPosition), 0.0), 32);
    //vec3 specular = specularStrength * spec * surfaceColor;
    vec3 specular = uSpecularLight * spec * texture(uTexture, TextureCoord).rgb;

    // Result
    vec3 result = diffuseReflection + ambientReflection + specular;
    FragColor = vec4(result, 1.0);
}