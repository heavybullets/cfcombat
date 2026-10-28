// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "UObject/WeakObjectPtr.h"
#include "CFAttributeEffectorBase.generated.h"

//fwd
class UCFGameplayAttributesComponent;
class UNiagaraComponent;
class UNiagaraSystem;

/**
 * Base class for attribute effectors on the system.
 */
UCLASS(Blueprintable)
class CFCOMBAT_API UCFAttributeEffectorBase : public UObject
{
	GENERATED_BODY()

protected:
	/* The tag that identifies this effector. */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Effectors")
	FGameplayTag EffectorTagType;

	/* The tags that this effector is triggered on. */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Effectors")
	FGameplayTagContainer AffectedTags;

	/* If true, this effector can be stacked with effectors of the same tag type. */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Effectors")
	bool bAllowTypeStacking;

	/* If true, this effector can be stacked with other instances of their same instance.  */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Effectors")
	bool bAllowInstanceStacking;

	/* If greater than zero, the effector will automatically unregister itself when the elapsed time passes. */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Effectors")
	float InitDuration;

	/* A default particle effect to play during the lifetime of this effect. */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Effectors")
	TObjectPtr<UNiagaraSystem> EffectorParticleEmitter;

	/* Relative offset from the effector owner's transform from where the particle system will be played. */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Effectors")
	FTransform EmitterRelativeTransform;

	/* The current instance of the emitter VFX that is playing. */
	UPROPERTY(Transient)
	TObjectPtr<UNiagaraComponent> ActiveEmitter;

	/* The parent effector that we're bound to. */
	TWeakObjectPtr<UCFGameplayAttributesComponent> GameplayAttributesComponent;

	/* Timer handle used for the auto-unregistration. */
	FTimerHandle DurationTimerHandle;

public:
	//ctor
	UCFAttributeEffectorBase();

	//Getters
	bool AllowsTypeStacking() const { return bAllowTypeStacking; }
	bool AllowsInstanceStacking() const { return bAllowInstanceStacking; }
	FGameplayTag GetEffectorTagType() const { return EffectorTagType; }
	FGameplayTagContainer GetAffectedTags() const { return AffectedTags; }

	/* Obtain the world of this UObject. */
	virtual UWorld* GetWorld() const;

	/* Obtain the modified value of the tag using this effector. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes|Effectors")
	float GetModifiedFloatValue(FGameplayTag AttributeTag, float AttributeCurrentValue) const;

	/* If this effector affects the given attribute tag. */
	UFUNCTION(BlueprintPure, Category = "Attributes|Effectors")
	bool DoesAffectAttributeTag(FGameplayTag AttributeTag) const;

	/* Overrides the duration of this effector and refreshes the timer. */
	UFUNCTION(BlueprintCallable, Category = "Attributes|Effectors")
	void SetDuration(float InDuration);
	
	/* Initializes the given effector inside its parent attribute component. */
	void InitEffector(UCFGameplayAttributesComponent* ParentAttributesComponent);

	/* Removes this effector from its parent attributes component. */
	void RemoveEffector();

	/* Triggers a notification that this effector got added to an attribute component. */
	void NotifyEffectorAdded();

	/* Triggers a notification that this effector got removed from an attribute component. */
	void NotifyEffectorRemoved();

protected:
	/* Called when the attribute gets added to an attribute component. */
	virtual void OnEffectorAddedToComponent() {}

	/* Called when the attribute gets removed from an attribute component. */
	virtual void OnEffectorRemovedFromComponent() {}

	/* Called when an effector is added, used to apply any visual/sound effects to the target it might need. */
	virtual void ApplyFeedbackEffects();

	/* Removes any added effect on the target. */
	virtual void RemoveFeedbackEffects();

	/* Initializes the duration of the attribute. */
	virtual void RefreshDuration(float InDuration);
};
