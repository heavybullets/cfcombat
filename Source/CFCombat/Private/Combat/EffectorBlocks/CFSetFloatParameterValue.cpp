// Copyright Critical Failure Studio Ltd.
#include "Combat/EffectorBlocks/CFSetFloatParameterValue.h"
#include "Combat/CFSystemParameterCollection.h"

UCFSetFloatParameterValue::UCFSetFloatParameterValue()
	: Value(0.0f)
	, bApplyOnEnd(false)
{}

void UCFSetFloatParameterValue::OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context)
{
	if (!bApplyOnEnd && Parameter.IsValid() && Context.Parameters)
	{
		if (bRandomInRange)
		{
			Value = FMath::RandRange(RangeMin, RangeMax);
		}
		Context.Parameters->SetFloatParameter(Parameter, Value);
	}
}

void UCFSetFloatParameterValue::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	if (bApplyOnEnd && Parameter.IsValid() && Context.Parameters)
	{
		if (bRandomInRange)
		{
			Value = FMath::RandRange(RangeMin, RangeMax);
		}
		Context.Parameters->SetFloatParameter(Parameter, Value);
	}
}
