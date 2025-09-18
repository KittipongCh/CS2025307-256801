#version 330
out vec4 FragColor;
in vec3 outColor;
in vec2 outTexture;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main(void)
{
    FragColor = mix(texture(texture1, outTexture), texture(texture2, outTexture), 0.4);
}