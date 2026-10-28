// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFHasTimeElapsed.h"
#include "CFSystemParameterCollection.h"

UCFHasTimeElapsed::UCFHasTimeElapsed()
	: Duration(1.0f)
{}

bool UCFHasTimeElapsed::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	if (bUseDurationParameter && DurationParameter.IsValid())
	{
		float DurationFromTag;
		if (Context.Parameters->GetFloatParameter(DurationParameter, DurationFromTag))
		{
			return Context.CurrentSectionTime > DurationFromTag;
		}
		
		return false;
	}
	else
	{
		return Context.CurrentSectionTime > Duration;
	}
}
