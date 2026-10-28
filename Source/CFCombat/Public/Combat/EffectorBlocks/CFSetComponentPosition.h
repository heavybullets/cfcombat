// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFSetComponentPosition.generated.h"

/**
 * Moves the component defined by the system parameter tag to the defined location.
 * If an actor is given as a dynamic parameter instead, it uses its root component.
 */
UCLASS()
class CFCOMBAT_API UCFSetComponentPosition : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* The system parameter tag that holds the scene component or actor to move. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag ComponentParameterTag;

	/* If true, we will use another system parameter to read the target. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bUseSystemParameterTarget;

	/* The system parameter tag that holds the scene component to rotate. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bUseSystemParameterTarget"))
	FGameplayTag TargetParameterTag;

	/* The target when not using the target parameter tag. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "!bUseSystemParameterTarget"))
	FVector Target;

	/* If true, the targets are defined in the owner's local space. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bLocalSpace;

public:
	//ctor
	UCFSetComponentPosition();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
