// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFRotateComponentByRate.generated.h"

/**
 * Rotates a component at a certain constant rate.
 * Setting an actor will use its root component instead.
 */
UCLASS()
class CFCOMBAT_API UCFRotateComponentByRate : public UCFEffectorBlock
{
	GENERATED_BODY()

	/* The system parameter tag that holds the scene component to rotate. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag ComponentParameterTag;

	/* If true, we will use another system parameter to read the rate. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bUseSystemParameterRotationRate;

	/* The system parameter tag that holds the scene component to rotate. This parameter should be a vector holding a rotator in Euler form. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bUseSystemParameterRotationRate"))
	FGameplayTag RotationRateParameterTag;

	/* The target when not using the target parameter tag. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "!bUseSystemParameterRotationRate"))
	FRotator RotationRate;
	
public:
	//ctor
	UCFRotateComponentByRate();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
