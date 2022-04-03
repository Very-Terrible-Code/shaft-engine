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

void map_eCOL_IMGUIMENU(GAME *game)
{
    if (game->texm.textures.size())
    {
        static dectile dct;
        static float pos[2] = {dct.pos.x, dct.pos.y};
        static float scl[2] = {dct.scl.x, dct.scl.y};
        static int tex = 0;
        static char tag[32];
        ImGui::InputText("Name", (char*)tag, IM_ARRAYSIZE(tag));
        ImGui::InputFloat2("Position", pos, "%.3f units");
        ImGui::InputFloat2("Scale", scl, "%.3f units");
        ImGui::InputFloat("Rotation", &dct.rot, 0.1, 0.15, "%.3f degrees");
        ImGui::InputInt("Texture ID:", (int*)&tex);
        if (tex > (int)game->texm.textures.size() - 1)
        {
            tex--;
        }
        if ((int)tex < 0)
        {
            tex++;
        }
        ImVec2 oldP = ImGui::GetCursorPos();
        ImGui::SameLine();
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 
        ImGui::SetCursorPos(ImVec2{ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - 70});
        ImGui::Image((void *)(intptr_t)game->texm.textures[tex].texture.glLoc, ImVec2(100, 100), uv_min, uv_max, tint_col, border_col);
        ImGui::SetCursorPos(oldP);

        if (ImGui::Button("Add"))
        {
            dectile base;
            drawOIT drawo;
            base.pos.x = pos[0];
            base.pos.y = pos[1];
            base.scl.x = scl[0];
            base.scl.y = scl[1];
            base.rot = dct.rot;
            base.tex = (unsigned int)tex;
            memcpy(&base.tag, tag, strlen(tag) + 1);
            game->cmap.decorationsTile.push_back(base);
            drawo.id = (int)game->cmap.decorationsTile.size() - 1;
            fflush(NULL);
            drawo.type = S_DECTILE;
            game->cmap.drawOrder.push_back(drawo);
        }
    }
    else
    {
        ImGui::TextColored(ImVec4{1., 0., 0., 1.}, "No Texture Database Loaded!");
    }
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
            

            ImGui::Combo("Type", &itemsel, items, IM_ARRAYSIZE(items));
            switch (itemsel)
            {
            case 0:
                map_eCOL_IMGUIMENU(game);
                break;
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Edit"))
        {
            for (int i = 0; i < (int)game->cmap.drawOrder.size(); i++)
            {
                ImGui::Text("Type: %i", game->cmap.drawOrder[i].type);
                switch (game->cmap.drawOrder[i].type)
                {
                case S_DECTILE:
                {
                    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                
                    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 
                    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   
                    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    ImGui::Image((void *)(intptr_t)game->texm.textures[game->cmap.decorationsTile[game->cmap.drawOrder[i].id].tex].texture.glLoc, ImVec2(50, 50), uv_min, uv_max, tint_col, border_col);
                    
                    ImGui::SameLine();
                    ImGui::Text((char*)game->cmap.decorationsTile[game->cmap.drawOrder[i].id].tag);
                    ImGui::Text("HEY FUCKER: %i",  game->texm.textures[game->cmap.decorationsTile[game->cmap.drawOrder[i].id].tex].texture.glLoc);
                    break;
                }
                }
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::Separator();
    ImGui::End();
}