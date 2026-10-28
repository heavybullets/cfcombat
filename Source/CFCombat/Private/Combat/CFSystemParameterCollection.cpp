// Copyright Critical Failure Studio Ltd.

#include "Combat/CFSystemParameterCollection.h"

UCFSystemParameterCollection::UCFSystemParameterCollection()
{}

bool UCFSystemParameterCollection::GetFloatParameter(FGameplayTag ParameterTag, float& OutValue) const
{
	const float* pValue = FloatParameters.Find(ParameterTag);
	if (pValue)
	{
		OutValue = *pValue;
		return true;
	}

	return false;
}

bool UCFSystemParameterCollection::GetVectorParameter(FGameplayTag ParameterTag, FVector& OutValue) const
{
	const FVector* pValue = VectorParameters.Find(ParameterTag);
	if (pValue)
	{
		OutValue = *pValue;
		return true;
	}

	return false;
}

UObject* UCFSystemParameterCollection::GetObjectParameter(FGameplayTag ParameterTag) const
{
	UObject* const* pValue = ObjectParameters.Find(ParameterTag);
	return pValue ? *pValue : nullptr;
}

void UCFSystemParameterCollection::SetFloatParameter(FGameplayTag ParameterTag, float NewValue)
{
	if (FloatParameters.Contains(ParameterTag))
	{
		FloatParameters.Add(ParameterTag, NewValue);
	}
}

void UCFSystemParameterCollection::SetVectorParameter(FGameplayTag ParameterTag, const FVector& NewValue)
{
	if (VectorParameters.Contains(ParameterTag))
	{
		VectorParameters.Add(ParameterTag, NewValue);
	}
}

void UCFSystemParameterCollection::SetObjectParameter(FGameplayTag ParameterTag, UObject* NewValue)
{
	if (ObjectParameters.Contains(ParameterTag))
	{
		ObjectParameters.Add(ParameterTag, NewValue);
	}
}
