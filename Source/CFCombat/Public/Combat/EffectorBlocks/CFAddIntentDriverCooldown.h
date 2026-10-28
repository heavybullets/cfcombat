// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFAddIntentDriverCooldown.generated.h"

//fwd
class UCFActionIntent;

/**
 * Adds a cooldown timer for the given intent.
 * The owner combat component needs an intent driver setup for this to work.
 */
UCLASS()
class CFCOMBAT_API UCFAddIntentDriverCooldown : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* The ActionIntent we want to set the cooldown to. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	TObjectPtr<UCFActionIntent> ActionIntent;
	
	/* Whether to use a constant variable or a parameter for cooldown instead. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bUseCooldownParameter;

	/* Amount of cooldown to apply to the action intent. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (UIMin = "0.0", EditCondition = "!bUseCooldownParameter", EditConditionHides))
	float Cooldown;
	
	/* Float parameter for cooldown to apply to the action intent. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bUseCooldownParameter", EditConditionHides))
	FGameplayTag CooldownParameter;

	/* If true, this only applies when the effector ends, otherwise it applies on effector start. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bApplyOnEnd;

public:
	//ctor
	UCFAddIntentDriverCooldown();

	//~ Begin UCFEffectorBlock interface
	void OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context) override;
	void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End UCFEffectorBlock interface
};
