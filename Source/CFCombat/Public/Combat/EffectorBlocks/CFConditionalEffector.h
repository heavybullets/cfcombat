// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "CFConditionalEffector.generated.h"

//fwd
class UCFConditionBlock;

/**
 * Conditionally runs the given effectors depending on whether the conditions are met or not.
 */
UCLASS()
class CFCOMBAT_API UCFConditionalEffector : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* Effectors to play. */
	UPROPERTY(EditAnywhere, Instanced, Category = "Effector")
	TArray<TObjectPtr<UCFEffectorBlock>> SubEffectors;

	/* Conditions to check against. */
	UPROPERTY(EditAnywhere, Instanced, Category = "Conditions")
	TArray<TObjectPtr<UCFConditionBlock>> SubConditions;

	/* If true, the conditions will keep being tested to see if the effectors should be triggered, otherwise the conditions are tested only for entrace and then the effector will continue triggering. */
	UPROPERTY(EditAnywhere, Category = "Conditions")
	bool bContinuousTesting;

	/* Whether this effector has been activated after its initialization. */
	bool bHasBeenActivated;

public:
	//ctor
	UCFConditionalEffector();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorInit_Implementation(const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
