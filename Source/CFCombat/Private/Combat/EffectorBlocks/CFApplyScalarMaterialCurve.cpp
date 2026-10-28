// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFApplyScalarMaterialCurve.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Curves/CurveFloat.h"
#include "Materials/MaterialInstanceDynamic.h"

UCFApplyScalarMaterialCurve::UCFApplyScalarMaterialCurve()
	: ParameterScale(1.0f)
	, bScaleTimeToDuration(false)
	, bResetOnEnd(false)
	, ResetValue(0.0f)
{}

void UCFApplyScalarMaterialCurve::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	if (ParameterCurve && SystemParameterTag.IsValid() && Context.Parameters)
	{
		UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(Context.Parameters->GetObjectParameter(SystemParameterTag));
		if (MID)
		{
			const float DurationScale = bScaleTimeToDuration && GetDuration() > 0.0f ? 1.0f / GetDuration() : 1.0f;
			const float CurrentTimeNormalized = (Context.CurrentSectionTime - GetStartTime()) * DurationScale;
			const float CurveValue = ParameterCurve->GetFloatValue(CurrentTimeNormalized);

			MID->SetScalarParameterValue(ParameterName, CurveValue * ParameterScale);
		}
	}
}

void UCFApplyScalarMaterialCurve::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	if (bResetOnEnd && ParameterCurve && SystemParameterTag.IsValid() && Context.Parameters)
	{
		UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(Context.Parameters->GetObjectParameter(SystemParameterTag));
		if (MID)
		{
			MID->SetScalarParameterValue(ParameterName, ResetValue);
		}
	}
}
