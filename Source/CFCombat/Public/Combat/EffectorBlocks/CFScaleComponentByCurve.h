// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFScaleComponentByCurve.generated.h"

//fwd
class UCurveVector;

/**
 * Applies the given curve as scale to the component defined by the system parameter.
 * If an actor is given as a dynamic parameter instead, it uses its root component.
 */
UCLASS()
class CFCOMBAT_API UCFScaleComponentByCurve : public UCFEffectorBlock
{
	GENERATED_BODY()

	/* The system parameter that holds the dynamic component we wish to modify. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag SystemParameterTag;
	
	/* Contains the data for the curve. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	TObjectPtr<UCurveVector> MovementCurve;

	/* A constant value to scale the curve as a whole. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	float CurveScale;

	/* An offset to add to the curve after multiplying by the curve scale. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FVector Offset;

	/* Whether we should scale the curve to match the duration of this effector. Assumes a normalized curve from [0,1]. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bScaleTimeToDuration;

	/* When exiting, do we want to force the parameter to a hard reset? */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bResetOnEnd;

	/* Value to reset the parameter to. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FVector ResetValue;

public:
	//ctor
	UCFScaleComponentByCurve();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
