#include "logic/game.h"

void gameUpdate(GAME *game)
{
    game = game;
}

void physUpdate(GAME *game)
{
    game = game;
}

void aabbCheck(GAME *game, int item, std::vector<int> *collist)
{
    SDL_Rect itemrec = {(int)game->cmap.tiles[item].pos.x, (int)game->cmap.tiles[item].pos.y, (int)game->cmap.tiles[item].scl.x, (int)game->cmap.tiles[item].scl.y};
    for (int i = 0; i < (int)game->cmap.tiles.size(); i++)
    {
        if (i != item)
        {
            SDL_Rect ibec = {(int)game->cmap.tiles[i].pos.x, (int)game->cmap.tiles[i].pos.y, (int)game->cmap.tiles[i].scl.x, (int)game->cmap.tiles[i].scl.y};
            if (SDL_HasIntersection(&itemrec, &ibec))
            {
                collist->push_back(i);
            }
        }
    }
}
