// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CFIntentDriver.generated.h"

//fwd
class UCFActionIntent;
class UCFCombatComponent;
class UCFSystemParameterCollection;
class AActor;

/* Update structure for the intent driver. */
USTRUCT(BlueprintType)
struct FCFIntentDriverUpdateContext
{
	GENERATED_BODY()

	/* The actor that the combat component is working with. */
	UPROPERTY(BlueprintReadOnly, Category = "Intent Driver")
	TWeakObjectPtr<AActor> Owner;

	/* System parameters collections if they exist, can be NULL if not defined. */
	UPROPERTY(BlueprintReadOnly, Category = "Intent Driver")
	UCFSystemParameterCollection* Parameters;

public:
	//ctor
	FCFIntentDriverUpdateContext()
	{}

	FCFIntentDriverUpdateContext(AActor* InOwner, UCFSystemParameterCollection* InParams)
		: Owner(InOwner)
		, Parameters(InParams)
	{}
};


/**
 * An intent driver can be used to generate the 'intents' for a given player/AI using the combat system.
 * It's a perfect place to put logic that will dictate how the stimulus for the combat system get created (via intents).
 * 
 * When used with an AI to generate intents, the IntentDriver can be thought as the 'personality' of the AI system.
 * You can build different AIs that generate different stimulus with the same CombatSystem (having versions that are more aggressive and choose to attack more than others).
 * 
 * For the player, it can be used to bind and map inputs to intents.
 */
UCLASS(Blueprintable, abstract)
class CFCOMBAT_API UCFIntentDriver : public UObject
{
	GENERATED_BODY()
	
	/* The combat component that we're running in. */
	UPROPERTY(BlueprintReadOnly, Category = "Intent", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UCFCombatComponent> OwnerCombatComponent;

	/* The queued intents we wish to register the next time the buffer is open. */
	TArray<UCFActionIntent*> QueuedIntents;	

	/* List of active cooldowns. */
	TMap<UCFActionIntent*, FTimerHandle> ActiveCooldownHandles;

public:
	//ctor
	UCFIntentDriver();

	//getters
	FORCEINLINE UCFCombatComponent* GetOwnerCombatComponent() const { return OwnerCombatComponent.Get(); }

	/* Initializes the intent driver with the given combat component. */
	void Init(UCFCombatComponent* CombatComponentOwner);

	/* Cleanup any pending intent. */
	virtual void BeginDestroy() override;

	/* Called when the intent driver is initialized by its owner combat component. */
	UFUNCTION(BlueprintNativeEvent, Category = "Intent")
	void OnInit(UCFCombatComponent* CombatComponentOwner);

	/* Sets up an intent to be triggered the next time the buffer is open. */
	UFUNCTION(BlueprintCallable, Category = "Intent")
	void EnqueueIntentRegistration(UCFActionIntent* Intent);

	/* Triggered whenever the buffers are open and expecting intents. */
	void ProcessIntentBufferUpdate(const FCFIntentDriverUpdateContext& UpdateContext);

	/* Triggered whenever the buffers are open and expecting intents. */
	UFUNCTION(BlueprintNativeEvent, Category = "Intent")
	void OnRequestIntentBufferUpdate(const FCFIntentDriverUpdateContext& UpdateContext, TArray<UCFActionIntent*>& OutIntents);

	/**
	 * Makes the given intent be on 'cooldown' which will prevent it from triggering on the combat component, even when registered.
	 * A value <=0 clears the cooldown early.
	 */
	UFUNCTION(BlueprintCallable, Category = "Intent")
	void SetIntentCooldown(UCFActionIntent* Intent, float CooldownTime);

	/* Obtain the owner actor of this intent driver. */
	UFUNCTION(BlueprintPure, Category = "Intent")
	AActor* GetOwner() const;

	/* Used to access world-related functions. */
	virtual UWorld* GetWorld() const override;

private:
	/* Deactivates the cooldown for the given intent. */
	void DeactivateIntentCooldown(UCFActionIntent* Intent);
};
