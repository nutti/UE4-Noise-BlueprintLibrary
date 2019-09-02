// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NoiseBlueprintLibrary.h"

#define LOCTEXT_NAMESPACE "FNoiseBlueprintLibraryModule"

void FNoiseBlueprintLibraryModule::StartupModule()
{
	// nothing to do here ...
}

void FNoiseBlueprintLibraryModule::ShutdownModule()
{
	// nothing to do here ...
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNoiseBlueprintLibraryModule, NoiseBlueprintLibrary)
