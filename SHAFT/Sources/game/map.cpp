#include "game/map.h"

void loadMAP(char *name, GAME *game)
{
    name = name;
    game = game;
}

void saveMAP(char *name, GAME *game)
{
    name = name;
    game = game;
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