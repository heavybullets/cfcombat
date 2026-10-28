// Copyright Critical Failure Studio Ltd.

#include "CFActionIntent_AssetTypeActions.h"
#include "Combat/CFActionIntent.h"

#define LOCTEXT_NAMESPACE "CFCombatEditor"

FCFActionIntent_AssetTypeActions::FCFActionIntent_AssetTypeActions(EAssetTypeCategories::Type InAssetCategory) :
	CustomAssetCategory(InAssetCategory)
{}

FText FCFActionIntent_AssetTypeActions::GetName() const
{ 
	return LOCTEXT("CFActionIntent_AssetTypeActions_GetName", "Action Intent");
}

FColor FCFActionIntent_AssetTypeActions::GetTypeColor() const  
{ 
	return FColor(0, 175, 255); 
}

UClass* FCFActionIntent_AssetTypeActions::GetSupportedClass() const
{
	return UCFActionIntent::StaticClass();
}

void FCFActionIntent_AssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	//For now only the default asset editor
	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}

uint32 FCFActionIntent_AssetTypeActions::GetCategories()
{
	return CustomAssetCategory;
}

FText FCFActionIntent_AssetTypeActions::GetAssetDescription(const FAssetData& AssetData) const
{
	return LOCTEXT("CFActionIntent_AssetTypeActions_GetDescription", "Defines an input that can be triggered on the 'Combat System' to drive the next action to be executed.");
}

#undef LOCTEXT_NAMESPACE
