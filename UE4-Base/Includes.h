#pragma once
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <TlHelp32.h>
#include <iostream>
#include <d3d11.h>
#include <stdio.h>

#include "Helpers/Hooking/MinHook/MinHook.h"
#pragma comment(lib, "Helpers/Hooking/MinHook/MinHook.x64.lib")
#include "Helpers/Hooking/Detours/detours.h"
#pragma comment(lib, "Helpers/Hooking/detours/detours.lib")

#include "Helpers/imgui/imgui.h"
#include "Helpers/imgui/imgui_impl_win32.h"
#include "Helpers/imgui/imgui_impl_dx11.h"
#include "Helpers/imgui/imgui_internal.h"
#include "Cheat/settings.h"
#include "Helpers/Logging/Logger.h"
#include "Helpers/Memory/Memory.h"
#include "Helpers/Memory/lazyimporter.h"
#include "Helpers/Discord/Discord.h"
#include "Helpers/SDK/SDK.h"
#include "Helpers/ImGuiRendering.h"
#include "Menu/menu.h"
#include "Cheat/Hooks.h"
//#include "Cheat/Cheat.h"