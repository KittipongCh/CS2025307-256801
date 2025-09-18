#version 330
out vec4 FragColor;

in vec3 outColor;
in vec2 TextureCoord;

uniform sampler2D texture1;

void main(void)
{
    //FragColor = vec4(outColor, 1.0);
    FragColor = texture(texture1, TextureCoord);
}