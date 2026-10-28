// Copyright Critical Failure Studio Ltd.

#pragma once

#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "GameplayTagContainer.h"
#include "CFCompareAttributeValue.generated.h"

/* The type of comparator operation we want to use. */
UENUM()
enum class ECFCompareAttributeOperation : uint8
{
	Equal						UMETA(DisplayName="="),
	NotEqual					UMETA(DisplayName="!="),
	Less						UMETA(DisplayName="<"),
	LessOrEqual					UMETA(DisplayName="<="),
	Greater						UMETA(DisplayName=">"),
	GreaterOrEqual				UMETA(DisplayName=">="),
};

/* What type of target are we comparing against. */
UENUM()
enum class ECFCompareAttributeTargetType : uint8
{
	Value,
	Attribute,
	Parameter
};

/**
 * Checks the owner for the given attribute and compares it against another value.
 */
UCLASS()
class CFCOMBAT_API UCFCompareAttributeValue : public UCFConditionBlock
{
	GENERATED_BODY()

	/* The attribute to check on the owner. */
	UPROPERTY(EditDefaultsOnly, Category = "Condition")
	FGameplayTag AttributeTag;
	
	/* Comparison between AttributeTag val and ComparedToVal */
	UPROPERTY(EditDefaultsOnly, Category = "Condition")
	ECFCompareAttributeOperation Comparator;
	
	/* If true, compare against a parameter value. */
	UPROPERTY(EditDefaultsOnly, Category = "Condition")
	ECFCompareAttributeTargetType CompareTargetType;
	
	/* The value to be compared with the AttributeTag value */
	UPROPERTY(EditDefaultsOnly, Category = "Condition", meta=(EditCondition="CompareTargetType == ECFCompareAttributeTargetType::Value", EditConditionHides))
	float CompareValue;
	
	/* The tag for the attribute or parameter to be compared with AttributeTag */
	UPROPERTY(EditDefaultsOnly, Category = "Condition", meta=(EditCondition="CompareTargetType != ECFCompareAttributeTargetType::Value", EditConditionHides))
	FGameplayTag CompareTag;
	
public:
	//~ Begin UCFConditionBlock Interface
	bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock Interface
};
