#version 330 core
out vec4 color;

uniform vec3 icolor;

void main()
{    
    color = vec4(icolor, 1.);
}  