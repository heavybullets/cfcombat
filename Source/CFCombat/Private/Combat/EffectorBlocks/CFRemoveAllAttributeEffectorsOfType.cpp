// Copyright Critical Failure Studio Ltd.


#include "EffectorBlocks/CFRemoveAllAttributeEffectorsOfType.h"

#include "CFGameplayAttributesComponent.h"
#include "ICFGameplayAttributeSource.h"

void UCFRemoveAllAttributeEffectorsOfType::OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context)
{
	Super::OnEffectorStart_Implementation(Context);
	
	if (!bRemoveOnEnd && EffectorType.IsValid() && Context.Owner.Get())
	{
		if (UCFGameplayAttributesComponent* GameplayAttributes = ICFGameplayAttributeSource::Execute_GetCFAttributesComponent(Context.Owner.Get()))
		{
			GameplayAttributes->RemoveEffectorsOfType(EffectorType);
		}
	}
}

void UCFRemoveAllAttributeEffectorsOfType::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	Super::OnEffectorEnd_Implementation(Context);
	
	if (bRemoveOnEnd && EffectorType.IsValid() && Context.Owner.Get())
	{
		if (UCFGameplayAttributesComponent* GameplayAttributes = ICFGameplayAttributeSource::Execute_GetCFAttributesComponent(Context.Owner.Get()))
		{
			GameplayAttributes->RemoveEffectorsOfType(EffectorType);
		}
	}
}
