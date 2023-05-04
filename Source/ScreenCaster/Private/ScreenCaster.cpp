// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScreenCaster.h"

#include <Interfaces/IPluginManager.h>

#define LOCTEXT_NAMESPACE "FScreenCasterModule"

void FScreenCasterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	const FString BasePluginDir = IPluginManager::Get().FindPlugin("ScreenCaster")->GetBaseDir();
	const FString LibExamplePath = FPaths::Combine(*BasePluginDir, TEXT("Source/ThirdParty/libconductor.dll"));
	handle = FPlatformProcess::GetDllHandle(*LibExamplePath);
	if (handle) {
		UE_LOG(LogTemp, Log, TEXT("libconductor.dll loaded successfully!"));
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("libconductor.dll failed to load!"));
	}
}

void FScreenCasterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FPlatformProcess::FreeDllHandle(handle);
	handle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FScreenCasterModule, ScreenCaster)