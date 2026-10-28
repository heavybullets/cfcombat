// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFRotateComponentByRate.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"

UCFRotateComponentByRate::UCFRotateComponentByRate()
	: bUseSystemParameterRotationRate(false)
{}

void UCFRotateComponentByRate::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	if (ComponentParameterTag.IsValid() && Context.Parameters)
	{
		USceneComponent* SceneComponent = Context.Parameters->GetObjectParameter<USceneComponent>(RotationRateParameterTag);
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
			FRotator RotRate = RotationRate;
			if (bUseSystemParameterRotationRate)
			{
				FVector RotRateEuler;
				if (!RotationRateParameterTag.IsValid() || !Context.Parameters->GetVectorParameter(RotationRateParameterTag, RotRateEuler))
				{
					return;
				}

				RotRate = FRotator::MakeFromEuler(RotRateEuler);
			}

			//Obtain the rotator that will make us point towards that target
			SceneComponent->AddWorldRotation(RotRate * DeltaTime);
		}
	}
}
