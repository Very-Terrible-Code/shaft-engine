#include "game/game.h"

void gameUpdate(GAME *game)
{
    float currentFrame = SDL_GetTicks64();
    game->gl.deltaTime = currentFrame - game->gl.lastframe;
    game->gl.lastframe = currentFrame;  
    physUpdate(game);
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
    SDL_Rect ibec = {(int)game->mplay.pos.x,
                     (int)game->mplay.pos.y,
                     10,
                     40};
    if (SDL_HasIntersection(&itemrec, &ibec))
    {
        collist->push_back(-1);
    }
}
