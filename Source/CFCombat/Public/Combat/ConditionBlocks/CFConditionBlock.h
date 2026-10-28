// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Combat/CFActionIntentBuffer.h"
#include "CFConditionBlock.generated.h"

//fwd
class AActor;
class UCFSystemParameterCollection;

/**
 * The context to be passed to the condition block.
 */
USTRUCT(BlueprintType)
struct FCFConditionBlockContext
{
	GENERATED_BODY()

	/* The actor that the combat component is working with. */
	UPROPERTY(BlueprintReadOnly, Category = "Condition")
	TWeakObjectPtr<AActor> Owner;

	/* Intent buffer if it exists. */
	UPROPERTY(BlueprintReadOnly, Category = "Condition")
	FCFActionIntentBuffer IntentBuffer;

	/* The duration of the current active combat section. */
	UPROPERTY(BlueprintReadOnly, Category = "Condition")
	float CurrentSectionTime;

	/* System parameters collections if they exist, can be NULL if not defined. */
	UPROPERTY(BlueprintReadOnly, Category = "Condition")
	UCFSystemParameterCollection* Parameters;

public:
	//ctor
	FCFConditionBlockContext(AActor* InOwner = nullptr, UCFSystemParameterCollection* InParameters = nullptr, float InCurrentSectionTime = 0.0f)
		: Owner(InOwner)
		, CurrentSectionTime(InCurrentSectionTime)
		, Parameters(InParameters)
	{}
};

/**
 * A logic unit that can be added to drive different conditions for either 'entry' or 'exit' points on a combat section.
 * These objects shouldn't hold states.
 */
UCLASS(Blueprintable, EditInlineNew, abstract)
class CFCOMBAT_API UCFConditionBlock : public UObject
{
	GENERATED_BODY()

	/* If true, the result is negated. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bNegateCondition;

	/* Used for grabbing the context of the world when called. */
	mutable TWeakObjectPtr<UWorld> CurrentWorldContext;

public:
	//ctor
	UCFConditionBlock();

	/* Obtains the condition result from this block. */
	bool GetConditionResult(const FCFConditionBlockContext& Context) const;

	/* Used to access world-related functions. */
	virtual UWorld* GetWorld() const override;
	
	/* Initializes the condition before its processed. */
	void InitCondition(const FCFConditionBlockContext& Context);
	
protected:
	/* Initializes the condition before its processed. */
	UFUNCTION(BlueprintNativeEvent, Category = "Condition")
	void OnConditionInit(const FCFConditionBlockContext& Context);
	
	/* Executes the logic block and returns whether the condition passed or not. */
	UFUNCTION(BlueprintNativeEvent, Category = "Condition")
	bool ProcessCondition(const FCFConditionBlockContext& Context) const;
};
