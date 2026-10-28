// Copyright Critical Failure Studio Ltd.

#include "Attributes/Effectors/CFAttributeEffectorBase.h"
#include "Attributes/CFGameplayAttributesComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"

UCFAttributeEffectorBase::UCFAttributeEffectorBase()
{
	bAllowTypeStacking = false;
	bAllowInstanceStacking = false;
	InitDuration = -1.0f;
}

UWorld* UCFAttributeEffectorBase::GetWorld() const
{
	return GameplayAttributesComponent.IsValid() ? GameplayAttributesComponent->GetWorld() : nullptr;
}

float UCFAttributeEffectorBase::GetModifiedFloatValue_Implementation(FGameplayTag AttributeTag, float AttributeCurrentValue) const
{
	//Base effector returns the same value
	return AttributeCurrentValue;
}

bool UCFAttributeEffectorBase::DoesAffectAttributeTag(FGameplayTag AttributeTag) const
{
	return AffectedTags.HasTagExact(AttributeTag);
}

void UCFAttributeEffectorBase::SetDuration(float InDuration)
{
	RefreshDuration(InDuration);
}

void UCFAttributeEffectorBase::InitEffector(UCFGameplayAttributesComponent* ParentAttributesComponent)
{
	GameplayAttributesComponent = ParentAttributesComponent;
	RefreshDuration(InitDuration);
}

void UCFAttributeEffectorBase::RemoveEffector()
{
	if (GameplayAttributesComponent.IsValid())
	{
		GameplayAttributesComponent->RemoveAttributeEffector(this);
	}
}

void UCFAttributeEffectorBase::NotifyEffectorAdded()
{
	ApplyFeedbackEffects();
	OnEffectorAddedToComponent();
}

void UCFAttributeEffectorBase::NotifyEffectorRemoved()
{
	RemoveFeedbackEffects();
	OnEffectorRemovedFromComponent();
}

void UCFAttributeEffectorBase::ApplyFeedbackEffects()
{
	if (EffectorParticleEmitter && GameplayAttributesComponent.IsValid() && GameplayAttributesComponent->GetOwner())
	{
		AActor* Parent = GameplayAttributesComponent->GetOwner();
		ActiveEmitter = UNiagaraFunctionLibrary::SpawnSystemAttached(EffectorParticleEmitter, Parent->GetRootComponent(), NAME_None, EmitterRelativeTransform.GetTranslation(), EmitterRelativeTransform.GetRotation().Rotator(), EmitterRelativeTransform.GetScale3D(), EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);
	}
}

void UCFAttributeEffectorBase::RemoveFeedbackEffects()
{
	if (ActiveEmitter)
	{
		ActiveEmitter->DestroyComponent();
	}
}

void UCFAttributeEffectorBase::RefreshDuration(float InDuration)
{
	UWorld* World = GetWorld();
	if (World)
	{
		//Disable the old timer
		if (DurationTimerHandle.IsValid())
		{
			World->GetTimerManager().ClearTimer(DurationTimerHandle);
		}

		//Enable the new timer
		if (InDuration > 0.0f)
		{
			World->GetTimerManager().SetTimer(DurationTimerHandle, this, &UCFAttributeEffectorBase::RemoveEffector, InDuration);
		}
	}
}
