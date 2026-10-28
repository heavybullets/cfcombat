// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFHasActionIntentChord.h"
#include "Combat/CFActionIntentBuffer.h"
#include "Combat/CFActionIntent.h"

UCFHasActionIntentChord::UCFHasActionIntentChord()
{}

bool UCFHasActionIntentChord::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	//Create a set with the intents we wish to test
	TMap<UCFActionIntent*, FCFIntentChordEntry> IntentMap;
	IntentMap.Reserve(ChordEntries.Num());
	for (const FCFIntentChordEntry& Entry : ChordEntries)
	{
		IntentMap.Add(Entry.Intent, Entry);
	}

	//We will abuse the fact that the intent buffer is ordered in time, so anytime we find a new timestamp, the chord will need to be re-evaluated.
	float CurrentTime = 0.0f;
	bool bChordActivated = false;
	TMap<UCFActionIntent*, FCFIntentChordEntry> RemainingIntentsMap;
	for (const FCFActionIntentBufferEntry& Entry : Context.IntentBuffer.Entries)
	{
		//Check if we're on a new frame time
		if (CurrentTime != Entry.Timestamp)
		{
			CurrentTime = Entry.Timestamp;
			bChordActivated = false;
			RemainingIntentsMap = IntentMap;
		}

		//Check if this entry belongs to any of the remaining ones
		const FCFIntentChordEntry* pChordEntry = RemainingIntentsMap.Find(Entry.Intent.Get());
		if (pChordEntry && Entry.Type != ECFActionIntentBufferEntryType::Unregistered)
		{
			//We found an intent, check activation
			if (!bChordActivated && bRequireActivation && pChordEntry->bCanActivateChord)
			{
				bChordActivated |= Entry.Type == ECFActionIntentBufferEntryType::Registered;
			}

			//Remove from the list of pending intents and check if we have completed the chord
			RemainingIntentsMap.Remove(Entry.Intent.Get());
			if (RemainingIntentsMap.Num() == 0 && (!bRequireActivation || bChordActivated))
			{
				return true;
			}
		}
	}

	return false;
}
