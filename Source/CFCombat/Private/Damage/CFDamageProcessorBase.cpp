// Copyright Critical Failure Studio Ltd.

#include "Damage/CFDamageProcessorBase.h"

UCFDamageProcessorBase::UCFDamageProcessorBase()
	: bEnabled(true)
{}

void UCFDamageProcessorBase::SetEnabled(bool bNewEnabled)
{
	bEnabled = bNewEnabled;
}

float UCFDamageProcessorBase::ProcessDurabilityDamage_Implementation(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	return Damage;
}

