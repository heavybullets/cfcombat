// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFChangeAttributeBaseValue.generated.h"

/**
 * Modifies the value of the given attribute on the owner.
 * This changes the base value and the change cannot be undone.
 * 
 * This is useful for invalidating 'flag attributes' that we want to set to 'false' or 'true'.
 * If you wish to setup a temporal attribute change that can be undone, don't use this effector.
 */
UCLASS()
class CFCOMBAT_API UCFChangeAttributeBaseValue : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* The attribute to change on the owner. */
	UPROPERTY(EditDefaultsOnly, Category = "Effector")
	FGameplayTag AttributeTag;

	/* The new value to setup. */
	UPROPERTY(EditDefaultsOnly, Category = "Effector")
	float NewValue;

	/* If instead of applying the value right away, we wait for the effector to end. */
	UPROPERTY(EditDefaultsOnly, Category = "Effector")
	bool bApplyOnEnd;

public:
	//~ Begin UCFEffectorBlock Interface
	virtual void OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End UCFEffectorBlock Interface
};
