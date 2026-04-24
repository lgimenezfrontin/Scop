#version 330 core

in vec3 vColor;
in vec2 vUV;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform float uBlendFactor;

void main()
{
    vec4 colorPart = vec4(vColor, 1.0);
    vec4 texturePart = texture(uTexture, vUV);
    FragColor = mix(colorPart, texturePart, uBlendFactor);
}