// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFCopyAttributeValueToParameter.generated.h"

/**
 * Stores the input attribute from the owner of this combat system onto a system parameter that can be used as input on other effectors.
 * Can be kept updating each tick, or have it only copy the parameter once.
 */
UCLASS()
class CFCOMBAT_API UCFCopyAttributeValueToParameter : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* Input gameplay attribute to read from. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag InputAttribute;

	/* Output system parameter to write to. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag OutputSystemParameter;

	/* If true, the copy is only done once on initialization and then the effector is stopped. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bInitializeOnly;

public:
	//ctor
	UCFCopyAttributeValueToParameter();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
