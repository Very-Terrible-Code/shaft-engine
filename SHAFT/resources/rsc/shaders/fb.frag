
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;

uniform vec2 res;


void main()
{    
    vec2 a = TexCoords.st * vec2(1.0, -1.0);
    color = vec4(vec3(1.), 1.0) * texture(image, a);
}  