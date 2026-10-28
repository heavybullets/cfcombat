// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "CFLogicOr.generated.h"

/**
 * A condition block that triggers if one of its contained condition blocks returns true.
 */
UCLASS()
class CFCOMBAT_API UCFLogicOr : public UCFConditionBlock
{
	GENERATED_BODY()
	
	/* List of conditions to have inside this 'or' block. */
	UPROPERTY(EditAnywhere, Instanced, Category = "Condition")
	TArray<TObjectPtr<UCFConditionBlock>> Conditions;

public:
	//~ Begin UCFConditionBlock Interface
	virtual void OnConditionInit_Implementation(const FCFConditionBlockContext& Context) override;
	virtual bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock Interface
};
