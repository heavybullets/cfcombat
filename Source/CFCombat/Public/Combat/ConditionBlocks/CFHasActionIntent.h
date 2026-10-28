// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "Combat/CFActionIntent.h"
#include "CFHasActionIntent.generated.h"

/**
 * Condition that passes the test then the action buffer that is related to the current section has the defined intent action anywhere on it.
 * This simply checks that the intent is present and doesn't care about order or a chain of intents, for that use other condition blocks.
 */
UCLASS()
class CFCOMBAT_API UCFHasActionIntent : public UCFConditionBlock
{
	GENERATED_BODY()
	
	/* Intent to check for. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	TObjectPtr<UCFActionIntent> Intent;

	/* Type of entry to search for. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	ECFActionIntentBufferEntryType EntryType;

public:
	//ctor
	UCFHasActionIntent();

	//~ Begin UCFConditionBlock Interface
	virtual bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock Interface
};
