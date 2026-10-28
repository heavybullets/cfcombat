// Copyright Critical Failure Studio Ltd.

#include "Attributes/Effectors/CFOverrideAttributeEffector.h"

UCFOverrideAttributeEffector::UCFOverrideAttributeEffector()
	: NewValue(0.0f)
{}

void UCFOverrideAttributeEffector::SetValue(float Value)
{
	NewValue = Value;
}

float UCFOverrideAttributeEffector::GetModifiedFloatValue_Implementation(FGameplayTag AttributeTag, float AttributeCurrentValue) const
{
	return NewValue;
}
