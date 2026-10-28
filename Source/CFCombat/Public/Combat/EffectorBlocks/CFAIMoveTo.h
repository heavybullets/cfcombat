// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFAIMoveTo.generated.h"

//fwd
class UNavigationQueryFilter;

/**
 * Move the owner AI to the given location or actor.
 * This uses the default AIMoveTo implementation and requires a navmesh to work.
 */
UCLASS()
class CFCOMBAT_API UCFAIMoveTo : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* The system parameter for the target actor of the movement. If not passed, the system will fallback to the location instead. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag TargetActorParameterTag;

	/* The system parameter for the target location of the movement. Will be used if no actor target is given. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag TargetLocationParameterTag;

	/* How close the owner AI needs to get to the target to complete the movement request. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	float AcceptanceRadius;

	/* Whether we allow for paths that can't completely reach the target to be used. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bAllowPartialPaths;

	/* Whether the pathfinding size includes the agent radius. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bStopOnOverlap;

	/* Should we keep focal point at move goal. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bCanStrafe;

	/* Optional navigation filter to be used on the request. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	TSubclassOf<UNavigationQueryFilter> FilterClass;

public:
	//ctor
	UCFAIMoveTo();

	//~ Begin Interface UCFEffectorBlock
	virtual void OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End Interface UCFEffectorBlock
};
