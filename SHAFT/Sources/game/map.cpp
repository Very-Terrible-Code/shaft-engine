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

void map_eCOL_IMGUIMENU(GAME *game, dectile *dct)
{
    if (game->texm.textures.size())
    {
        static float *pos[2] = {&dct->pos.x, &dct->pos.y};
        static float *scl[2] = {&dct->scl.x, &dct->scl.y};
        static unsigned int tex = 0;
        game = game;
        ImGui::InputFloat2("Position", *pos, "%.3f units");
        ImGui::InputFloat2("Scale", *scl, "%.3f units");
        ImGui::InputFloat("Rotation", &dct->rot, 0., 0., "%.3f degrees");
        ImGui::InputInt("Texture ID:", (int *)&tex, 0., 0.);
        if (tex == (unsigned int)game->texm.textures.size())
        {
            tex--;
        }
        ImVec2 oldP = ImGui::GetCursorPos();
        ImGui::SameLine();
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
        ImGui::SetCursorPos(ImVec2{ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - 70});
        ImGui::Image((void *)(intptr_t)game->texm.textures[tex].texture.glLoc, ImVec2(100, 100), uv_min, uv_max, tint_col, border_col);
        ImGui::SetCursorPos(oldP);

        if (ImGui::Button("Add"))
        {
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
            static dectile dct;
            static coltile cct;
            static col ct;
            ct = ct;
            cct = cct;
            dct = dct;
            ImGui::Combo("Type", &itemsel, items, IM_ARRAYSIZE(items));
            switch (itemsel)
            {
            case 0:
                map_eCOL_IMGUIMENU(game, &dct);
                break;
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Edit"))
        {
            for (int i = 0; i < (int)game->cmap.drawOrder.size(); i++)
            {
                ImGui::Text("ANUS!!!!1!");
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::Separator();
    ImGui::End();
}