#include "textures/manager.h"
#include <fstream>

void loadTextureDB(char *name, GAME *game)
{
    std::ifstream is(name, std::ifstream::binary);
    if (is)
    {
        game->texm.loadTexCheck = true;
        clearTEX(game);
        game->texm.textures.clear();
        memcpy(&game->texm.sourcefile, name, 64);

        texdbheader fh;

        is.read(reinterpret_cast<char *>(&fh), sizeof(texdbheader));

        game->texm.textures.resize(fh.texsize);

        is.read(reinterpret_cast<char *>(game->texm.textures.data()), fh.texsize * sizeof(tagtex));

        is.close();

        for (int i = 0; i < (int)game->texm.textures.size(); i++)
        {
            loadTextureFromDB(&game->texm.textures[i].texture);
        }

        game->texm.loadTexCheck = false;
    }
    else
    {
        is.close();
    }
}

void saveTextureDB(char *name, GAME *game)
{
    std::ofstream savedata;
    savedata.open(name, std::ofstream::binary);
    if (savedata)
    {
        texdbheader fh;
        fh.texsize = (int)game->texm.textures.size();

        savedata.write(reinterpret_cast<char *>(&fh),
                       sizeof(texdbheader));
        savedata.write(reinterpret_cast<char *>(&game->texm.textures[0]),
                       sizeof(tagtex) * game->texm.textures.size());
        savedata.close();
    }
    else
    {
        savedata.close();
    }
}

void clearTEX(GAME *texdata)
{
    for (int i = 0; i < (int)texdata->texm.textures.size(); i++)
    {
        glDeleteTextures(1, &texdata->texm.textures[i].texture.glLoc);
    }
}
void removeTEX(int item, GAME *texdata)
{
    glDeleteTextures(1, &texdata->texm.textures[item].texture.glLoc);
    texdata->texm.textures.erase(texdata->texm.textures.begin() + item);
}
