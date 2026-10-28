// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "CFGameplayAttributeSet.generated.h"

/**
 * Holds the definitions of the attributes to be stored by an attribute component.
 */
USTRUCT(BlueprintType)
struct CFCOMBAT_API FCFGameplayAttributeSet : public FTableRowBase
{
	GENERATED_BODY()

private:
	/* The map between gameplay attributes and their default values. */
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TMap<FGameplayTag, float> AttributeMap;

public:
	/* Obtain the attribute map as a const reference. */
	FORCEINLINE const TMap<FGameplayTag, float>& GetAttributeMap() const { return AttributeMap; }
};
