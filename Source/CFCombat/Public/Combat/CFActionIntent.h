// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CFActionIntent.generated.h"

/* The type of intents. */
UENUM()
enum class ECFActionIntentType : uint8
{
	SingleFrame		UMETA(ToolTip = "This intent fires for a single and can't be held open."),
	MultiFrame		UMETA(ToolTip = "This intent registers as an 'open intent' that needs to be closed manually, it will last several frames until unregistered.")
};

/**
 * An intent describes an action that a character wishes to perform on the combat system.
 * While it is similar to 'inputs' that players can do on their characters (and most intents map 1:1 to inputs), they are not necessarily bound to them.
 * An intent can be a 'chord' of inputs, a special 'event' (like waiting for x seconds) or even AI actions.
 * 
 * Intents are created as assets and can then be 'registered' to 'CombatComponents' to let the system know their owner 'wishes' to perform an action.
 * These action intents are then stored on a buffer and timestamped for the system to use as information to trigger new combo sections.
 */
UCLASS(BlueprintType)
class CFCOMBAT_API UCFActionIntent : public UObject
{
	GENERATED_BODY()

	/* The type of intent which will define how the buffer manages its registration. */
	UPROPERTY(EditAnywhere, Category = "Intent")
	ECFActionIntentType Type;

public:
	//ctor
	UCFActionIntent();

	//Getters
	FORCEINLINE ECFActionIntentType GetIntentType() const { return Type; }
};
