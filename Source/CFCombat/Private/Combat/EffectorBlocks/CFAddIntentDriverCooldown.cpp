// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFAddIntentDriverCooldown.h"
#include "Combat/CFIntentDriver.h"
#include "Combat/CFSystemParameterCollection.h"

UCFAddIntentDriverCooldown::UCFAddIntentDriverCooldown()
	: bUseCooldownParameter(false)
	, Cooldown(1.0f)
	, bApplyOnEnd(true)
{}

void UCFAddIntentDriverCooldown::OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context)
{
	if (!bApplyOnEnd && ActionIntent && Context.IntentDriver)
	{
		float EffectiveCooldown = Cooldown;
		if (bUseCooldownParameter && Context.Parameters && CooldownParameter.IsValid())
		{
			Context.Parameters->GetFloatParameter(CooldownParameter, EffectiveCooldown);
		}
		
		Context.IntentDriver->SetIntentCooldown(ActionIntent, EffectiveCooldown);
	}
}

void UCFAddIntentDriverCooldown::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	if (bApplyOnEnd && ActionIntent && Context.IntentDriver)
	{
		float EffectiveCooldown = Cooldown;
		if (bUseCooldownParameter && Context.Parameters && CooldownParameter.IsValid())
		{
			Context.Parameters->GetFloatParameter(CooldownParameter, EffectiveCooldown);
		}
		
		Context.IntentDriver->SetIntentCooldown(ActionIntent, EffectiveCooldown);
	}
}
