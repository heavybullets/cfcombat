// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFCompareFloatParameterValue.h"
#include "Combat/CFSystemParameterCollection.h"

UCFCompareFloatParameterValue::UCFCompareFloatParameterValue()
	: bCompareWithSystemParameter(false)
	, Operation(ECFCompareParameterOperation::Equal)
{}

bool UCFCompareFloatParameterValue::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	if (Context.Parameters && InputParameter.IsValid() && (!bCompareWithSystemParameter || CompareParameter.IsValid()))
	{
		float LeftOperand;
		if (Context.Parameters->GetFloatParameter(InputParameter, LeftOperand))
		{
			float RightOperand = CompareValue;
			if (!bCompareWithSystemParameter || Context.Parameters->GetFloatParameter(CompareParameter, RightOperand))
			{
				//Then compare
				switch (Operation)
				{
					case ECFCompareParameterOperation::Equal:
						return LeftOperand == RightOperand;
					case ECFCompareParameterOperation::GreaterThan:
						return LeftOperand > RightOperand;
					case ECFCompareParameterOperation::GreaterEqualThan:
						return LeftOperand >= RightOperand;
					case ECFCompareParameterOperation::LessThan:
						return LeftOperand < RightOperand;
					case ECFCompareParameterOperation::LessEqualThan:
						return LeftOperand <= RightOperand;
				}
			}
		}
	}

	return false;
}
