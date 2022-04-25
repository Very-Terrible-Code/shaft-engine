#include "editor/debug.h"
#include "editor/mapedit.h"
#include "game/map.h"
#include "render/render.h"
#include "game/phys.h"

void map_dragOBJ(GAME *game, int item, bool axis)
{
    axis = axis;
    static bool held = false;
    static int brad = 0;
    if (!ImGui::IsKeyDown(ImGuiKey_LeftShift))
    {
		// If there are tiles in game
        if (game->cmap.tiles.size())
        {

            vec2 midpoint = game->cmap.tiles[item].pos;
            midpoint.x = midpoint.x + (game->cmap.tiles[item].scl.x / 2.);
            midpoint.y = midpoint.y + (game->cmap.tiles[item].scl.y / 2.);
            raw_drawBP(game, vec2toGLM(midpoint), glm::vec2(10., 10.), 0., glm::vec3(1., 1., 1.));
            raw_drawBP(game, glm::vec2(midpoint.x, midpoint.y - 20), glm::vec2(10., 10.), 0., glm::vec3(1., 0., 0.));
            raw_drawBP(game, glm::vec2(midpoint.x - 20, midpoint.y), glm::vec2(10., 10.), 0., glm::vec3(0., 1., 0.));

            // HELL TIME
            vec2i mouse;
            SDL_GetMouseState(&mouse.x, &mouse.y);

            raw_drawBP(game, vec2itoGLM(mouse), glm::vec2(8., 8.), 0., glm::vec3(1., 1., 1.));
            SDL_Rect phf = {mouse.x, mouse.y, 8, 8};
            SDL_Rect sax = {(int)midpoint.x, (int)midpoint.y, 10, 10};
            if (!held)
            {
                if (SDL_HasIntersection(&sax, &phf))
                {

                    if (ImGui::IsMouseDown(0))
                    {
                        brad = 0;
                        held = true;
                    }
                }

                sax = {(int)midpoint.x, (int)midpoint.y - 20, 10, 10};
                if (SDL_HasIntersection(&sax, &phf))
                {

                    if (ImGui::IsMouseDown(0))
                    {
                        brad = 1;
                        held = true;
                    }
                }

                sax = {(int)midpoint.x - 20, (int)midpoint.y, 10, 10};
                if (SDL_HasIntersection(&sax, &phf))
                {

                    if (ImGui::IsMouseDown(0))
                    {
                        brad = 2;
                        held = true;
                    }
                }
            }
            else
            {
                vec2 br = vec2itoVec2(mouse);
                if (!axis)
                {
                    switch (brad)
                    {
                    case 0:

                        br.x -= (game->cmap.tiles[item].scl.x / 2.);
                        br.y -= (game->cmap.tiles[item].scl.y / 2.);
                        game->cmap.tiles[item].pos = br;
                        break;
                    case 1:
                        br.y -= (game->cmap.tiles[item].scl.y / 2.);
                        br.y += 20;
                        br.x = game->cmap.tiles[item].pos.x;
                        game->cmap.tiles[item].pos = br;
                        break;
                    case 2:
                        br.x -= (game->cmap.tiles[item].scl.x / 2.);
                        br.x += 20;
                        br.y = game->cmap.tiles[item].pos.y;
                        game->cmap.tiles[item].pos = br;
                        break;
                    }
                }
                else
                {
                    switch (brad)
                    {
                    case 1:
                        sax = {(int)game->cmap.tiles[item].pos.x, (int)game->cmap.tiles[item].pos.y + (int)game->cmap.tiles[item].scl.y, (int)game->cmap.tiles[item].scl.x, (int)game->cmap.tiles[item].scl.y};
                        if (SDL_HasIntersection(&sax, &phf))
                        {
                            game->cmap.tiles[item].pos.y += game->cmap.tiles[item].scl.y;
                        }
                        sax = {(int)game->cmap.tiles[item].pos.x, (int)game->cmap.tiles[item].pos.y - (int)game->cmap.tiles[item].scl.y, (int)game->cmap.tiles[item].scl.x, (int)game->cmap.tiles[item].scl.y};
                        if (SDL_HasIntersection(&sax, &phf))
                        {
                            game->cmap.tiles[item].pos.y -= game->cmap.tiles[item].scl.y;
                        }
                        break;
                    case 2:
                        sax = {(int)game->cmap.tiles[item].pos.x + (int)game->cmap.tiles[item].scl.x, (int)game->cmap.tiles[item].pos.y, (int)game->cmap.tiles[item].scl.x, (int)game->cmap.tiles[item].scl.y};
                        if (SDL_HasIntersection(&sax, &phf))
                        {
                            game->cmap.tiles[item].pos.x += game->cmap.tiles[item].scl.x;
                        }
                        sax = {(int)game->cmap.tiles[item].pos.x - (int)game->cmap.tiles[item].scl.x, (int)game->cmap.tiles[item].pos.y, (int)game->cmap.tiles[item].scl.x, (int)game->cmap.tiles[item].scl.y};
                        if (SDL_HasIntersection(&sax, &phf))
                        {
                            game->cmap.tiles[item].pos.x -= game->cmap.tiles[item].scl.x;
                        }
                        break;
                    }
                }
                if (ImGui::IsMouseReleased(0))
                {
                    held = false;
                }
            }
        }
    }
}

void map_IMGUIMENUBY(GAME *game)
{
    if (game->texm.textures.size())
    {
        static tile dct;
        dct.scl.x = 100;

        dct.scl.y = 100;
        static float pos[2] = {dct.pos.x, dct.pos.y};
        static float scl[2] = {dct.scl.x, dct.scl.y};
        static int tex = 0;
        static char tag[32] = "Hello!";
        ImGui::InputText("Name", (char *)tag, IM_ARRAYSIZE(tag));
        ImGui::InputFloat2("Position", pos, "%.3f units");
        ImGui::InputFloat2("Scale", scl, "%.3f units");
        ImGui::InputFloat("Rotation", &dct.rot, 0.1, 0.15, "%.3f degrees");
        ImGui::InputInt("Texture ID", (int *)&tex);
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
            tile base;
            base.pos.x = pos[0];
            base.pos.y = pos[1];
            base.scl.x = scl[0];
            base.scl.y = scl[1];
            base.rot = dct.rot;
            base.phys.physOn = false;
            base.scr.impl = NULL;
            base.scr.exist = 0;
            base.tex = (unsigned int)tex;

            base.phys.dynst = 0;
            base.phys.friction = 0.;
            base.phys.mass = 0.;

            memcpy(&base.tag, tag, strlen(tag) + 1);
            game->cmap.tiles.push_back(base);
        }
    }
    else
    {
        ImGui::TextColored(ImVec4{1., 0., 0., 1.}, "No Texture Database Loaded!");
    }
}

void map_IMGUIDISPLAYDCT(GAME *game, int id, int *sel)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::TreeNodeEx((char *)game->cmap.tiles[id].tag, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

    if (ImGui::Button(DB_imIDGEN((char *)"Delete", id)))
    {
        removeIDB(game, id);
        switch(game->cmap.tiles.size()){
        case 0:
        break;
        default:
        *sel -= 1;
        break;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button(DB_imIDGEN((char *)"Select", id)))
    {
        *sel = id;
    }

    if (*sel == id)
    {

        ImGui::TextColored(ImVec4{0., 1., 0., 1.}, "Selected!");
    }
    std::string dw = DB_imIDGEN((char *)"Duplicate", id);
    if (ImGui::Button(dw.c_str()))
    {
        game->cmap.tiles.push_back(game->cmap.tiles[id]);
    }

    ImGui::TableNextColumn();
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImGui::Image((void *)(intptr_t)game->texm.textures[game->cmap.tiles[id].tex].texture.glLoc, ImVec2(50, 50), uv_min, uv_max, tint_col, border_col);

    ImGui::TableNextColumn();

    if (ImGui::Button(DB_imIDGEN((char *)"^", id)))
    {
        if (id != 0)
        {
            std::swap(game->cmap.tiles[id], game->cmap.tiles[id - 1]);
        }
    }
    if (ImGui::Button(DB_imIDGEN((char *)"V", id)))
    {
        if (id != (int)game->cmap.tiles.size() - 1)
        {
            std::swap(game->cmap.tiles[id], game->cmap.tiles[id + 1]);
        }
    }
    ImGui::TableNextColumn();
    ImGui::Text("Pos: %fx%f", game->cmap.tiles[id].pos.x, game->cmap.tiles[id].pos.y);
    ImGui::Text("Size: %fx%f", game->cmap.tiles[id].scl.x, game->cmap.tiles[id].scl.y);
    ImGui::Text("Rotation: %f", game->cmap.tiles[id].rot);

    std::string nameds = DB_imIDGEN((char *)"Edit", id);
    if (ImGui::CollapsingHeader(nameds.c_str()))
    {

        float *pos[2] = {&game->cmap.tiles[id].pos.x, &game->cmap.tiles[id].pos.y};
        float *scl[2] = {&game->cmap.tiles[id].scl.x, &game->cmap.tiles[id].scl.y};
        std::string bs = DB_imIDGEN((char *)"Name", id);
        std::string bd = DB_imIDGEN((char *)"Position", id);
        std::string ba = DB_imIDGEN((char *)"Scale", id);
        std::string bz = DB_imIDGEN((char *)"Rotation", id);
        std::string bt = DB_imIDGEN((char *)"Texture ID", id);
        ImGui::InputText(bs.c_str(), (char *)game->cmap.tiles[id].tag, 32);
        ImGui::InputFloat2(bd.c_str(), *pos, "%.3f units");
        ImGui::InputFloat2(ba.c_str(), *scl, "%.3f units");
        ImGui::InputFloat(bz.c_str(), &game->cmap.tiles[id].rot, 0.1, 0.15, "%.3f degrees");
        ImGui::InputInt(bt.c_str(), &game->cmap.tiles[id].tex);
    }
    ImGui::TableNextColumn();
    std::string vm = DB_imIDGEN((char *)"Physics", id);
    ImGui::Checkbox(vm.c_str(), &game->cmap.tiles[id].phys.physOn);
    if (game->cmap.tiles[id].phys.physOn)
    {
        //ImGui::InputFloat(DB_imIDGEN((char *)"Mass", id), &game->cmap.tiles[id].phys.mass);

        std::string as = DB_imIDGEN((char *)"Static", id);
        std::string ad = DB_imIDGEN((char *)"Dynamic", id);
        std::string ms = DB_imIDGEN((char *)"Mass", id);
        std::string md = DB_imIDGEN((char *)"Friction", id);
        if (ImGui::Button(as.c_str()))
        {
            game->cmap.tiles[id].phys.dynst = DYNAMIC;
        }
        ImGui::SameLine();
        if (ImGui::Button(ad.c_str()))
        {
            game->cmap.tiles[id].phys.dynst = STATIC;
        }
        switch (game->cmap.tiles[id].phys.dynst)
        {
        case 1:
            ImGui::Text("Set to: Static");
            break;
        case 0:
            ImGui::Text("Set to: Dynamic");
            break;
        default:
            ImGui::Text("Set to: None, somehow");
            break;
        }
        ImGui::InputFloat(ms.c_str(), (float *)&game->cmap.tiles[id].phys.mass);

        ImGui::InputFloat(md.c_str(), (float *)&game->cmap.tiles[id].phys.friction);
    }
    ImGui::TableNextColumn();
    if (game->cmap.tiles[id].scr.exist == 0)
    {
        ImGui::Text("No script added.");
        std::string g = DB_imIDGEN((char *)"Add Script", id);
        if (ImGui::Button(g.c_str()))
        {
            initScript(&game->cmap.tiles[id].scr);
            game->cmap.tiles[id].scr.exist = 1;
        }
    }
    else
    {
        std::string g = DB_imIDGEN((char *)"Run Script", id);
        std::string a = DB_imIDGEN((char *)"Remove", id);
        if (ImGui::Button(g.c_str()))
        {
            runScriptEmb(&game->cmap.tiles[id].scr);
        }
        ImGui::SameLine();
        if (ImGui::Button(a.c_str()))
        {
            game->cmap.tiles[id].scr.impl = NULL;
            game->cmap.tiles[id].scr.exist = 0;
        }
        ImGui::InputTextMultiline("Edit", (char *)game->cmap.tiles[id].scr.script, IM_ARRAYSIZE(game->cmap.tiles[id].scr.script));
    }
}

// ImGui Gui setup
void map_IMGUIMENU(GAME *game)
{
    static bool axise = 1;
    static int selectedItem = 0;
    static char tempolocsave[64] = "";

    map_dragOBJ(game, selectedItem, axise);
    if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
    {

        if (ImGui::IsMouseClicked(0))
        {
            if (!ImGui::IsAnyItemFocused())
            {

                for (int i = 0; i < (int)game->cmap.tiles.size(); i++)
                {
                    vec2i mouse;
                    SDL_GetMouseState(&mouse.x, &mouse.y);
                    SDL_Rect sax = {(int)game->cmap.tiles[i].pos.x, (int)game->cmap.tiles[i].pos.y, (int)game->cmap.tiles[i].scl.x, (int)game->cmap.tiles[i].scl.y};
                    SDL_Rect phf = {mouse.x, mouse.y, 10, 10};
                    if (SDL_HasIntersection(&sax, &phf))
                    {
                        selectedItem = i;
                    }
                }
            }
        };
    }

    ImGui::Begin("MapEditor");
    ImGui::Text("Currently loaded TextureDB: %s", (char *)game->texm.sourcefile);
    ImGui::Text("Selected Object - %i", selectedItem);
    ImGui::Checkbox("Enable Grid Snap", &axise);

    if (ImGui::BeginTabBar("Map Editor"))
    {

        if (ImGui::BeginTabItem("Add to Scene"))
        {
            map_IMGUIMENUBY(game);

            ImGui::EndTabItem();
        }
        static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        if (ImGui::BeginTabItem("Edit"))
        {
            if (game->cmap.tiles.size())
            {
                if (ImGui::BeginTable("edit", 6, flags))
                {
                    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
                    ImGui::TableSetupColumn("Preview", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
                    ImGui::TableSetupColumn("Move", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
                    ImGui::TableSetupColumn("Stats", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
                    ImGui::TableSetupColumn("Phys", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
                    ImGui::TableSetupColumn("Info", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
                    ImGui::TableHeadersRow();
                    for (int i = 0; i < (int)game->cmap.tiles.size(); i++)
                    {
                        map_IMGUIDISPLAYDCT(game, i, &selectedItem);
                    }
                    ImGui::EndTable();
                }
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Global Edit"))
        {
            //ImGui::InputFloat("XlGravity", (float*)(double*)&game->cmap.gravity.y);
            ImGui::InputDouble("XP Gravity", &game->cmap.gravity.y);
            if (ImGui::Button("Run Global Script"))
            {
                runScriptEmb(&game->cmap.globalscr);
            }
            ImGui::InputTextMultiline("Global Script", (char *)game->cmap.globalscr.script, IM_ARRAYSIZE(game->cmap.globalscr.script));
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Player Spawn Locations"))
        {
            if (ImGui::Button("Add spawn location"))
            {
                spwn spa;
                spa.loc = vec2{0., 0.};
                memcpy(&spa.tag, "hello!", sizeof("hello!"));
                game->cmap.spawns.push_back(spa);
            }
            if (game->cmap.spawns.size())
            {
                if (ImGui::BeginTable("locs", 2, flags))
                {
                    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
                    ImGui::TableSetupColumn("Move", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
                    ImGui::TableHeadersRow();
                    for (int i = 0; i < (int)game->cmap.spawns.size(); i++)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        if (ImGui::Button(DB_imIDGEN((char *)"Erase", i)))
                        {
                            game->cmap.spawns.erase(game->cmap.spawns.begin() + i);
                        }
                        ImGui::InputText(DB_imIDGEN((char *)"Name", i), (char *)game->cmap.spawns[i].tag, IM_ARRAYSIZE(game->cmap.spawns[i].tag));
                        ImGui::TableNextColumn();
                        float *scl[2] = {&game->cmap.spawns[i].loc.x, &game->cmap.spawns[i].loc.y};
                        std::string temp = "Set Spawn Pos##" + std::to_string(i);
                        ImGui::InputFloat2(temp.c_str(), *scl, "%.3f units");
                        raw_drawBP(game, vec2toGLM(game->cmap.spawns[i].loc), glm::vec2(15), sin(((1 + i) * 10) + SDL_GetTicks() / 100.) * 100., glm::vec3(0, 1., 0));
                    }

                    ImGui::EndTable();
                }
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Level Testing"))
        {
            ImGui::TextColored(ImVec4{1., 0.6, 0., 0.7f + ((float)sin(SDL_GetTicks() / 200.f) / 2.f)}, "Your level will be saved when being tested and will be loaded back on finish");
            if (!game->cmap.spawns.size())
            {
                ImGui::TextColored(ImVec4{1, 0, 0, 1}, "Nowhere for player to spawn! Create spawn points in the Player Spawn Locations tab");
            }
            else
            {
                static int bruh;
                ImGui::InputInt("Spawn Point", &bruh);
                if (ImGui::Button("Start/Stop Testing"))
                {
                    if (game->edgameRunning == false)
                    {
                        saveMAP(tempolocsave, game);
                        physInit(game);
                        game->edgameRunning = true;
                    }
                    else
                    {
                        loadMAP(tempolocsave, game);
                        game->edgameRunning = false;
                    }
                }
            }

            if (game->edgameRunning == true)
            {
                ImGui::SameLine();
                ImGui::Text("Test Running...");
                ImGui::Text("Pos:%fx%f", game->mplay.pos.x, game->mplay.pos.y);
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Separator();

    if (game->edgameRunning == false)
    {
        ImGui::InputTextWithHint(" ", "rsc/level1.map", tempolocsave, IM_ARRAYSIZE(tempolocsave));
        ImGui::SameLine();
        if (ImGui::Button("Save Map"))
        {
            saveMAP(tempolocsave, game);
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Map"))
        {
            loadMAP(tempolocsave, game);
        }
    }

    ImGui::End();
}
