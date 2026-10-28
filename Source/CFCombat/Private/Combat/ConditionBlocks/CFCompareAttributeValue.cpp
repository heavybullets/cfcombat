// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFCompareAttributeValue.h"
#include "CFSystemParameterCollection.h"
#include "Attributes/ICFGameplayAttributeSource.h"
#include "Attributes/CFGameplayAttributesComponent.h"

bool UCFCompareAttributeValue::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	if (AttributeTag.IsValid() && Context.Owner.Get() && Context.Owner->Implements<UCFGameplayAttributeSource>())
	{
		if (const UCFGameplayAttributesComponent* GameplayAttributes = ICFGameplayAttributeSource::Execute_GetCFAttributesComponent(Context.Owner.Get()))
		{			
			float CheckValue;
			if (!GameplayAttributes->GetFloatAttribute(AttributeTag, CheckValue))
			{
				//No float found
				return false;
			}
			
			float ComparedTo = 0.f;
			if (CompareTargetType == ECFCompareAttributeTargetType::Value)
			{
				ComparedTo = CompareValue;
			}
			else if (CompareTargetType == ECFCompareAttributeTargetType::Attribute)
			{
				if (!GameplayAttributes->GetFloatAttribute(CompareTag, ComparedTo))
				{
					//Target attribute not found
					return false;	
				}
			}
			else
			{
				if (!Context.Parameters || !Context.Parameters->GetFloatParameter(CompareTag, ComparedTo))
				{
					//Parameter not found
					return false;
				}
			}

			//Apply comparisons
			switch (Comparator) 
			{
				case ECFCompareAttributeOperation::Equal:
					return CheckValue == ComparedTo;
				case ECFCompareAttributeOperation::NotEqual:
					return CheckValue != ComparedTo;
				case ECFCompareAttributeOperation::Less:
					return CheckValue < ComparedTo;
				case ECFCompareAttributeOperation::LessOrEqual:
					return CheckValue <= ComparedTo;
				case ECFCompareAttributeOperation::Greater:
					return CheckValue > ComparedTo;
				case ECFCompareAttributeOperation::GreaterOrEqual:
					return CheckValue >= ComparedTo;
			}
		}
	}

	return false;
}
