// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFSetComponentPositionBySpline.generated.h"


/**
 * Updates the position of a given component to follow the path of a spline.
 * If an actor is given as a dynamic parameter instead, it uses its root component.
 * 
 * The duration of the effector is used to drive the movement, so an indefinite effector is not allowed in this case.
 */
UCLASS()
class CFCOMBAT_API UCFSetComponentPositionBySpline : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* The system parameter tag that holds the scene component or actor to move. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag ComponentParameterTag;

	/* The system parameter tag that holds the spline component that we wish to use. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag SplineComponentParameterTag;

	/* If true, the effector uses a custom parameter for driving the position on the spline instead of the duration. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bUseCustomParameterTime;

	/* System parameter that holds the 'custom time' to use on the spline, instead of being driven by effector duration. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bUseCustomParameterTime"))
	FGameplayTag CustomTimeParameterTag;

	/* The system parameter tag that holds the spline component that we wish to use. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bUseCustomParameterTime"))
	bool bParameterIsDistance;

	/* Value in world coordinates to offset the position of the component after applying the spline movement. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FVector Offset;

public:
	//ctor
	UCFSetComponentPositionBySpline();

	//~ Begin UCFEffectorBlock Interface
	void OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context) override;
	//~ End UCFEffectorBlock Interface
};
