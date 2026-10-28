// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFRotateComponentToTarget.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"

UCFRotateComponentToTarget::UCFRotateComponentToTarget()
	: bUseSystemParameterTarget(false)
	, bLocalSpace(false)
	, bTargetIsDirection(false)
	, bResetOnEnd(false)
{}

void UCFRotateComponentToTarget::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	if (ComponentParameterTag.IsValid() && Context.Parameters)
	{
		USceneComponent* SceneComponent = Context.Parameters->GetObjectParameter<USceneComponent>(ComponentParameterTag);
		if (!SceneComponent)
		{
			AActor* ActorFallback = Context.Parameters->GetObjectParameter<AActor>(ComponentParameterTag);
			if (ActorFallback)
			{
				SceneComponent = ActorFallback->GetRootComponent();
			}
		}

		if (SceneComponent)
		{
			FVector RotateTarget = Target;
			if (bUseSystemParameterTarget)
			{
				if (!TargetParameterTag.IsValid() || !Context.Parameters->GetVectorParameter(TargetParameterTag, RotateTarget))
				{
					return;
				}
			}

			//Transform to worldpsace if needed
			if (bLocalSpace)
			{
				RotateTarget = SceneComponent->GetOwner()->GetActorTransform().TransformPosition(RotateTarget);
			}

			//If we have a 'world direction' setup as a parameter, then we can obtain the rotator directly, otherwise we need to obtain the direction ourselves
			FVector WorldDirection;
			if (bTargetIsDirection && !bLocalSpace)
			{
				WorldDirection = RotateTarget;
			}
			else
			{
				const FVector ActorLocation = SceneComponent->GetOwner()->GetActorLocation();
				WorldDirection = RotateTarget - ActorLocation;
			}

			//Apply the rotation
			FRotator Rot = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, WorldDirection);
			SceneComponent->SetWorldRotation(Rot);
		}
	}
}

void UCFRotateComponentToTarget::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	if (bResetOnEnd && ComponentParameterTag.IsValid() && Context.Parameters)
	{
		USceneComponent* SceneComponent = Context.Parameters->GetObjectParameter<USceneComponent>(ComponentParameterTag);
		if (SceneComponent)
		{
			SceneComponent->SetRelativeRotation(ResetLocalValue);
		}
	}
}
