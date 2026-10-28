// Copyright Critical Failure Studio Ltd.

#include "CFCombatSystem_AssetTypeActions.h"
#include "Combat/CFCombatSystem.h"

#define LOCTEXT_NAMESPACE "CFCombatEditor"

FCFCombatSystem_AssetTypeActions::FCFCombatSystem_AssetTypeActions(EAssetTypeCategories::Type InAssetCategory) :
	CustomAssetCategory(InAssetCategory)
{}

FText FCFCombatSystem_AssetTypeActions::GetName() const
{ 
	return LOCTEXT("CFCombatSystem_AssetTypeActions_GetName", "Combat System");
}

FColor FCFCombatSystem_AssetTypeActions::GetTypeColor() const  
{ 
	return FColor(0, 175, 255); 
}

UClass* FCFCombatSystem_AssetTypeActions::GetSupportedClass() const
{
	return UCFCombatSystem::StaticClass();
}

void FCFCombatSystem_AssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	//For now only the default asset editor
	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}

uint32 FCFCombatSystem_AssetTypeActions::GetCategories()
{
	return CustomAssetCategory;
}

FText FCFCombatSystem_AssetTypeActions::GetAssetDescription(const FAssetData& AssetData) const
{
	return LOCTEXT("CFCombatSystem_AssetTypeActions_GetDescription", "Defines the rules, transitions and effects on how a combat component will behave.");
}

#undef LOCTEXT_NAMESPACE
