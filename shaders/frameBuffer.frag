#version 410 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

void main()
{
    const float gamma = 2.2;
    vec3 fragment = texture(screenTexture, texCoords).rgb;
    vec3 bloom = texture(bloomTexture, texCoords).rgb;

    vec3 color = fragment + bloom;

    float exposure = 0.8f;
    vec3 toneMapped = vec3(1.0f) - exp(-color * exposure);

    FragColor.rgb = pow(toneMapped, vec3(1.0f / gamma));
}