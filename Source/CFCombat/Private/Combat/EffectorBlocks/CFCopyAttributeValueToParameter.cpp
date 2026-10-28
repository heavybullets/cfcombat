// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFCopyAttributeValueToParameter.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Attributes/ICFGameplayAttributeSource.h"
#include "Attributes/CFGameplayAttributesComponent.h"

UCFCopyAttributeValueToParameter::UCFCopyAttributeValueToParameter()
	: bInitializeOnly(false)
{}

void UCFCopyAttributeValueToParameter::OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context)
{
	if (bInitializeOnly)
	{
		if (OutputSystemParameter.IsValid() && InputAttribute.IsValid() && Context.Parameters && Context.Owner->Implements<UCFGameplayAttributeSource>())
		{
			//Attempt to obtain the attribute from the owner
			UCFGameplayAttributesComponent* GameplayAttributes = ICFGameplayAttributeSource::Execute_GetCFAttributesComponent(Context.Owner.Get());
			if (GameplayAttributes)
			{
				//Copy the attribute value onto the output parameter
				float InputValue = 0.0f;
				if (GameplayAttributes->GetFloatAttribute(InputAttribute, InputValue))
				{
					Context.Parameters->SetFloatParameter(OutputSystemParameter, InputValue);
				}
			}
		}

		//Stop effector so it doesn't tick anymore
		StopEffector(Context);
	}
}

void UCFCopyAttributeValueToParameter::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	if (OutputSystemParameter.IsValid() && InputAttribute.IsValid() && Context.Parameters && Context.Owner->Implements<UCFGameplayAttributeSource>())
	{
		//Attempt to obtain the attribute from the owner
		UCFGameplayAttributesComponent* GameplayAttributes = ICFGameplayAttributeSource::Execute_GetCFAttributesComponent(Context.Owner.Get());
		if (GameplayAttributes)
		{
			//Copy the attribute value onto the output parameter
			float InputValue = 0.0f;
			if (GameplayAttributes->GetFloatAttribute(InputAttribute, InputValue))
			{
				Context.Parameters->SetFloatParameter(OutputSystemParameter, InputValue);
			}
		}
	}
}
