// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFChangeAttributeBaseValue.h"
#include "Attributes/ICFGameplayAttributeSource.h"
#include "Attributes/CFGameplayAttributesComponent.h"

void UCFChangeAttributeBaseValue::OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context)
{
	if (!bApplyOnEnd && AttributeTag.IsValid() && Context.Owner->Implements<UCFGameplayAttributeSource>())
	{
		UCFGameplayAttributesComponent* GameplayAttributes = ICFGameplayAttributeSource::Execute_GetCFAttributesComponent(Context.Owner.Get());
		if (GameplayAttributes)
		{
			GameplayAttributes->RegisterFloatAttribute(AttributeTag, NewValue);
		}
	}
}

void UCFChangeAttributeBaseValue::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	if (bApplyOnEnd && AttributeTag.IsValid() && Context.Owner->Implements<UCFGameplayAttributeSource>())
	{
		UCFGameplayAttributesComponent* GameplayAttributes = ICFGameplayAttributeSource::Execute_GetCFAttributesComponent(Context.Owner.Get());
		if (GameplayAttributes)
		{
			GameplayAttributes->RegisterFloatAttribute(AttributeTag, NewValue);
		}
	}
}
