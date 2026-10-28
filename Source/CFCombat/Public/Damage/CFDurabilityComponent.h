// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CFDurabilityComponent.generated.h"

//fwd
class AController;
class UDamageType;
class UCFDamageProcessorBase;

//delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCFOnTakingDurabilityDamageSignature, float, AmountDamaged, AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCFOnDurabilityHealedSignature, float, AmountHealed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCFOnDurabilitySimpleDelegateSignature);

/**
 * A component that allows for the owning actor to have 'durability' and be damaged by other actors.
 * Manages IFrames and callback events.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CFCOMBAT_API UCFDurabilityComponent : public UActorComponent
{
	GENERATED_BODY()

	/* The maximum amount of durability the owner can have. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Durability", meta = (AllowPrivateAccess = "true"))
	float MaxDurability;

	/* The current amount of durability. */
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "Durability", meta = (AllowPrivateAccess = "true"))
	float CurrentDurability;

	/* If true, each attack will enable a 'iframe' window and make the owner invulnerable for a period of time. */
	UPROPERTY(EditAnywhere, Category = "Durability")
	bool bEnableDamageIFrames;

	/* Duration of the 'iframes' window. */
	UPROPERTY(EditAnywhere, Category = "Durability")
	float IFramesWindowDuration;

	/* If true, we're currently on the 'iframes' period after getting attacked, disabling damage. */
	bool bIFramesActive;

	/* Timer handle delegate for the 'iframes'. */
	FTimerHandle IFramesTimerHandle;

	/* Damage processors currently registered. */
	UPROPERTY(Transient)
	TArray<UCFDamageProcessorBase*> RegisteredProcessors;

public:
	/* Master switch: if false, the component ignores all incoming damage. */
	UPROPERTY(EditAnywhere, Category = "Durability")
	bool bEnabled;

	/* Called every time damage is applied to the owner with the amount actually taken, including the hit that depletes the durability. */
	UPROPERTY(BlueprintAssignable, Category = "Durability")
	FCFOnTakingDurabilityDamageSignature OnTakingDurabilityDamage;

	/* Called when the owner gets healed. */
	UPROPERTY(BlueprintAssignable, Category = "Durability")
	FCFOnDurabilityHealedSignature OnDurabilityHealed;

	/* Called when an incoming damage decreases the durability to zero, right after 'OnTakingDurabilityDamage' for that same hit. */
	UPROPERTY(BlueprintAssignable, Category = "Durability")
	FCFOnDurabilitySimpleDelegateSignature OnDurabilityDepleted;

	/* Called when the max durability of this component changes. */
	UPROPERTY(BlueprintAssignable, Category = "Durability")
	FCFOnDurabilitySimpleDelegateSignature OnMaxDurabilityChanged;

public:	
	// Sets default values for this component's properties
	UCFDurabilityComponent();

	//Getters
	FORCEINLINE float GetMaxDurability() const { return MaxDurability; }
	FORCEINLINE float GetCurrentDurability() const { return CurrentDurability; }
	FORCEINLINE bool GetEnableIFrames() const { return bEnableDamageIFrames; }
	FORCEINLINE float GetIFramesWindowDuration() const { return IFramesWindowDuration; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/* Re-fills the owner's health to the maximum. */
	UFUNCTION(BlueprintCallable, Category = "Durability")
	void RestoreMaxDurability();

	/* Heals the owner the given amount. */
	UFUNCTION(BlueprintCallable, Category = "Durability")
	void RefillDurability(float HealedAmount);

	/* Changes the max durability of this object, applying limits when needed. */
	UFUNCTION(BlueprintCallable, Category = "Durability")
	void SetMaxDurability(float NewMaxDurability);

	/* Whether the owner should enter a state of temporal invulnerability after being damaged. */
	UFUNCTION(BlueprintCallable, Category = "Durability")
	void SetEnableDamageIFrames(bool bEnable);

	/* Changes the IFrame window duration. */
	void SetIFramesDurationInSeconds(float Duration);

	/* Called to initialize durability without doing any callbacks, usually called on the owner's constructor. */
	void InitDurability(float InValue);

	/* Forcefully changes the current durability. */
	UFUNCTION(BlueprintCallable, Category = "Durability")
	void OverrideCurrentDurability(float InValue);

	/* Registers the given damage processor using the class to create an instance. */
	UFUNCTION(BlueprintCallable, Category = "Durability", Meta = (DeterminesOutputType = "ProcessorClass"))
	UCFDamageProcessorBase* RegisterProcessorFromClass(TSubclassOf<UCFDamageProcessorBase> ProcessorClass);

	/* Adds an instance of a damage processor to the stack. */
	UFUNCTION(BlueprintCallable, Category = "Durability")
	void AddDamageProcessor(UCFDamageProcessorBase* Processor);

	/* Removes the given damage processor. */
	UFUNCTION(BlueprintCallable, Category = "Durability")
	void RemoveDamageProcessor(UCFDamageProcessorBase* Processor);

protected:
	/* Obtains the damage factor for this component, checking the owner for attributes if they have them. */
	virtual float GetDamageFactor() const;

private:
	/* Handles the TakeDamage call from the base actor. */
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser );

	/* Triggers the 'iframes' windows of invulnerability. */
	void ApplyIFrames();

	/* Triggered when the iframes window completes. */
	void OnIFramesWindowExpired();
};
