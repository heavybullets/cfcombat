// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFConditionBlock.h"

UCFConditionBlock::UCFConditionBlock()
	: bNegateCondition(false)
{}

bool UCFConditionBlock::GetConditionResult(const FCFConditionBlockContext& Context) const
{
	const bool bProcessedResult = ProcessCondition(Context);
	return bNegateCondition ? !bProcessedResult : bProcessedResult;
}

void UCFConditionBlock::InitCondition(const FCFConditionBlockContext& Context)
{
	CurrentWorldContext = Context.Owner->GetWorld();
	OnConditionInit(Context);
}

void UCFConditionBlock::OnConditionInit_Implementation(const FCFConditionBlockContext& Context)
{
	//To be implemented by child classes
}

bool UCFConditionBlock::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	return false;
}

UWorld* UCFConditionBlock::GetWorld() const
{
	return CurrentWorldContext.Get();
}
