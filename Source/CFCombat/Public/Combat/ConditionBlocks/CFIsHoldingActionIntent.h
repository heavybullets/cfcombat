// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "CFIsHoldingActionIntent.generated.h"

/**
 * Checks whether the current action intent is being held on the input buffer.
 */
UCLASS()
class CFCOMBAT_API UCFIsHoldingActionIntent : public UCFConditionBlock
{
	GENERATED_BODY()
	
	/* Intent to check for. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	TObjectPtr<UCFActionIntent> Intent;

public:
	//ctor
	UCFIsHoldingActionIntent();

	//~ Begin UCFConditionBlock Interface
	virtual bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock Interface
};
