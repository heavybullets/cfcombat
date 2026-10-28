// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "CFHasActionIntentChord.generated.h"

/* Entry of an intent chord. */
USTRUCT()
struct FCFIntentChordEntry
{
	GENERATED_BODY()

	/* The intent to search for. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	TObjectPtr<UCFActionIntent> Intent;

	/* If the intent chord requires activation, this intent can then activate the chord by being pressed on that frame. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bCanActivateChord;

public:
	//ctor
	FCFIntentChordEntry()
		: bCanActivateChord(false)
	{}
};

/**
 * Returns whether the user has an 'ActionIntent' chord pressed on the related buffer.
 * A chord can require activation to be considered valid, in this case the chord will activate if at least one of the 'activable' intents has been pressed that frame.
 */
UCLASS()
class CFCOMBAT_API UCFHasActionIntentChord : public UCFConditionBlock
{
	GENERATED_BODY()
	
	/* Intent to check for. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	TArray<FCFIntentChordEntry> ChordEntries;

	/**
	 * Whether the chord requires any of the 'activable' intents to be pressed in order for the chord to be considered valid. 
	 * Otherwise the chord will just require the intents to be held pressed, even if they were pressed before the buffer was active.
	 */
	 UPROPERTY(EditAnywhere, Category = "Condition")
	 bool bRequireActivation;

public:
	//ctor
	UCFHasActionIntentChord();

	//~ Begin UCFConditionBlock Interface
	virtual bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock Interface
};
