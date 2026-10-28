// Copyright Critical Failure Studio Ltd.

#include "Combat/CFIntentDriver.h"
#include "Combat/CFCombatComponent.h"
#include "Combat/CFActionIntent.h"

UCFIntentDriver::UCFIntentDriver()
{}

void UCFIntentDriver::Init(UCFCombatComponent* Owner)
{
	check(Owner);
	OwnerCombatComponent = Owner;

	//Call BP implementation
	OnInit(Owner);
}

void UCFIntentDriver::BeginDestroy()
{
	Super::BeginDestroy();

	UWorld* World = GetWorld();
	if (World)
	{
		for (auto& Entry : ActiveCooldownHandles)
		{
			GetWorld()->GetTimerManager().ClearTimer(Entry.Value);
		}
		ActiveCooldownHandles.Empty();
	}
}

void UCFIntentDriver::OnInit_Implementation(UCFCombatComponent* Owner)
{
	//Empty for child classes
}

void UCFIntentDriver::ProcessIntentBufferUpdate(const FCFIntentDriverUpdateContext& UpdateContext)
{
	//Request the child classes for any custom buffer update they might have
	TArray<UCFActionIntent*> CustomIntents;
	OnRequestIntentBufferUpdate(UpdateContext, CustomIntents);
	for (UCFActionIntent* Intent : CustomIntents)
	{
		if (!ActiveCooldownHandles.Contains(Intent))
		{
			OwnerCombatComponent->RegisterActionIntent(Intent);
		}
	}

	//Then trigger any queued intent that might have been setup on the buffer update
	for (UCFActionIntent* Intent : QueuedIntents)
	{
		if (!ActiveCooldownHandles.Contains(Intent))
		{
			OwnerCombatComponent->RegisterActionIntent(Intent);
		}
	}

	//Clear the queued intents, we already triggered them
	QueuedIntents.Empty();
}

void UCFIntentDriver::OnRequestIntentBufferUpdate_Implementation(const FCFIntentDriverUpdateContext& UpdateContext, TArray<UCFActionIntent*>& OutIntents)
{
	//Empty for child classes
}

void UCFIntentDriver::EnqueueIntentRegistration(UCFActionIntent* Intent)
{
	QueuedIntents.Add(Intent);
}

void UCFIntentDriver::SetIntentCooldown(UCFActionIntent* Intent, float CooldownTime)
{
	UWorld* World = GetWorld();
	if (Intent && World)
	{
		DeactivateIntentCooldown(Intent);
		if (CooldownTime > 0.0f)
		{
			FTimerHandle Handle;
			FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UCFIntentDriver::DeactivateIntentCooldown, Intent);
			World->GetTimerManager().SetTimer(Handle, Delegate,  CooldownTime, false);
			ActiveCooldownHandles.Add(Intent, Handle);
		}
	}
}

AActor* UCFIntentDriver::GetOwner() const
{
	return OwnerCombatComponent.IsValid() ? OwnerCombatComponent->GetOwner() : nullptr;
}

UWorld* UCFIntentDriver::GetWorld() const
{
	return OwnerCombatComponent.IsValid() ? OwnerCombatComponent->GetWorld() : nullptr;
}

void UCFIntentDriver::DeactivateIntentCooldown(UCFActionIntent* Intent)
{
	FTimerHandle* pHandle = ActiveCooldownHandles.Find(Intent);
	if (pHandle)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(*pHandle);
		}

		//Remove from here, this will make the cooldown not active anymore
		ActiveCooldownHandles.Remove(Intent);
	}
}
