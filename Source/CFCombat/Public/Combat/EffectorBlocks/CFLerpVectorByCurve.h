// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFLerpVectorByCurve.generated.h"

//fwd
class UCurveFloat;

/**
 * Interpolates two vectors by using the given float curve as the alpha and stores the result on a system parameter.
 */
UCLASS()
class CFCOMBAT_API UCFLerpVectorByCurve : public UCFEffectorBlock
{
	GENERATED_BODY()

	/* System parameter tag we are using to store the result. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag TargetParameterTag;
	
	/* Contains the data for the curve. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	TObjectPtr<UCurveFloat> AlphaCurve;

	/* If true, we will use another system parameter to read the 'start'. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bUseStartParameterTag;

	/* The system parameter tag that holds the scene component to rotate. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bUseStartParameterTag"))
	FGameplayTag StartParameterTag;

	/* The system parameter tag that holds the scene component to rotate. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "!bUseStartParameterTag"))
	FVector StartValue;

	/* If true, we will use another system parameter to read the 'start'. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bUseEndParameterTag;

	/* The system parameter tag that holds the scene component to rotate. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bUseEndParameterTag"))
	FGameplayTag EndParameterTag;

	/* The system parameter tag that holds the scene component to rotate. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "!bUseStartParameterTag"))
	FVector EndValue;

	/* Whether we should scale the curve to match the duration of this effector. Assumes a normalized curve from [0,1]. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bScaleTimeToDuration;

public:
	//ctor
	UCFLerpVectorByCurve();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
