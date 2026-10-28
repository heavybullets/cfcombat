// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * Defines the actions that can be executed on a "Action Intent" asset.
 */
class FCFActionIntent_AssetTypeActions : public FAssetTypeActions_Base
{	
	/* Custom asset category used. */
	EAssetTypeCategories::Type CustomAssetCategory;

public:
	//Constructor will have the custom asset category passed
	FCFActionIntent_AssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	//~ Begin IAssetTypeActions Implementation
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override;
	//~ End IAssetTypeActions Implementation
};
