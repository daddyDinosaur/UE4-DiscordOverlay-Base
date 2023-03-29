#pragma once
#include "../Includes.h"

struct { 
	uintptr_t UGameInstance = 0x0;//GWorld -> UGameInstance
	uintptr_t ULocalPlayer = 0x0; //GWorld -> UGameInstance -> ULocalPlayer
	uintptr_t PlayerController = 0x0; //GWorld -> UGameInstance -> ULocalPlayer -> PlayerController
	uintptr_t PlayerCamaeraManager = 0x0; //GWorld -> UGameInstance -> ULocalPlayer -> PlayerCamaeraManager
	uintptr_t CameraCache = 0x0; //GWorld -> UGameInstance -> ULocalPlayer -> PlayerCamaeraManager -> CameraCache
	uintptr_t POV = 0x0; //GWorld -> UGameInstance -> ULocalPlayer -> PlayerCamaeraManager -> CameraCache -> POV
	uintptr_t ULevels = 0x0;//GWorld -> ULevels
	uintptr_t AActors = 0x0; //Unknown - Generally 0xA0
	uintptr_t AcknowledgedPawn = 0x0; //GWorld -> UGameInstance -> ULocalPlayer -> PlayerController -> AcknowledgedPawn
}Offsets;

struct {
public:
	uintptr_t mBaseAddr;
	FRotator MyCamRotation; //PlayerController->PlayerCameraManager->CameraCache.POV.Rotation;
	FVector MyCamLocation; //PlayerController->PlayerCameraManager->CameraCache.POV.Location;
	float DefaultFOV; //PlayerController->PlayerCameraManager->CameraCache.POV.FOV

	uintptr_t gWorld;
	uintptr_t LocalPlayer;
	uintptr_t PlayerController;
	uintptr_t LocalActor;
	uintptr_t CameraManager;
	FMinimalViewInfo CameraCachePOV;

	int NearbyEnemies;
	int OnlinePlayers = 0;
}Data;

namespace Hooks
{
	//Menu Stuff

	typedef void(__thiscall* tProcessEvent)(uintptr_t pObject, uintptr_t pFunction, void* params);

	//========================================================================================================================================================================

	//Cheats

	//========================================================================================================================================================================

	//Menu Stuff

	void StartUp();
	void InitializeAll();
	void ProcessEvent(uintptr_t pObject, uintptr_t pFunction, void* params);
	void* FindFunction(const char* FunctionName);
	void Initialize();
	void GetOffsets();
	void __stdcall RemoveHooks();

	//========================================================================================================================================================================
}

struct
{

}OrigHooks;

