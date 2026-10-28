// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "GameplayTagContainer.h"
#include "CFBoolAttributeCheck.generated.h"

/**
 * Checks the owner for the given attribute tag and checks its value as a boolean.
 * Any non zero value is considered 'true'.
 */
UCLASS()
class CFCOMBAT_API UCFBoolAttributeCheck : public UCFConditionBlock
{
	GENERATED_BODY()

	/* The attribute to check on the owner. */
	UPROPERTY(EditDefaultsOnly, Category = "Condition")
	FGameplayTag AttributeTag;
	
public:
	//~ Begin UCFConditionBlock Interface
	virtual bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock Interface
};
