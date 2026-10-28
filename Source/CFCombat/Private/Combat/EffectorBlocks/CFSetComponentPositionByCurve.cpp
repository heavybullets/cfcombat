// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFSetComponentPositionByCurve.h"
#include "Components/SceneComponent.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Actor.h"

UCFSetComponentPositionByCurve::UCFSetComponentPositionByCurve()
	: MovementScale(1.0f)
	, bScaleTimeToDuration(false)
	, bResetOnEnd(false)
{}

void UCFSetComponentPositionByCurve::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	if (SystemParameterTag.IsValid() && MovementCurve && Context.Parameters)
	{
		USceneComponent* ComponentToMove = Context.Parameters->GetObjectParameter<USceneComponent>(SystemParameterTag);
		if (!ComponentToMove)
		{
			AActor* ActorFallback = Context.Parameters->GetObjectParameter<AActor>(SystemParameterTag);
			if (ActorFallback)
			{
				ComponentToMove = ActorFallback->GetRootComponent();
			}
		}

		if (ComponentToMove)
		{
			const float DurationScale = bScaleTimeToDuration && GetDuration() > 0.0f ? 1.0f / GetDuration() : 1.0f;
			const float CurrentTimeNormalized = (Context.CurrentSectionTime - GetStartTime()) * DurationScale;
			const FVector CurrentOffset = MovementCurve->GetVectorValue(CurrentTimeNormalized) * MovementScale;
			ComponentToMove->SetRelativeLocation(CurrentOffset);
		}
	}
}

void UCFSetComponentPositionByCurve::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	if (bResetOnEnd && SystemParameterTag.IsValid() && MovementCurve && Context.Parameters)
	{
		USceneComponent* ComponentToMove = Context.Parameters->GetObjectParameter<USceneComponent>(SystemParameterTag);
		if (ComponentToMove)
		{
			ComponentToMove->SetRelativeLocation(ResetValue);
		}
	}
}

