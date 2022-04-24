#include "game/map.h"
#include "textures/manager.h"
#include <cstring>

void loadMAP(char *name, GAME *game)
{
    std::ifstream is(name, std::ifstream::binary);
    if (is)
    {
        game->cmap.tiles.clear();

        //memcpy(&game->texm.sourcefile, name, 64);

        mapHeader fh;

        // Load header
        is.read(reinterpret_cast<char *>(&fh), sizeof(mapHeader));
        // Interpret header
        game->cmap.gravity = fh.gravity;
        game->cmap.tiles.resize(fh.s_size);
        game->cmap.spawns.resize(fh.s_spsize);

        // Load tiles
        is.read(reinterpret_cast<char *>(game->cmap.tiles.data()), fh.s_size * sizeof(tile));
        // Load spawn locations
        is.read(reinterpret_cast<char *>(game->cmap.spawns.data()), fh.s_spsize * sizeof(spwn));

        is.close();
        // Load textures
        switch(strcmp((char*)game->texm.sourcefile, fh.texloc)){
            case 0:
            break;
            default:
            loadTextureDB(fh.texloc, game);
            break;
        }
        memcpy(&game->cmap.globalscr, &fh.globalscr, sizeof(fh.globalscr));

        // Init scripts
        initScript(&game->cmap.globalscr); // Global
        for(int i = 0; i < (int)game->cmap.tiles.size(); i++){ // Tile scripts
            switch(game->cmap.tiles[i].scr.exist){
                case 1:
                initScript(&game->cmap.tiles[i].scr);
                break;
            }
        }
    }
    else
    {
        is.close();
    }
}

// @param name Name of file
// @param game Game instance
void saveMAP(char *name, GAME *game)
{
    // Open file
    std::ofstream savedata;
    savedata.open(name, std::ofstream::binary);

    // If file opened
    if (savedata)
    {
        //  Headers
        mapHeader fh;
        fh.s_size = (int)game->cmap.tiles.size();
        fh.s_spsize = (int)game->cmap.spawns.size();
        fh.gravity.y = game->cmap.gravity.y;
        memcpy(&fh.texloc, &game->texm.sourcefile, 64);
        memcpy(&fh.globalscr.script, &game->cmap.globalscr.script, sizeof(game->cmap.globalscr.script));
        savedata.write(reinterpret_cast<char *>(&fh),
                       sizeof(mapHeader));
        
        // Tiles
        savedata.write(reinterpret_cast<char *>(&game->cmap.tiles[0]),
                       sizeof(tile) * fh.s_size);
        // Spawn locations
        savedata.write(reinterpret_cast<char *>(&game->cmap.spawns[0]),
                       sizeof(spwn) * fh.s_spsize);

        savedata.close();
    }
    else
    {
        savedata.close();
    }
}

void removeIDB(GAME *game, int id)
{
    game->cmap.tiles.erase(game->cmap.tiles.begin() + id);
}