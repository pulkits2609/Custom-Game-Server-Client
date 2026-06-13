// Copyright Epic Games, Inc. All Rights Reserved.

#include "Custom_Game_Server_Client.h"

#define LOCTEXT_NAMESPACE "FCustom_Game_Server_ClientModule"

void FCustom_Game_Server_ClientModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCustom_Game_Server_ClientModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustom_Game_Server_ClientModule, Custom_Game_Server_Client)