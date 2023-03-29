#pragma once
#include "../Includes.h"

struct
{
	bool ShowMenu = false;
	float MenuSizeX = 1000;
	float MenuSizeY = 540;
	int WindowSizeX;
	int WindowSizeY;

	struct
	{
		bool RGB = true;
		bool FOVCircle = true;
		bool Crosshair = true;

		float FOV = 90.f;
		float FOVCricleSides = 100.f;
		float FOVCircleThikkness = 1.f;
		float CrosshairThikkness = 1.f;

		struct {
			bool EnablePlayerESP = false;
		}Players;
	}Visuals;
	struct
	{

	}Aimbot;
	struct {

	}Misc;
	struct
	{
		USHORT AimbotKey = VK_RBUTTON;
		USHORT MenuKey = VK_DELETE;
	}Keybinds;
	struct {
		int GWorld = 0x498C1C0;
		int Objects = 0x496f9f0;
		int Names = 0x49baa08;
	}EngineOffsets;
	struct
	{
		ImColor Crosshair = ImColor(255, 255, 255, 255);
		ImColor FovCircle = ImColor(255, 255, 255, 255);
	}Colors;
}Settings;