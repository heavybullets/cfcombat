// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CFCombatSystem.generated.h"

//fwd
class UCFEffectorBlock;
class UCFConditionBlock;

/**
 * Represents a link to another combo section that is gated against condition blocks.
 */
USTRUCT()
struct FCFConditionalLink
{
	GENERATED_BODY()

	/* Name of the combo stage this link points toward. */
	UPROPERTY(EditAnywhere, Category = "Combat System")
	FName SectionId;

	/* Entry conditions for this link, if they all succeed the stage can be entered. */
	UPROPERTY(EditAnywhere, Instanced, Category = "Combat System")
	TArray<TObjectPtr<UCFConditionBlock>> EntryConditions;
};

/**
 * Root node of the combat system, contains a list of entry points to stages and their conditions for triggering.
 */
USTRUCT()
struct FCFCombatSystemRoot
{
	GENERATED_BODY()

	/**
	 * List of connections to other sections. They need all their conditions to be valid in order to go to the next section
	 * The list is reviewed in order, so earlier entries have priority for the ones below it.
	 */
	UPROPERTY(EditAnywhere, Category = "Combat System")
	TArray<FCFConditionalLink> Links;
};

/**
 * Represents a connection between combo stages that that has a buffer window attached to it.
 */
USTRUCT()
struct FCFComboWindow
{
	GENERATED_BODY()

	/* Link to the next combo stage. */
	UPROPERTY(EditAnywhere, Category = "Combat System")
	FCFConditionalLink Target;

	/* If true, the combo window opens a buffer for checking intents. */
	UPROPERTY(EditAnywhere, Category = "Combat System")
	bool bEnableIntentBuffer;

	/* The time in which the buffer for this combo window starts receiving input. */
	UPROPERTY(EditAnywhere, Category = "Combat System", meta = (EditCondition = "bEnableIntentBuffer", UIMin = "0.0", ClampMin = "0.0"))
	float BufferStartTime;

	/* Optional duration that will close the buffer after the given time has been completed, not allowing more inputs to be processed. */
	UPROPERTY(EditAnywhere, Category = "Combat System", meta = (EditCondition = "bEnableIntentBuffer", UIMin = "0.0", ClampMin = "0.0"))
	float BufferDuration;

public:
	//ctor
	FCFComboWindow()
	{}

	FCFComboWindow(float InBufferStartTime, float InBufferDuration)
		: BufferStartTime(InBufferStartTime)
		, BufferDuration(InBufferDuration)
	{}
};

/**
 * Represents how we connect back to the neutral/root combo section.
 * Activating the intent buffer for the neutral state allows us to preemptively buffer intents to be used when we get back to neutral.
 */
 USTRUCT()
 struct FCFComboRootBufferSettings
{
	GENERATED_BODY()

	/* If true, the combo window opens a buffer for checking intents. */
	UPROPERTY(EditAnywhere, Category = "Combat System")
	bool bEnableRootIntentBuffer;

	/* The time in which the buffer for this combo window starts receiving input. */
	UPROPERTY(EditAnywhere, Category = "Combat System", meta = (EditCondition = "bEnableRootIntentBuffer", UIMin = "0.0", ClampMin = "0.0"))
	float BufferStartTime;

public:
	//ctor
	FCFComboRootBufferSettings()
	{}

	FCFComboRootBufferSettings(float InBufferStartTime)
		: bEnableRootIntentBuffer(false)
		, BufferStartTime(InBufferStartTime)
	{}
};

/**
 * A state in the combat system machine.
 */
USTRUCT()
struct FCFCombatSystemSection
{
	GENERATED_BODY()

	/* Effectors that can be added to this combo step. */
	UPROPERTY(EditAnywhere, Instanced, Category = "Combat System")
	TArray<TObjectPtr<UCFEffectorBlock>> Effectors;

	/* End conditions for this step, will finish the combo-chain and reset the system to its root. */
	UPROPERTY(EditAnywhere, Instanced, Category = "Combat System")
	TArray<TObjectPtr<UCFConditionBlock>> EndConditions;

	/* The combo windows that can connect to other combo stages. */
	UPROPERTY(EditAnywhere, Category = "Combat System")
	TArray<FCFComboWindow> OutWindows;

	/* Defines how we connect back to the root/neutral state if no combo windows have been activated. */
	UPROPERTY(EditAnywhere, Category = "Combat System")
	FCFComboRootBufferSettings RootBufferSettings;
};

/**
 * The combat system defines the rules, transitions and actions that the character can take when interfacing with the combat component.
 * It defines the rules and it's the 'blueprint' or the 'combat-set' that a character has available.
 *  
 * Rules can be set by adding 'combo steps' with varying entry and exit rules (like actor state and button presses), while combo windows can be created to allow for interconnected combo-flows.
 * Effectors can be added to each of these combo steps to modify how the player behaves during the steps.
 * Collision shapes can be spawned in the world to manage collision logic.
 * 
 * It's basically a state machine with defined states (called sections) and transitions, that will have effectors applied to them to affect the world and player while executing them.
 */
UCLASS()
class CFCOMBAT_API UCFCombatSystem : public UObject
{
	GENERATED_BODY()

	/* Root of the combat system, used as entry point when we aren't inside a combo and wish to begin a new one. */
	UPROPERTY(EditAnywhere, Category = "Combat System")
	FCFCombatSystemRoot Entry;
	
	/* List of steps that the combat system can have, indexed by a key for connections. */
	UPROPERTY(EditAnywhere, Category = "Combat System")
	TMap<FName, FCFCombatSystemSection> ComboSections;

	/* Global 'End Conditions' that will forcefully interrupt any running section back to the root entry when any of these conditions returns true. */
	UPROPERTY(EditAnywhere, Instanced, Category = "Combat System")
	TArray<TObjectPtr<UCFConditionBlock>> SystemInterrupts;

	/* Instances of modifiers that were used to modify this system. */
	UPROPERTY(Transient)
	TMap<const UCFCombatSystem*, UCFCombatSystem*> ActiveModifierInstanceMap;

public:
	//ctor
	UCFCombatSystem();

	/* Obtains the conditional links that stem from the root node and point to the next stages of the system. */
	const TArray<FCFConditionalLink>& GetRootNodeLinks() const;

	/* Request the combat system for a section with the given ID. Can return null if the section doesn't exist. */
	FCFCombatSystemSection const* GetComboSectionWithId(FName Id) const;

	/* Obtains the global interrupts for this system. */
	const TArray<TObjectPtr<UCFConditionBlock>>& GetGlobalSystemInterrupts() const { return SystemInterrupts; }

	/* Appends the given combat system conditionals and states to this one. */
	void AppendModifierCombatSystem(const UCFCombatSystem* ModifierSystem);

	/* Appends the given combat system conditionals and states to this one. */
	void RemoveModifierCombatSystem(const UCFCombatSystem* ModifierSystem);
};
