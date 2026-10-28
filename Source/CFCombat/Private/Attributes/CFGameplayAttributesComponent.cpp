// Copyright Critical Failure Studio Ltd.

#include "Attributes/CFGameplayAttributesComponent.h"
#include "Attributes/Effectors/CFAttributeEffectorBase.h"

// Sets default values for this component's properties
UCFGameplayAttributesComponent::UCFGameplayAttributesComponent()
	: bLockAttributeChangeListeners(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UCFGameplayAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	//Initialize the default attribute set
	static const TCHAR* ContextString = TEXT("UCFGameplayAttributesComponent::BeginPlay");
	FCFGameplayAttributeSet* AttributePtr = DefaultAttributeSet.GetRow<FCFGameplayAttributeSet>(ContextString);
	if (AttributePtr)
	{
		AttributeStorage = AttributePtr->GetAttributeMap();
	}

	//Apply the per-instance attributes directly, they override the default set or add new ones, and shouldn't trigger notifications at this stage
	for (const TPair<FGameplayTag, float>& AttributeKV : InstanceAttributes)
	{
		AttributeStorage.Add(AttributeKV.Key, AttributeKV.Value);
	}
}

void UCFGameplayAttributesComponent::RegisterAttributesFromSet(FDataTableRowHandle AttributeSet)
{
	static const TCHAR* ContextString = TEXT("UCFGameplayAttributesComponent::RegisterAttributesFromSet");
	FCFGameplayAttributeSet* AttributePtr = AttributeSet.GetRow<FCFGameplayAttributeSet>(ContextString);
	if (AttributePtr)
	{
		AttributeStorage.Append(AttributePtr->GetAttributeMap());
	}
}

void UCFGameplayAttributesComponent::RemoveAttributesFromSet(FDataTableRowHandle AttributeSet)
{
	static const TCHAR* ContextString = TEXT("UCFGameplayAttributesComponent::RemoveAttributesFromSet");
	FCFGameplayAttributeSet* AttributePtr = AttributeSet.GetRow<FCFGameplayAttributeSet>(ContextString);
	if (AttributePtr)
	{
		for (const TPair<FGameplayTag, float>& AttributeKV : AttributePtr->GetAttributeMap())
		{
			AttributeStorage.Remove(AttributeKV.Key);
		}
	}
}

bool UCFGameplayAttributesComponent::HasAttribute(FGameplayTag AttributeTag) const
{
	return AttributeStorage.Contains(AttributeTag);
}

bool UCFGameplayAttributesComponent::GetFloatAttribute(FGameplayTag AttributeTag, float& OutValue) const
{
	const float* pValue = AttributeStorage.Find(AttributeTag);
	if (!pValue)
	{
		return false;
	}

	//Found the value, process it
	OutValue = *pValue;
	TArray<UCFAttributeEffectorBase*> AttributeEffectors = GetEffectorsForTag(AttributeTag);
	for (UCFAttributeEffectorBase* Effector : AttributeEffectors)
	{
		OutValue = Effector->GetModifiedFloatValue(AttributeTag, OutValue);
	}

	return true;
}

bool UCFGameplayAttributesComponent::GetRawFloatAttribute(FGameplayTag AttributeTag, float& OutValue) const
{
	const float* pValue = AttributeStorage.Find(AttributeTag);
	if (pValue)
	{
		OutValue = *pValue;
	}
	
	return pValue != nullptr;
}

bool UCFGameplayAttributesComponent::GetBoolAttribute(FGameplayTag AttributeTag) const
{
	float Value = 0.0f;
	GetFloatAttribute(AttributeTag, Value);
	return Value != 0;
}

bool UCFGameplayAttributesComponent::GetRawBoolAttribute(FGameplayTag AttributeTag) const
{
	float Value = 0.0f;
	GetRawFloatAttribute(AttributeTag, Value);
	return Value != 0;
}

void UCFGameplayAttributesComponent::RegisterFloatAttribute(FGameplayTag AttributeTag, float Value)
{
	//Store the old value, an attribute that wasn't registered yet starts from zero
	float OldValue = 0.0f;
	GetFloatAttribute(AttributeTag, OldValue);

	//Update storage
	AttributeStorage.Add(AttributeTag, Value);

	//Send notifications with the value as it is read through the effectors
	float NewValue = Value;
	GetFloatAttribute(AttributeTag, NewValue);
	NotifyAttributeChange(AttributeTag, OldValue, NewValue);
}

UCFAttributeEffectorBase* UCFGameplayAttributesComponent::RegisterEffectorFromClass(TSubclassOf<UCFAttributeEffectorBase> EffectorClass)
{
	if (EffectorClass)
	{
		UCFAttributeEffectorBase* NewEffector = NewObject<UCFAttributeEffectorBase>(GetTransientPackage(), EffectorClass);
		AddAttributeEffector(NewEffector);
		return NewEffector;
	}

	return nullptr;
}

void UCFGameplayAttributesComponent::AddAttributeEffector(UCFAttributeEffectorBase* NewEffector)
{
	//Null check first
	if (!NewEffector)
	{
		return;
	}
	
	//Store the old values
	TMap<FGameplayTag, float> OldValues;
	for (const FGameplayTag& Tag : NewEffector->GetAffectedTags())
	{
		float Value;
		if (GetFloatAttribute(Tag, Value))
		{
			OldValues.Add(Tag, Value);
		}
	}

	//Store the old values in case they change
	TMap<FGameplayTag, float> OldAttributeStorage = AttributeStorage;

	//Remove any effector that shouldn't stack with this one
	bLockAttributeChangeListeners = true;
	for (int32 i = RegisteredEffectors.Num() - 1; i >= 0; i--)
	{
		UCFAttributeEffectorBase* Effector = RegisteredEffectors[i];
		const bool bShouldRemoveEffector = (NewEffector->GetEffectorTagType().IsValid() && NewEffector->GetEffectorTagType() == Effector->GetEffectorTagType() && (!NewEffector->AllowsTypeStacking() || !Effector->AllowsTypeStacking()))
										|| (NewEffector->GetClass() == Effector->GetClass() && (!NewEffector->AllowsInstanceStacking() || !Effector->AllowsInstanceStacking()));
		if (bShouldRemoveEffector)
		{
			RemoveAttributeEffector(Effector);
		}
	}
	bLockAttributeChangeListeners = false;

	//Add the effector and initialize
	RegisteredEffectors.Add(NewEffector);
	NewEffector->InitEffector(this);
	NewEffector->NotifyEffectorAdded();
	OnEffectorAdded.Broadcast(NewEffector);

	//Notify the new values
	for (const FGameplayTag& Tag : NewEffector->GetAffectedTags())
	{
		float Value;
		if (OldValues.Contains(Tag) && GetFloatAttribute(Tag, Value))
		{
			NotifyAttributeChange(Tag, OldValues[Tag], Value);
		}
	}
}

void UCFGameplayAttributesComponent::RemoveAttributeEffector(UCFAttributeEffectorBase* Effector)
{
	//Null check first
	if (!Effector)
	{
		return;
	}
	
	//Store the old values
	TMap<FGameplayTag, float> OldValues;
	if (!bLockAttributeChangeListeners)
	{
		for (const FGameplayTag& Tag : Effector->GetAffectedTags())
		{
			float Value;
			if (GetFloatAttribute(Tag, Value))
			{
				OldValues.Add(Tag, Value);
			}
		}
	}

	if (RegisteredEffectors.Remove(Effector) > 0)
	{
		Effector->NotifyEffectorRemoved();
		OnEffectorRemoved.Broadcast(Effector);
	}

	//Notify the changes
	if (!bLockAttributeChangeListeners)
	{
		for (const FGameplayTag& Tag : Effector->GetAffectedTags())
		{
			float Value;
			if (OldValues.Contains(Tag) && GetFloatAttribute(Tag, Value))
			{
				NotifyAttributeChange(Tag, OldValues[Tag], Value);
			}
		}
	}
}

void UCFGameplayAttributesComponent::NotifyAttributeEffectorChange(UCFAttributeEffectorBase* Effector)
{
	if (RegisteredEffectors.Contains(Effector))
	{
		//Notify the new values
		for (const FGameplayTag& Tag : Effector->GetAffectedTags())
		{
			float Value;
			if (GetFloatAttribute(Tag, Value))
			{
				//@TODO: We don't have a way to cache old values, so we cannot obtain the old value for the event, so we will just send the new one twice.
				NotifyAttributeChange(Tag, Value, Value);
			}
		}
	}
}

TArray<UCFAttributeEffectorBase*> UCFGameplayAttributesComponent::GetEffectorsForTag(FGameplayTag AttributeTag) const
{
	return RegisteredEffectors.FilterByPredicate([AttributeTag](const UCFAttributeEffectorBase* Item) { return Item->DoesAffectAttributeTag(AttributeTag); });
}

TArray<UCFAttributeEffectorBase*> UCFGameplayAttributesComponent::GetEffectorsForType(FGameplayTag EffectorTagType) const
{
	return RegisteredEffectors.FilterByPredicate([EffectorTagType](const UCFAttributeEffectorBase* Item) { return Item->GetEffectorTagType() == EffectorTagType; });
}

TArray<UCFAttributeEffectorBase*> UCFGameplayAttributesComponent::GetEffectors() const
{
	return RegisteredEffectors;
}

void UCFGameplayAttributesComponent::RemoveEffectorsOfType(FGameplayTag EffectorTypeTag, int32 RemoveLimit /* = 0 */)
{
	const TArray<UCFAttributeEffectorBase*> Effectors = GetEffectorsForType(EffectorTypeTag);

	//Store the old values
	TMap<FGameplayTag, float> OldValues;
	for (const UCFAttributeEffectorBase* Effector : Effectors)
	{
		for (const FGameplayTag& Tag : Effector->GetAffectedTags())
		{
			float Value;
			if (GetFloatAttribute(Tag, Value))
			{
				OldValues.Add(Tag, Value);
			}
		}
	}

	//Remove the attributes
	bLockAttributeChangeListeners = true;
	int32 NumRemoved = 0;
	for (UCFAttributeEffectorBase* Effector : Effectors)
	{
		RemoveAttributeEffector(Effector);
		NumRemoved++;
		if (RemoveLimit > 0 && NumRemoved >= RemoveLimit)
		{
			break;
		}
	}
	bLockAttributeChangeListeners = false;

	//Fire notifies
	for (const TPair<FGameplayTag, float> ValuePair : OldValues)
	{
		float NewValue;
		if (GetFloatAttribute(ValuePair.Key, NewValue))
		{
			NotifyAttributeChange(ValuePair.Key, ValuePair.Value, NewValue);
		}
		
	}
}

void UCFGameplayAttributesComponent::AddAttributeChangeListener(FGameplayTag AttributeTag, FCFOnAttributeValueChangeSignature Listener)
{
	FCFAttributeListenerArray& Listeners = AttributeChangeListeners.FindOrAdd(AttributeTag);
	Listeners.AddUnique(Listener);
}

void UCFGameplayAttributesComponent::RemoveAttributeChangeListener(FGameplayTag AttributeTag, FCFOnAttributeValueChangeSignature Listener)
{
	FCFAttributeListenerArray* pListeners = AttributeChangeListeners.Find(AttributeTag);
	if (pListeners)
	{
		pListeners->Remove(Listener);
	}
}

void UCFGameplayAttributesComponent::NotifyAttributeChange(FGameplayTag AttributeTag, float OldValue, float NewValue)
{
	//@TODO: Disabled until we can cache old values
	//if (OldValue != NewValue)
	{
		FCFAttributeListenerArray* pListeners = AttributeChangeListeners.Find(AttributeTag);
		if (pListeners)
		{
			for (const FCFOnAttributeValueChangeSignature& Listener : *pListeners)
			{
				Listener.ExecuteIfBound(OldValue, NewValue);
			}
		}
	}
}
