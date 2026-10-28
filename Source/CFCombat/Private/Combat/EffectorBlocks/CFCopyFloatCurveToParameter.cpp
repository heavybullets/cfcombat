// Copyright Critical Failure Studio Ltd.
#include "Combat/EffectorBlocks/CFCopyFloatCurveToParameter.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Curves/CurveFloat.h"

UCFCopyFloatCurveToParameter::UCFCopyFloatCurveToParameter()
	: Scale(1.0f)
	, bScaleTimeToDuration(false)
{}

void UCFCopyFloatCurveToParameter::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	if (InputCurve && OutputParameter.IsValid() && Context.Parameters)
	{
		float EffectiveScale = Scale;
		if (ScaleParameter.IsValid() && Context.Parameters)
		{
			float ParameterValue;
			if (Context.Parameters->GetFloatParameter(ScaleParameter, ParameterValue))
			{
				EffectiveScale *= ParameterValue;
			}
		}

		const float DurationScale = bScaleTimeToDuration && GetDuration() > 0.0f ? 1.0f / GetDuration() : 1.0f;
		const float CurrentTimeNormalized = (Context.CurrentSectionTime - GetStartTime()) * DurationScale;
		const float PrevTimeNormalized = (Context.CurrentSectionTime - GetStartTime() - DeltaTime) * DurationScale;
		const float CurrentValue = InputCurve->GetFloatValue(CurrentTimeNormalized) * EffectiveScale;

		Context.Parameters->SetFloatParameter(OutputParameter, CurrentValue);
	}
}
