// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFSetComponentPositionBySpline.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"

UCFSetComponentPositionBySpline::UCFSetComponentPositionBySpline()
	: bUseCustomParameterTime(false)
{}

void UCFSetComponentPositionBySpline::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	const bool bValidDuration = bUseCustomParameterTime || GetDuration() > 0.0f;
	if (bValidDuration && Context.Parameters && ComponentParameterTag.IsValid() && SplineComponentParameterTag.IsValid())
	{
		//Obtain the component to move
		USceneComponent* ComponentToMove = Context.Parameters->GetObjectParameter<USceneComponent>(ComponentParameterTag);
		if (!ComponentToMove)
		{
			AActor* ActorFallback = Context.Parameters->GetObjectParameter<AActor>(ComponentParameterTag);
			if (ActorFallback)
			{
				ComponentToMove = ActorFallback->GetRootComponent();
			}
		}

		//Obtain the spline component or actor to use
		USplineComponent* SplineComponent = Context.Parameters->GetObjectParameter<USplineComponent>(SplineComponentParameterTag);
		if (ComponentToMove && SplineComponent)
		{
			float CurrentTime = 0.0f;
			if (bUseCustomParameterTime && Context.Parameters->GetFloatParameter(CustomTimeParameterTag, CurrentTime))
			{
				if (bParameterIsDistance)
				{
					CurrentTime = CurrentTime / SplineComponent->GetSplineLength() * SplineComponent->Duration;
				}
			}
			else if (GetDuration() > 0.0f)
			{
				CurrentTime = (Context.CurrentSectionTime - GetStartTime()) / GetDuration();
			}

			const FVector CurrentLocation = SplineComponent->GetLocationAtTime(CurrentTime, ESplineCoordinateSpace::World);
			ComponentToMove->SetWorldLocation(CurrentLocation + Offset);
		}
	}
}
