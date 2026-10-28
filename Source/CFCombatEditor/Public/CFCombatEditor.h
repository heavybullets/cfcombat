// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

//fwd
class FCFCombatSystem_AssetTypeActions;
class FCFActionIntent_AssetTypeActions;
class FCFSystemParameterCollection_AssetTypeActions;

/* Main editor module for the plugin. */
class FCFCombatEditorModule : public IModuleInterface
{
	//Asset type actions
	TSharedPtr<class FCFCombatSystem_AssetTypeActions> CombatSystemAssetTypeActions;
	TSharedPtr<class FCFActionIntent_AssetTypeActions> ActionIntentAssetTypeActions;
	TSharedPtr<class FCFSystemParameterCollection_AssetTypeActions> SystemParamsAssetTypeActions;

	//Category Key for registering an advanced category
	static const FName CategoryKey;

	//Display name of the category
	static const FText CategoryDisplayName;

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	//Asset action related
	void RegisterAssetActions();
	void UnregisterAssetActions();
};
