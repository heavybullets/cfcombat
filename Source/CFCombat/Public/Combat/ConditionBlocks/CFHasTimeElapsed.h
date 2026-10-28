// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "CFHasTimeElapsed.generated.h"

/**
 * Condition that passes the tests whenever its containing 'combat section' has been running for at least 'Duration' amount of time.
 */
UCLASS()
class CFCOMBAT_API UCFHasTimeElapsed : public UCFConditionBlock
{
	GENERATED_BODY()
	
	/* Whether to read the Duration from a parameter instead. */
	UPROPERTY(EditAnywhere, Category="Condition")
	bool bUseDurationParameter = false;
	
	/* Tag to get the duration from */
	UPROPERTY(EditAnywhere, Category="Condition", meta=(EditCondition="bUseDurationParameter", EditConditionHides))
	FGameplayTag DurationParameter; 
	
	/* How long does the parent section need to be running before letting this condition block pass. */
	UPROPERTY(EditAnywhere, Category = "Condition", meta=(EditCondition="!bUseDurationParameter", EditConditionHides))
	float Duration;

public:
	//ctor
	UCFHasTimeElapsed();

	//~ Begin UCFConditionBlock Interface
	virtual bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock Interface
};
