#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// sampler2D is a GLSL built-in texture object datatype
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixValue;

void main()
{
    // GLSL built-in texture function takes a texture sampler as 1st param, texture coords as 2nd param
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-TexCoord.x, TexCoord.y)), mixValue); // flipped smiley face
}
