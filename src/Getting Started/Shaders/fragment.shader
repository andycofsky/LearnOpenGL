#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 bPos;

void main()
{
    FragColor = vec4(bPos, 1.0);
}
