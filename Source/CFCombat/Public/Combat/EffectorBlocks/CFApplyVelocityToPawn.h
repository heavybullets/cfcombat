// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFApplyVelocityToPawn.generated.h"

/**
 * Applies the given velocity to a pawn actor, respecting their movement component if they have one.
 */
UCLASS()
class CFCOMBAT_API UCFApplyVelocityToPawn : public UCFEffectorBlock
{
	GENERATED_BODY()

	/* If true, we will use another system parameter to read the velocity to apply. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bUseSystemParameterVelocity;
	
	/* A value to scale the curve as a whole. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "!bUseSystemParameterVelocity"))
	FVector Velocity;

	/* The system parameter tag that holds the velocity to apply. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bUseSystemParameterVelocity"))
	FGameplayTag VelocityParameterTag;

	/* Scale to apply to the velocity before applying. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag ScaleParameterTag;

	/* Whether the movement defined by the curve is on local space or world space. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bLocalSpace;

	/* If true, the velocity is treated as a single impulse. The velocity is applied only once and the effector is forcefully stopped. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bTreatAsImpulse;

	/* If true, forces falling movement mode for the pawn. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bForceFalling;

	/* Only apply to horizontal plane, ignoring vertical movement. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bIgnoreZAxis;

public:
	//ctor
	UCFApplyVelocityToPawn();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
