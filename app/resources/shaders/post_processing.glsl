//#shader vertex
#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPosition, 0.0, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool effectEnabled;
uniform float vignetteStrength;

void main() {
    vec3 color = texture(screenTexture, TexCoords).rgb;

    float distanceFromCenter = length(TexCoords - vec2(0.5));
    float edgeAmount = smoothstep(0.25, 0.70, distanceFromCenter);
    float strength = effectEnabled ? vignetteStrength : 0.0;
    color *= 1.0 - edgeAmount * strength;

    FragColor = vec4(color, 1.0);
}
