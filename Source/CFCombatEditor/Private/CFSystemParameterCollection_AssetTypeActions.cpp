// Copyright Critical Failure Studio Ltd.

#include "CFSystemParameterCollection_AssetTypeActions.h"
#include "Combat/CFSystemParameterCollection.h"

#define LOCTEXT_NAMESPACE "CFCombatEditor"

FCFSystemParameterCollection_AssetTypeActions::FCFSystemParameterCollection_AssetTypeActions(EAssetTypeCategories::Type InAssetCategory) :
	CustomAssetCategory(InAssetCategory)
{}

FText FCFSystemParameterCollection_AssetTypeActions::GetName() const
{ 
	return LOCTEXT("CFSystemParameterCollection_AssetTypeActions_GetName", "System Parameter Collection");
}

FColor FCFSystemParameterCollection_AssetTypeActions::GetTypeColor() const  
{ 
	return FColor(0, 175, 255); 
}

UClass* FCFSystemParameterCollection_AssetTypeActions::GetSupportedClass() const
{
	return UCFSystemParameterCollection::StaticClass();
}

void FCFSystemParameterCollection_AssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	//For now only the default asset editor
	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}

uint32 FCFSystemParameterCollection_AssetTypeActions::GetCategories()
{
	return CustomAssetCategory;
}

FText FCFSystemParameterCollection_AssetTypeActions::GetAssetDescription(const FAssetData& AssetData) const
{
	return LOCTEXT("CFSystemParameterCollection_AssetTypeActions_GetDescription", "Defines a list of parameters that can be shared be shared by conditions and effectors inside a combat system to drive logic.");
}

#undef LOCTEXT_NAMESPACE
