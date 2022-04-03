#include "textures/rescm.h"

#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "render/stb_image.h"

tex loadTexture(const char *location, int flip)
{
    tex texturea;
    flip = flip;

    glGenTextures(1, &texturea.glLoc);
    glBindTexture(GL_TEXTURE_2D, texturea.glLoc);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned char *data = stbi_load(location, &texturea.size.x, &texturea.size.y, &texturea.channeli, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texturea.size.x, texturea.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        texturea.loaded = true;
    }
    else
    {
        texturea.loaded = false;
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    memcpy(texturea.location, location, strlen(location) + 1);

    return texturea;
}

void loadTextureFromDB(tex *texturea)
{
    glGenTextures(1, &texturea->glLoc);
    glBindTexture(GL_TEXTURE_2D, texturea->glLoc);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data = stbi_load((char *)texturea->location, &texturea->size.x, &texturea->size.y, &texturea->channeli, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texturea->size.x, texturea->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return;
}