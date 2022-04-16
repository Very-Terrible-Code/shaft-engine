#pragma once
#include "common.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"


void sh_debugMenu(GAME *game);

void ImGuiBeginRender(GAME* render);

void ImGuiEndRender();

const char* DB_imIDGEN(char* name, int id);