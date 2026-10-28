// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFApplyScalarMaterialCurve.generated.h"

//fwd
class UCurveFloat;

/**
 * Applies the given float curve to the dynamic material defined as a combat system parameter.
 */
UCLASS()
class CFCOMBAT_API UCFApplyScalarMaterialCurve : public UCFEffectorBlock
{
	GENERATED_BODY()

	/* The system parameter that holds the dynamic material we wish to modify. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag SystemParameterTag;
	
	/* Contains the data for the movement curve. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	TObjectPtr<UCurveFloat> ParameterCurve;

	/* A constant value to scale the curve as a whole. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	float ParameterScale;

	/* Name of the material parameter we are modifying. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FName ParameterName;

	/* Whether we should scale the movement curve to match the duration of this effector. Assumes a normalized curve from [0,1]. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bScaleTimeToDuration;

	/* When exiting, do we want to force the parameter to a hard reset? */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bResetOnEnd;

	/* Value to reset the parameter to. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	float ResetValue;

public:
	//ctor
	UCFApplyScalarMaterialCurve();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
