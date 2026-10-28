// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFLerpVectorByCurve.h"
#include "Components/SceneComponent.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Curves/CurveFloat.h"

UCFLerpVectorByCurve::UCFLerpVectorByCurve()
	: bUseStartParameterTag(false)
	, bUseEndParameterTag(false)
	, bScaleTimeToDuration(false)
{}

void UCFLerpVectorByCurve::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	if (TargetParameterTag.IsValid() && AlphaCurve && (Context.Parameters || (!bUseStartParameterTag && !bUseEndParameterTag)))
	{
		//Obtain start value
		FVector Start = StartValue;
		if (bUseStartParameterTag)
		{
			if (!Context.Parameters->GetVectorParameter(StartParameterTag, Start))
			{
				return;
			}
		}

		//Obtain end value
		FVector End = EndValue;
		if (bUseEndParameterTag)
		{
			if (!Context.Parameters->GetVectorParameter(EndParameterTag, End))
			{
				return;
			}
		}

		//Obtain the alpha value from the curve
		const float DurationScale = bScaleTimeToDuration && GetDuration() > 0.0f ? 1.0f / GetDuration() : 1.0f;
		const float CurrentTimeNormalized = (Context.CurrentSectionTime - GetStartTime()) * DurationScale;
		const float AlphaValue = AlphaCurve->GetFloatValue(CurrentTimeNormalized);

		//Apply the interpolation 
		const FVector InterpolatedValue = FMath::Lerp(Start, End, AlphaValue);
		Context.Parameters->SetVectorParameter(TargetParameterTag, InterpolatedValue);
	}
}
