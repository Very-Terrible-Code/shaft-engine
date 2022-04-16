#include "game/map.h"
#include "textures/manager.h"
#include <cstring>

void loadMAP(char *name, GAME *game)
{
    std::ifstream is(name, std::ifstream::binary);
    if (is)
    {
        game->cmap.decorationsTile.clear();
        game->cmap.collisionTile.clear();
        game->cmap.collision.clear();
        game->cmap.drawOrder.clear();

        //memcpy(&game->texm.sourcefile, name, 64);

        mapHeader fh;

        is.read(reinterpret_cast<char *>(&fh), sizeof(mapHeader));

        game->cmap.decorationsTile.resize(fh.s_dectile);
        game->cmap.collisionTile.resize(fh.s_coltile);
        game->cmap.collision.resize(fh.s_col);
        game->cmap.drawOrder.resize(fh.s_do);

        is.read(reinterpret_cast<char *>(game->cmap.decorationsTile.data()), fh.s_dectile * sizeof(dectile));
        is.read(reinterpret_cast<char *>(game->cmap.collisionTile.data()), fh.s_coltile * sizeof(coltile));
        is.read(reinterpret_cast<char *>(game->cmap.collision.data()), fh.s_col * sizeof(col));
        is.read(reinterpret_cast<char *>(game->cmap.drawOrder.data()), fh.s_do * sizeof(drawOIT));

        is.close();
        // load textures
        switch(strcmp((char*)game->texm.sourcefile, fh.texloc)){
            case 0:
            break;
            default:
            loadTextureDB(fh.texloc, game);
            break;
        }
        for(int i = 0; i < (int)game->cmap.decorationsTile.size(); i++){
            switch(game->cmap.decorationsTile[i].scr.exist){
                case 1:
                initScript(&game->cmap.decorationsTile[i].scr);
                break;
            }
        }
    }
    else
    {
        is.close();
    }
}

void saveMAP(char *name, GAME *game)
{
    std::ofstream savedata;
    savedata.open(name, std::ofstream::binary);
    if (savedata)
    {
        mapHeader fh;
        fh.s_dectile = (int)game->cmap.decorationsTile.size();
        fh.s_coltile = (int)game->cmap.collisionTile.size();
        fh.s_col = (int)game->cmap.collision.size();
        fh.s_do = (int)game->cmap.drawOrder.size();
        memcpy(&fh.texloc, &game->texm.sourcefile, 64);

        savedata.write(reinterpret_cast<char *>(&fh),
                       sizeof(mapHeader));
        savedata.write(reinterpret_cast<char *>(&game->cmap.decorationsTile[0]),
                       sizeof(dectile) * fh.s_dectile);
        savedata.write(reinterpret_cast<char *>(&game->cmap.collisionTile[0]),
                       sizeof(coltile) * fh.s_coltile);
        savedata.write(reinterpret_cast<char *>(&game->cmap.collision[0]),
                       sizeof(col) * fh.s_col);
        savedata.write(reinterpret_cast<char *>(&game->cmap.drawOrder[0]),
                       sizeof(drawOIT) * fh.s_do);
        savedata.close();
    }
    else
    {
        savedata.close();
    }
}

void removeIDB(GAME *game, int id)
{
    printf("Deleting %i\n", id);fflush(NULL);
    switch (game->cmap.drawOrder[id].type)
    {
    case S_DECTILE:
        
        for(int i = id + 1; i < (int)game->cmap.drawOrder.size(); i++){

            switch(game->cmap.drawOrder[i].type){
                case S_DECTILE:
                game->cmap.drawOrder[i].id--;
                break;
            }
        }
        game->cmap.decorationsTile.erase(game->cmap.decorationsTile.begin() + game->cmap.drawOrder[id].id);
        game->cmap.drawOrder.erase(game->cmap.drawOrder.begin() + id);
        break;

    case S_COLTILE:
        
        for(int i = id + 1; i < (int)game->cmap.drawOrder.size(); i++){

            switch(game->cmap.drawOrder[i].type){
                case S_COLTILE:
                game->cmap.drawOrder[i].id--;
                break;
            }
        }
        game->cmap.collisionTile.erase(game->cmap.collisionTile.begin() + game->cmap.drawOrder[id].id);
        game->cmap.drawOrder.erase(game->cmap.drawOrder.begin() + id);
        break;
    case S_COL:
        
        for(int i = id + 1; i < (int)game->cmap.drawOrder.size(); i++){

            switch(game->cmap.drawOrder[i].type){
                case S_COL:
                game->cmap.drawOrder[i].id--;
                break;
            }
        }
        game->cmap.collision.erase(game->cmap.collision.begin() + game->cmap.drawOrder[id].id);
        game->cmap.drawOrder.erase(game->cmap.drawOrder.begin() + id);
        break;
    }
}