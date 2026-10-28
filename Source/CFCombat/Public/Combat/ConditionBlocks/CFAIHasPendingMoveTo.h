// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "CFAIHasPendingMoveTo.generated.h"

/**
 * Returns true if the owner AI has a pending 'move to' currently being processed.
 */
UCLASS()
class CFCOMBAT_API UCFAIHasPendingMoveTo : public UCFConditionBlock
{
	GENERATED_BODY()
	
public:
	//~ Begin UCFConditionBlock Interface
	virtual bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock Interface
};
