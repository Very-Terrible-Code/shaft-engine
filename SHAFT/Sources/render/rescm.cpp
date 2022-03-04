#include "render/rescm.h"
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "render/stb_image.h"
tex loadTexture(const char *location, int flip)
{
    tex texture;
    texture.location = (char*)location;
    glGenTextures(1, &texture.glLoc);
    glBindTexture(GL_TEXTURE_2D, texture.glLoc);
    std::cout << "sts: " << texture.glLoc << std::endl;
 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    
    if (flip)
    {
        stbi_set_flip_vertically_on_load(true);
    }
    unsigned char *data = stbi_load(location, &texture.size.x, &texture.size.y, &texture.channeli, 0);
    if (data)
    {
        std::cout << "sex:" << texture.size.x << " and " << texture.size.y << std::endl;
        std::cout << "bex:" << texture.channeli << " and " << texture.location << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.size.x, texture.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        std::cout << glGetError() << std::endl;
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << glGetError() << std::endl;
    }
    else
    {
        printf("texture load failed");
    }
    stbi_image_free(data);
    return texture;
}