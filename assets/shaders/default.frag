#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

// Texture uniforms
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

// Optional color
uniform vec4 uColor;
uniform bool useTexture;
uniform bool useColor;
uniform bool useModel;

void main()
{
    vec4 texColor = vec4(1.0);

    if (useTexture)
    {
        // For now, just sample the diffuse map
        texColor = texture(texture_diffuse1, TexCoords);
        // You could also sample specular/normal here if implementing lighting
        // vec4 specColor = texture(texture_specular1, TexCoords);
        // vec3 normal = texture(texture_normal1, TexCoords).rgb;
    }

    vec4 finalColor = texColor;

    if (useColor)
        finalColor *= uColor; // modulate with color

    FragColor = finalColor;
    if(useModel)
           FragColor = texture(texture_diffuse1, TexCoords);
           
}
