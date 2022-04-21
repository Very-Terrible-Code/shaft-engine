#include "game/phys.h"

void physUpdate(GAME *game)
{
    static float timeStep = 1.0f / 60.0f;

        for (int i = 0; i < (int)game->cmap.tiles.size(); i++)
        {
            switch (game->cmap.tiles[i].phys.physOn)
            {
            case true:
                game->cmap.tiles[i].pos.x = cpBodyGetPosition(game->cmap.tiles[i].phys.body).x;
                game->cmap.tiles[i].pos.y = cpBodyGetPosition(game->cmap.tiles[i].phys.body).y;
                game->cmap.tiles[i].rot = cpBodyGetAngle(game->cmap.tiles[i].phys.body);
                printf("Body is at (%5.2f, %5.2f). It's velocity is (%5.2f, %5.2f)\n",
                       cpBodyGetPosition(game->cmap.tiles[i].phys.body).x, cpBodyGetPosition(game->cmap.tiles[i].phys.body).y, cpBodyGetVelocity(game->cmap.tiles[i].phys.body).x, cpBodyGetVelocity(game->cmap.tiles[i].phys.body).y);
                break;
            }
            cpSpaceStep(game->world, timeStep);
        }
    

}

void physInit(GAME *game)
{
    game->world = cpSpaceNew();
    cpSpaceSetGravity(game->world, game->cmap.gravity);
    for (int i = 0; i < (int)game->cmap.tiles.size(); i++)
    {
        switch (game->cmap.tiles[i].phys.physOn)
        {
        case true:
            if (game->cmap.tiles[i].phys.dynst == STATIC)
            {
                game->cmap.tiles[i].phys.shape = cpSegmentShapeNew(cpSpaceGetStaticBody(game->world), cpv(-20, 5), cpv(20, -5), 0);
                cpShapeSetFriction(game->cmap.tiles[i].phys.shape, 1);
                cpSpaceAddShape(game->world, game->cmap.tiles[i].phys.shape);

                printf("Static object created.\n");
                fflush(NULL);
            }
            else if (game->cmap.tiles[i].phys.dynst == DYNAMIC)
            {
                game->cmap.tiles[i].phys.inert = cpMomentForBox(game->cmap.tiles[i].phys.mass, game->cmap.tiles[i].scl.x, game->cmap.tiles[i].scl.y);
                game->cmap.tiles[i].phys.body = cpSpaceAddBody(game->world, cpBodyNew(game->cmap.tiles[i].phys.mass, game->cmap.tiles[i].phys.inert));
                cpBodySetPosition(game->cmap.tiles[i].phys.body, cpv(game->cmap.tiles[i].pos.x, game->cmap.tiles[i].pos.y));

                game->cmap.tiles[i].phys.shape = cpSpaceAddShape(game->world, cpBoxShapeNew(game->cmap.tiles[i].phys.body, game->cmap.tiles[i].scl.x, game->cmap.tiles[i].scl.y, 5.f));
                cpShapeSetFriction(game->cmap.tiles[i].phys.shape, game->cmap.tiles[i].phys.friction);

                printf("Dynamic object created.\n");
                fflush(NULL);
            }

            break;
        }
    }
}