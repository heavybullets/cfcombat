// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFLogicAnd.h"

void UCFLogicAnd::OnConditionInit_Implementation(const FCFConditionBlockContext& Context)
{
	for (UCFConditionBlock* Condition : Conditions)
	{
		if (Condition)
		{
			Condition->InitCondition(Context);
		}
	}
}

bool UCFLogicAnd::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	for (UCFConditionBlock const* Block : Conditions)
	{
		if (!Block || !Block->GetConditionResult(Context))
		{
			return false;
		}
	}

	return true;
}
