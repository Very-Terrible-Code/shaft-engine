#include "game/map.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

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

void map_eCOL_IMGUIMENU(GAME* game, dectile *dct){
    
}

void map_IMGUIMENU(GAME *game)
{

    static char *items[] = {(char *)"Decoration Tile", (char *)"Collision Tile", (char *)"Collision Box"};
    game = game;
    ImGui::Begin("MapEditor");

    if (ImGui::BeginTabBar("Map Editor"))
    {
        
        if (ImGui::BeginTabItem("Add to Scene"))
        {
            static int itemsel;
            dectile dct;
            coltile cct;
            col ct;
            ImGui::Combo("Type", &itemsel, items, IM_ARRAYSIZE(items));
            switch(itemsel){
                    case 0:
                    map_eCOL_IMGUIMENU(game, &dectile);
            }

            if(ImGui::Button("Add")){
                
            }
        }
        if (ImGui::BeginTabItem("Edit"))
        {
            for(int i = 0; i < (int)game->cmap.drawOrder.size(); i++){
                ImGui::Text("ANUS!!!!1!");
            }
        }
    }
    ImGui::End();
}