// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFApplyMovementToPawn.generated.h"

//fwd
class UCurveVector;

/**
 * Applies the given curve movement to the given pawn, respecting Character's movement if there's one.
 * Uses the Pawn/Character existing movement component to drive the movement itself.
 */
UCLASS()
class CFCOMBAT_API UCFApplyMovementCurveToPawn : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* Contains the data for the movement curve. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	TObjectPtr<UCurveVector> MovementCurve;

	/* A constant value to scale the curve as a whole. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	float MovementScale;

	/* If present a system parameter to scale the movement with. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag ScaleParameter;

	/* Whether the movement defined by the curve is on local space or world space. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bLocalSpace;

	/* Whether we should scale the movement curve to match the duration of this effector. Assumes a normalized curve from [0,1]. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bScaleTimeToDuration;

	/* If the movement should ignore or not the Z axis, so that way gravity still affects the character. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bIgnoreZAxis;

	/* WWhen finishing execution, whether to reset the velocity to zero or not. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bResetVelocityOnEnd;

public:
	//ctor
	UCFApplyMovementCurveToPawn();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
