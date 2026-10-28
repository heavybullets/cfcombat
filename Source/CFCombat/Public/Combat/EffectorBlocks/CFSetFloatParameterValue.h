// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFSetFloatParameterValue.generated.h"

/**
 * Forces a float value on the given float parameter.
 */
UCLASS()
class CFCOMBAT_API UCFSetFloatParameterValue : public UCFEffectorBlock
{
	GENERATED_BODY()

	/* The parameter to modify. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag Parameter;
	
	/* Whether to set the value directly or randomly from a range */
	UPROPERTY(EditAnywhere, Category="Effector")
	bool bRandomInRange = false;

	/* Value to override the parameter with. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta=(EditCondition="!bRandomInRange", EditConditionHides))
	float Value;
	
	/* random range minimum value that it can have */
	UPROPERTY(EditAnywhere, Category="Effector", meta=(EditCondition="bRandomInRange", EditConditionHides))
	float RangeMin;
	
	/* random range maximum value that it can have */
	UPROPERTY(EditAnywhere, Category="Effector", meta=(EditCondition="bRandomInRange", EditConditionHides))
	float RangeMax;

	/* Whether the effector gets applied on end instead. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bApplyOnEnd;
	
public:
	//ctor
	UCFSetFloatParameterValue();


	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
