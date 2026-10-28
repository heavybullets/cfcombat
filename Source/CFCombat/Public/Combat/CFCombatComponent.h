// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/CFActionIntentBuffer.h"
#include "CFCombatComponent.generated.h"

//fwd
class UCFCombatSystem;
class UCFSystemParameterCollection;
class UCFIntentDriver;
struct FCFCombatSystemSection;

//defs
typedef SIZE_T FActionIntentBufferKey;

//delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCFOnComboSectionChangeSignature, FName, SectionName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCFOnComboFinishedSignature);

/**
 * Manages combat and combo rules and states for a given actor by using a 'Combat System' to drive the logic itself.
 * This is the class that 'runs' the combat system and executes the logic defined on it.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CFCOMBAT_API UCFCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	/* The main combat system currently bound to this combat component. */
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UCFCombatSystem> CombatSystem;

	/* Additional combat systems that will be appended to the main system, modifying and expanding it. */
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<TObjectPtr<UCFCombatSystem>> ModifierCombatSystems;

	/* The system parameters that the combat system will use to pass data between its effectors and conditions. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCFSystemParameterCollection> SystemParameters;

	/* The intent driver that optionally runs for this combat component. */
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UCFIntentDriver> IntentDriverClass;

	/* Instance of the intent driver. */
	UPROPERTY(Transient)
	TObjectPtr<UCFIntentDriver> IntentDriver;

	/* Instance of the combat system we're using. */
	UPROPERTY(Transient)
	TObjectPtr<UCFCombatSystem> CombatSystemInstance;

	/* Current state we're pointing to on the combat system. Null if we're on the root and haven't entered a section. */
	FCFCombatSystemSection const* CurrentSection;

	/* Used to store the ID of the current section we're in, used for notification purposes. */
	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FName CurrentSectionName;

	/* Amount of time elapsed while on this section. */
	float CurrentSectionTime;

	/* The intent buffers currently listening to actions. */
	TMap<FActionIntentBufferKey, FCFActionIntentBuffer> IntentBuffers;

	/* The root section intent buffer. */
	FCFActionIntentBuffer RootIntentBuffer;

	/* Currently open 'multi-frame' intents. */
	TArray<UCFActionIntent*> OpenActionIntents;

	//Deferred changes we wish to apply next frame
	TWeakObjectPtr<UCFCombatSystem> DeferredCombatSystemChange;
	TArray<TWeakObjectPtr<UCFCombatSystem>> DeferredCombatSystemModifierRemovals;
	TArray<TWeakObjectPtr<UCFCombatSystem>> DeferredCombatSystemModifierAdditions;
	TSubclassOf<UCFIntentDriver> DeferredIntentDriverClassChange;
	bool bPendingResetState;
	bool bPendingDeactivation;

	/* Static lock used to make sure we can only ever trickle down to one state and avoid infinite recursion. */
	bool bLockSectionProcessOnChange;

public:
	/* Called when the combo jumps to a new valid section. */
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FCFOnComboSectionChangeSignature OnComboSectionStart;

	/* Called when the given section stops being the active one and ends execution. */
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FCFOnComboSectionChangeSignature OnComboSectionEnd;

	/* Called when the combo finishes and the system goes back to its root. */
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FCFOnComboFinishedSignature OnComboFinished;

public:	
	// Sets default values for this component's properties
	UCFCombatComponent();

	//getters
	FORCEINLINE UCFSystemParameterCollection* GetSystemParameters() const { return SystemParameters; }
	FORCEINLINE FName GetCurrentSectionName() const { return CurrentSectionName; }

	// Called when the game starts
	virtual void BeginPlay() override;

	/* Enables the combat system again: the component resumes ticking. The engine also calls this while registering auto activate components, before BeginPlay, where it does nothing. */
	virtual void Activate(bool bReset = false) override;

	/* Disables the combat system: the running section is closed on the next tick (its effectors end), the intents are discarded and the component stops ticking. Intents are ignored while inactive. */
	virtual void Deactivate() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Register the given intent into the buffer. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RegisterActionIntent(UCFActionIntent* Intent);

	/* Closes the given 'multi frame' open intent. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UnregisterMultiFrameActionIntent(UCFActionIntent* Intent);

	/* Hot swaps the combat system for another one, keeping the parameters and intent driver. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCombatSystem(UCFCombatSystem* NewCombatSystem);

	/* Hot swaps the intent driver for an instance of the given class, applied on the next frame. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetIntentDriverClass(TSubclassOf<UCFIntentDriver> NewIntentDriverClass);

	/* Interrupts the current state of the combat system and sends it back to root. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ResetState();

	/* Merges the given combat system as a modifier combat system. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AppendModifierCombatSystem(UCFCombatSystem * ModifierSystem);

	/* Removes the given modifier combat system. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveModifierCombatSystem(UCFCombatSystem * ModifierSystem);

	/* Restores the original combat system and modifiers. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RestoreOriginalCombatSystem();

private:
	/* Process the given section of the combat system. */
	void ProcessCurrentComboSection(float DeltaTime);

	/* Processes the entry point conditions and possibly enters a new section. */
	void ProcessRootEntryConditions();

	/* Changes the current stage of the system we're in. 'nullptr' returns the system to the root. */
	void SetCurrentComboSection(FCFCombatSystemSection const* NewSection, FName SectionName = NAME_None);

	/* Clears the combo buffer completely and closes any 'buffer window' that might be open. */
	void ClearComboBuffer(int32 ExpectedBufferNum = 0);

	/* Obtains a valid buffer with the given key. */
	FCFActionIntentBuffer* GetIntentBufferForKey(FActionIntentBufferKey Key);

	/* Creates a new intent buffer for the given key. */
	FCFActionIntentBuffer& CreateIntentBufferForKey(FActionIntentBufferKey Key);

	/* Updates the active buffers, removing old intents. */
	void FlushExpiredIntentsOnBuffers();

	/* Adds the entries related to every open intent to the buffers that are open. */
	void AddOpenIntentEntriesToBuffers();

	//Transforms the given pointer into a valid key for the intent buffers
	template <typename T>
	FActionIntentBufferKey GetBufferKey(T* Source)
	{
		return reinterpret_cast<FActionIntentBufferKey>(Source);
	}

	/* Applies any pending deferred operation. */
	void ApplyDeferredOperations();

	/* Initializes the combat system appending modifiers and duplicating for local use. */
	void SetupCombatSystemInstance();
};
