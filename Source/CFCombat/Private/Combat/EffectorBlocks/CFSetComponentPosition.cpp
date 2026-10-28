// Copyright Critical Failure Studio Ltd.UCFSetComponentPosition

#include "Combat/EffectorBlocks/CFSetComponentPosition.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"

UCFSetComponentPosition::UCFSetComponentPosition()
	: bUseSystemParameterTarget(false)
	, bLocalSpace(false)
{}

void UCFSetComponentPosition::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
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
			FVector LocationTarget = Target;
			if (bUseSystemParameterTarget)
			{
				if (!TargetParameterTag.IsValid() || !Context.Parameters->GetVectorParameter(TargetParameterTag, LocationTarget))
				{
					return;
				}
			}

			//Transform to worldpsace if needed
			if (bLocalSpace)
			{
				LocationTarget = Context.Owner->GetActorTransform().TransformPosition(LocationTarget);
			}

			//Finally set the location for this component
			SceneComponent->SetWorldLocation(LocationTarget);
		}
	}
}

