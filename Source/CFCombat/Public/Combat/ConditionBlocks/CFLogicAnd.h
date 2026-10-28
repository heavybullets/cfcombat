// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "CFLogicAnd.generated.h"

/**
 * A condition block that triggers only if all of its contained condition blocks return true.
 */
UCLASS()
class CFCOMBAT_API UCFLogicAnd : public UCFConditionBlock
{
	GENERATED_BODY()
	
	/* List of conditions to have inside this 'and' block. */
	UPROPERTY(EditAnywhere, Instanced, Category = "Condition")
	TArray<TObjectPtr<UCFConditionBlock>> Conditions;

public:
	//~ Begin UCFConditionBlock Interface
	virtual void OnConditionInit_Implementation(const FCFConditionBlockContext& Context) override;
	virtual bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock Interface
};
