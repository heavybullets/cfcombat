// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFLogicOr.h"

void UCFLogicOr::OnConditionInit_Implementation(const FCFConditionBlockContext& Context)
{
	for (UCFConditionBlock* Condition : Conditions)
	{
		if (Condition)
		{
			Condition->InitCondition(Context);
		}
	}
}

bool UCFLogicOr::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	for (UCFConditionBlock const* Block : Conditions)
	{
		if (Block && Block->GetConditionResult(Context))
		{
			return true;
		}
	}
	
	return false;
}
