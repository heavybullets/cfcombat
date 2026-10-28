// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFConditionalEffector.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"

UCFConditionalEffector::UCFConditionalEffector()
 : bContinuousTesting(true)
{}

void UCFConditionalEffector::OnEffectorInit_Implementation(const FCFEffectorBlockContext& Context)
{
	//Trigger init to the sub effectors
	for (UCFEffectorBlock* Effector : SubEffectors)
	{
		if (Effector)
		{
			Effector->InitEffector(Context);
		}
	}
	
	//Trigger init for the sub conditions
	const FCFConditionBlockContext ConditionContext(Context.Owner.Get(), Context.Parameters, Context.CurrentSectionTime);
	for (UCFConditionBlock* Condition : SubConditions)
	{
		if (Condition)
		{
			Condition->InitCondition(ConditionContext);
		}
	}

	bHasBeenActivated = false;
}

void UCFConditionalEffector::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{	
	//Create a condition context for use 
	FCFConditionBlockContext ConditionContext;
	ConditionContext.Owner = Context.Owner;
	ConditionContext.Parameters = Context.Parameters;
	ConditionContext.CurrentSectionTime = Context.CurrentSectionTime;
	
	//Check whether the conditions allow for us to tick
	if (bContinuousTesting || !bHasBeenActivated)
	{
		for (UCFConditionBlock const* Condition : SubConditions)
		{
			if (Condition && !Condition->GetConditionResult(ConditionContext))
			{
				return;
			}
		}

		bHasBeenActivated = true;
	}

	//If we're up to here, we can tick
	for (UCFEffectorBlock* Effector : SubEffectors)
	{
		if (Effector)
		{
			Effector->TickEffector(DeltaTime, Context);
		}
	}
}

void UCFConditionalEffector::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	//Trigger end on every sub effector
	for (UCFEffectorBlock* Effector : SubEffectors)
	{
		if (Effector)
		{
			Effector->StopEffector(Context);
		}
	}
}
