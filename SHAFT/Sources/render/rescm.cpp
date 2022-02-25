#include "render/rescm.h"
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "render/stb_image.h"
tex loadTexture(const char *location, int flip)
{
    tex texture;
    glGenTextures(1, &texture.glLoc);
    glBindTexture(GL_TEXTURE_2D, (GLuint)texture.glLoc);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (flip)
    {
        stbi_set_flip_vertically_on_load(true);
    }
    unsigned char *data = stbi_load(location, &texture.size.x, &texture.size.y, &texture.channeli, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.size.x, texture.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "texture loaded at " << texture.glLoc << std::endl;
    }
    else
    {
        printf("texture load failed");
    }
    stbi_image_free(data);
    return texture;
}