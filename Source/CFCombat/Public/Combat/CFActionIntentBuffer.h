// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CFActionIntentBuffer.generated.h"

//fwd
class UCFActionIntent;

/**
 * The types of buffer entries that can be added.
 */
UENUM(BlueprintType)
enum class ECFActionIntentBufferEntryType : uint8
{
	Registered,
	Held,
	Unregistered
};

/**
 * Defines an intent that has been registered to the intent buffer.
 */
USTRUCT(BlueprintType)
struct CFCOMBAT_API FCFActionIntentBufferEntry
{
	GENERATED_BODY()

	/* The action intent that was registered. */
	UPROPERTY(BlueprintReadOnly, Category = "Action Intents")
	TWeakObjectPtr<UCFActionIntent> Intent;

	/* The timestamp for this entry, marks when the intent was registered. */
	UPROPERTY(BlueprintReadOnly, Category = "Action Intents")
	float Timestamp;

	/* Type of entry registration. */
	UPROPERTY(BlueprintReadOnly, Category = "Action Intents")
	ECFActionIntentBufferEntryType Type;
};


/**
 * Defines a buffer that can hold action intents for the combat system.
 * Each combo section can open a buffer and process it as they see fit, with the timings they deem correct.
 */
USTRUCT(BlueprintType)
struct CFCOMBAT_API FCFActionIntentBuffer
{
	GENERATED_BODY()

	/* Whether or not this buffer is open for adding intents. */
	UPROPERTY(BlueprintReadOnly, Category = "Action Intents")
	bool bBufferOpen;

	/* If > 0, the maximum age of an intent before it gets thrown out of the buffer. */
	UPROPERTY(BlueprintReadOnly, Category = "Action Intents")
	float MaxIntentAge;

	/* The entries of this buffer, older entries first. */
	UPROPERTY(BlueprintReadOnly, Category = "Action Intents")
	TArray<FCFActionIntentBufferEntry> Entries;

public:
	//ctor
	FCFActionIntentBuffer();

	/* Clears every entry from the buffer. */
	void ClearBuffer();
};
