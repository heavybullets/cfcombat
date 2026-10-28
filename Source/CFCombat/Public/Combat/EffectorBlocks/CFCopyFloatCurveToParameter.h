// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFCopyFloatCurveToParameter.generated.h"

//fwd
class UCurveFloat;

/**
 * Copies the value of the given float curve into a parameter.
 */
UCLASS()
class CFCOMBAT_API UCFCopyFloatCurveToParameter : public UCFEffectorBlock
{
	GENERATED_BODY()

	/* Contains the data for the movement curve. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	TObjectPtr<UCurveFloat> InputCurve;

	/* If present a system parameter to scale the movement with. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag OutputParameter;

	/* A constant value to scale the curve as a whole. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	float Scale;

	/* If present a system parameter to scale the movement with. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag ScaleParameter;

	/* Whether we should scale the curve to match the duration of this effector. Assumes a normalized curve from [0,1]. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bScaleTimeToDuration;

public:
	//ctor
	UCFCopyFloatCurveToParameter();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
	
};
