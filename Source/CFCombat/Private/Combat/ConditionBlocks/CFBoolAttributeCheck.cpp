// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFBoolAttributeCheck.h"
#include "Attributes/ICFGameplayAttributeSource.h"
#include "Attributes/CFGameplayAttributesComponent.h"

bool UCFBoolAttributeCheck::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	if (AttributeTag.IsValid() && Context.Owner->Implements<UCFGameplayAttributeSource>())
	{
		const UCFGameplayAttributesComponent* GameplayAttributes = ICFGameplayAttributeSource::Execute_GetCFAttributesComponent(Context.Owner.Get());
		if (GameplayAttributes)
		{
			return GameplayAttributes->GetBoolAttribute(AttributeTag);
		}
	}

	return false;
}
