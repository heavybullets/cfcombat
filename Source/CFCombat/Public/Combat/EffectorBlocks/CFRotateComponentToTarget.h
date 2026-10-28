// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFRotateComponentToTarget.generated.h"

/**
 * Rotates the component defined by the system parameter tag to point towards the given vector.
 * If an actor is given as a dynamic parameter instead, it uses its root component.
 */
UCLASS()
class CFCOMBAT_API UCFRotateComponentToTarget : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* The system parameter tag that holds the scene component to rotate. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag ComponentParameterTag;

	/* If true, we will use another system parameter to read the target. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bUseSystemParameterTarget;

	/* The system parameter tag that holds the target to rotate towards. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bUseSystemParameterTarget"))
	FGameplayTag TargetParameterTag;

	/* The target when not using the target parameter tag. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "!bUseSystemParameterTarget"))
	FVector Target;

	/* If true, the targets are defined in localspace. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bLocalSpace;

	/* If true, the target is a direction instead of a position. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bTargetIsDirection;

	/* If true, the rotation resets to the given rotator. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bResetOnEnd;

	/* If true, the rotation resets to the given local rotator. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bResetOnEnd"))
	FRotator ResetLocalValue;

public:
	//ctor
	UCFRotateComponentToTarget();

	//~ Begin UCFEffectorBlock Interface
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End UCFEffectorBlock Interface
};
