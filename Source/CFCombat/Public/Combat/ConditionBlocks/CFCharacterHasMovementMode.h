// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "CFCharacterHasMovementMode.generated.h"

/**
 * Condition that passes the test when the owner character's movement mode matches the one defined here.
 */
UCLASS()
class CFCOMBAT_API UCFCharacterHasMovementMode : public UCFConditionBlock
{
	GENERATED_BODY()

	/* Movement mode we need to match. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	TEnumAsByte<enum EMovementMode> MovementMode;

	/* If movement set to custom, which custom movement mode we should be checking.. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	uint8 CustomMovementMode;
	
public:
	//ctor 
	UCFCharacterHasMovementMode();

	//~ Begin UCFConditionBlock Interface
	virtual bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock Interface
};
