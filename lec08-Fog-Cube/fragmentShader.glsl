#version 330
out vec4 FragColor;

in vec3 outColor;
in float Distance;

uniform vec3 uLightColor;
uniform vec3 uLightDirection;
uniform vec3 uAmbientLight;

uniform vec3 uFogColor;
uniform vec2 uFogDistance;

void main(void)
{
    // Fog
    float fog = (uFogDistance.y - Distance)/(uFogDistance.y - uFogDistance.x);
    float fogFactor = clamp(fog, 0.0, 1.0);

    // diffuse 
    //vec3 lightDirection = normalize(uLightDirection);
    //float diff = max(dot(Normal, lightDirection), 0.0);
    //vec3 diffuse = uLightColor * outColor * diff;

    // ambient
    vec3 ambient = uAmbientLight * outColor;

    vec3 result = mix(uFogColor, ambient, fogFactor);
    FragColor = vec4(result, 1.0);
}