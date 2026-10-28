// Copyright Critical Failure Studio Ltd.

#include "Attributes/Effectors/CFAdditiveAttributeEffector.h"

UCFAdditiveAttributeEffector::UCFAdditiveAttributeEffector()
	: Value(0.0f)
{}

void UCFAdditiveAttributeEffector::SetValue(float InValue)
{
	Value = InValue;
}

float UCFAdditiveAttributeEffector::GetModifiedFloatValue_Implementation(FGameplayTag AttributeTag, float AttributeCurrentValue) const
{
	return Value + AttributeCurrentValue;
}