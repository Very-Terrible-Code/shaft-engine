#include "game/map.h"

void loadMAP(char *name, GAME *game)
{
    name = name;
    game = game;
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