#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;  

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;

//out vec4 fColor;

uniform sampler2D hdrBuffer;
uniform float exposure;


void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, fTexCoords).rgb;
  
    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.15)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}  
