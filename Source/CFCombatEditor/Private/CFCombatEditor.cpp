// Copyright Epic Games, Inc. All Rights Reserved.

#include "CFCombatEditor.h"
#include "CFCombatSystem_AssetTypeActions.h"
#include "CFActionIntent_AssetTypeActions.h"
#include "CFSystemParameterCollection_AssetTypeActions.h"

#define LOCTEXT_NAMESPACE "FCFCombatModule"

const FName FCFCombatEditorModule::CategoryKey = FName("CFCombat_AssetActionsKey");
const FText FCFCombatEditorModule::CategoryDisplayName = FText::FromString("CFCombat");

void FCFCombatEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterAssetActions();
}

void FCFCombatEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UnregisterAssetActions();
}

void FCFCombatEditorModule::RegisterAssetActions()
{
	// Register the ZD Editor asset type actions
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type CustomType = AssetTools.RegisterAdvancedAssetCategory(FCFCombatEditorModule::CategoryKey, FCFCombatEditorModule::CategoryDisplayName);

	CombatSystemAssetTypeActions = MakeShared<FCFCombatSystem_AssetTypeActions>(CustomType);
	AssetTools.RegisterAssetTypeActions(CombatSystemAssetTypeActions.ToSharedRef());

	ActionIntentAssetTypeActions = MakeShared<FCFActionIntent_AssetTypeActions>(CustomType);
	AssetTools.RegisterAssetTypeActions(ActionIntentAssetTypeActions.ToSharedRef());

	SystemParamsAssetTypeActions = MakeShared<FCFSystemParameterCollection_AssetTypeActions>(CustomType);
	AssetTools.RegisterAssetTypeActions(SystemParamsAssetTypeActions.ToSharedRef());
}

void FCFCombatEditorModule::UnregisterAssetActions()
{
	// Register the ZD Editor asset type actions
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetTools.UnregisterAssetTypeActions(CombatSystemAssetTypeActions.ToSharedRef());
		AssetTools.UnregisterAssetTypeActions(ActionIntentAssetTypeActions.ToSharedRef());
		AssetTools.UnregisterAssetTypeActions(SystemParamsAssetTypeActions.ToSharedRef());

		CombatSystemAssetTypeActions = nullptr;
		ActionIntentAssetTypeActions = nullptr;
		SystemParamsAssetTypeActions = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCFCombatEditorModule, CFCombatEditor)