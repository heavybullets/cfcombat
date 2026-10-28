// Copyright Critical Failure Studio Ltd.

#include "Damage/CFDurabilityComponent.h"
#include "Damage/CFDamageProcessorBase.h"
#include "Attributes/CFGameplayAttributesComponent.h"
#include "Attributes/ICFGameplayAttributeSource.h"
#include "CFCombatGameplayTags.h"

// Sets default values for this component's properties
UCFDurabilityComponent::UCFDurabilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	//Set variables
	MaxDurability = 100.0f;
	CurrentDurability = 0.0f;
	IFramesWindowDuration = 0.5f;
	bEnabled = true;
}


// Called when the game starts
void UCFDurabilityComponent::BeginPlay()
{
	Super::BeginPlay();

	//Bind events
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UCFDurabilityComponent::OnTakeDamage);

	CurrentDurability = MaxDurability;
}

void UCFDurabilityComponent::RestoreMaxDurability()
{
	RefillDurability(MaxDurability);
}

void UCFDurabilityComponent::RefillDurability(float HealedAmount)
{
	const float PrevDurability = CurrentDurability;
	CurrentDurability = FMath::Min(CurrentDurability + HealedAmount, MaxDurability);
	OnDurabilityHealed.Broadcast(CurrentDurability - PrevDurability);
}

void UCFDurabilityComponent::SetMaxDurability(float NewMaxDurability)
{
	MaxDurability = FMath::Max(0.0f, NewMaxDurability);
	CurrentDurability = FMath::Min(CurrentDurability, MaxDurability);
	OnMaxDurabilityChanged.Broadcast();
}

void UCFDurabilityComponent::SetEnableDamageIFrames(bool bEnable)
{
	bEnableDamageIFrames = bEnable;
}

void UCFDurabilityComponent::OverrideCurrentDurability(float InValue)
{
	CurrentDurability = FMath::Min(InValue, MaxDurability);
}

void UCFDurabilityComponent::SetIFramesDurationInSeconds(float Duration)
{
	IFramesWindowDuration = Duration;
}

void UCFDurabilityComponent::InitDurability(float InValue)
{
	MaxDurability = InValue;
	CurrentDurability = InValue;
}

UCFDamageProcessorBase* UCFDurabilityComponent::RegisterProcessorFromClass(TSubclassOf<UCFDamageProcessorBase> ProcessorClass)
{
	if (ProcessorClass)
	{
		UCFDamageProcessorBase* NewProcessor = NewObject<UCFDamageProcessorBase>(GetTransientPackage(), ProcessorClass);
		AddDamageProcessor(NewProcessor);
		return NewProcessor;
	}

	return nullptr;
}

void UCFDurabilityComponent::AddDamageProcessor(UCFDamageProcessorBase* Processor)
{
	RegisteredProcessors.Add(Processor);
}

void UCFDurabilityComponent::RemoveDamageProcessor(UCFDamageProcessorBase* Processor)
{
	RegisteredProcessors.Remove(Processor);
}

float UCFDurabilityComponent::GetDamageFactor() const
{
	float OutVal = 1.0f;
	if (GetOwner()->Implements<UCFGameplayAttributeSource>())
	{
		UCFGameplayAttributesComponent* AttrComponent = ICFGameplayAttributeSource::Execute_GetCFAttributesComponent(GetOwner());
		if (AttrComponent)
		{
			AttrComponent->GetFloatAttribute(CFTAG_Attributes_Durability_DamageScale, OutVal);
		}
	}

	return OutVal;
}

void UCFDurabilityComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamagedActor->CanBeDamaged() && bEnabled && !bIFramesActive)
	{
		//Scale the incoming damage (never a heal) and let the processors modify it, so they see the damage before it is capped
		float AdjustedDamage = Damage > 0.0f ? Damage * GetDamageFactor() : Damage;
		const TArray<UCFDamageProcessorBase*> ProcessorsCopy = RegisteredProcessors;
		for (UCFDamageProcessorBase* Processor : ProcessorsCopy)
		{
			if (Processor->IsEnabled())
			{
				AdjustedDamage = Processor->ProcessDurabilityDamage(DamagedActor, AdjustedDamage, DamageType, InstigatedBy, DamageCauser);
			}			
		}

		//A negative result means the damage turned into a heal
		if (AdjustedDamage < 0.0f)
		{
			RefillDurability(-AdjustedDamage);
			return;
		}

		//Apply only what the remaining durability can take
		const float AppliedDamage = FMath::Min(AdjustedDamage, CurrentDurability);
		if (AppliedDamage > 0.0f)
		{
			CurrentDurability -= AppliedDamage;

			//Trigger correct callbacks
			OnTakingDurabilityDamage.Broadcast(AppliedDamage, DamageCauser);
			if (CurrentDurability > 0.0f)
			{
				ApplyIFrames();
			}
			else
			{
				OnDurabilityDepleted.Broadcast();
			}
		}
	}
}

void UCFDurabilityComponent::ApplyIFrames()
{
	if (bEnableDamageIFrames)
	{
		//We don't need to worry re-triggering an old timer as the 'iframes' make the character invincible either way, so we wouldn't be able to trigger 'iframes' even if we wanted.
		bIFramesActive = true;
		GetWorld()->GetTimerManager().SetTimer(IFramesTimerHandle, this, &UCFDurabilityComponent::OnIFramesWindowExpired, IFramesWindowDuration);
	}
}

void UCFDurabilityComponent::OnIFramesWindowExpired()
{
	bIFramesActive = false;
}

