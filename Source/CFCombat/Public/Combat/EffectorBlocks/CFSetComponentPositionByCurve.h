// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFSetComponentPositionByCurve.generated.h"

//fwd
class UCurveVector;

/**
 * Reads the given curve and sets the relative location of the given scene component to it.
 * If an actor is given as a dynamic parameter instead, it uses its root component.
 */
UCLASS()
class CFCOMBAT_API UCFSetComponentPositionByCurve : public UCFEffectorBlock
{
	GENERATED_BODY()

	/* The system parameter that holds the component we wish to modify. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag SystemParameterTag;
	
	/* Contains the data for the curve. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	TObjectPtr<UCurveVector> MovementCurve;

	/* A constant value to scale the curve as a whole. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	float MovementScale;

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
	UCFSetComponentPositionByCurve();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
