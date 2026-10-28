// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFApplyAttributeEffector.h"
#include "Attributes/Effectors/CFAttributeEffectorBase.h"
#include "Attributes/ICFGameplayAttributeSource.h"
#include "Attributes/CFGameplayAttributesComponent.h"

UCFApplyAttributeEffector::UCFApplyAttributeEffector()
	: bClearOnEnd(true)
{}

void UCFApplyAttributeEffector::OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context)
{
	if (EffectorClass && Context.Owner->Implements<UCFGameplayAttributeSource>())
	{
		UCFGameplayAttributesComponent* GameplayAttributes = ICFGameplayAttributeSource::Execute_GetCFAttributesComponent(Context.Owner.Get());
		if (GameplayAttributes)
		{
			UCFApplyAttributeEffector* MutableThis = const_cast<UCFApplyAttributeEffector*>(this);
			UCFAttributeEffectorBase* NewEffector = NewObject<UCFAttributeEffectorBase>(MutableThis, EffectorClass, NAME_None, RF_Transient);
			GameplayAttributes->AddAttributeEffector(NewEffector);
		}
	}
}

void UCFApplyAttributeEffector::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	if (bClearOnEnd && EffectorClass && Context.Owner->Implements<UCFGameplayAttributeSource>())
	{
		UCFGameplayAttributesComponent* GameplayAttributes = ICFGameplayAttributeSource::Execute_GetCFAttributesComponent(Context.Owner.Get());
		if (GameplayAttributes)
		{
			UCFAttributeEffectorBase* FoundEffector = nullptr;
			for (UCFAttributeEffectorBase* Effector : GameplayAttributes->GetEffectors())
			{
				if (Effector->GetOuter() == this)
				{
					FoundEffector = Effector;
					break;
				}
			}

			//Remove if found
			if (FoundEffector)
			{
				GameplayAttributes->RemoveAttributeEffector(FoundEffector);
			}
		}
	}
}
