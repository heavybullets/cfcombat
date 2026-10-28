// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "GameplayTagContainer.h"
#include "CFCompareFloatParameterValue.generated.h"

/* Comparison operations we can do. */
UENUM()
enum class ECFCompareParameterOperation : uint8
{
	Equal,
	GreaterThan,
	GreaterEqualThan,
	LessThan,
	LessEqualThan
};

/**
 * Compares the value of the given parameter and returns whether the comparison is true or not.
 */
UCLASS()
class CFCOMBAT_API UCFCompareFloatParameterValue : public UCFConditionBlock
{
	GENERATED_BODY()
	
	/* The input parameter to check against. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	FGameplayTag InputParameter;

	/* The value to compare against. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bCompareWithSystemParameter;

	/* The value to compare against. */
	UPROPERTY(EditAnywhere, Category = "Condition", meta = (EditCondition = "!bCompareWithSystemParameter"))
	float CompareValue;

	/* The value to compare against. */
	UPROPERTY(EditAnywhere, Category = "Condition", meta = (EditCondition = "bCompareWithSystemParameter"))
	FGameplayTag CompareParameter;

	/* Operation to use for comparing. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	ECFCompareParameterOperation Operation;

public:
	UCFCompareFloatParameterValue();

	//~ Begin UCFConditionBlock Interface
	virtual bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock Interface
};
