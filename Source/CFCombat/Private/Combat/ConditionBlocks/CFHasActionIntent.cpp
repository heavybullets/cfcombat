// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFHasActionIntent.h"
#include "Combat/CFActionIntentBuffer.h"

UCFHasActionIntent::UCFHasActionIntent()
	: EntryType(ECFActionIntentBufferEntryType::Registered)
{}

bool UCFHasActionIntent::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	bool bIntentFound = false;
	for (const FCFActionIntentBufferEntry& Entry : Context.IntentBuffer.Entries)
	{
		if (Entry.Intent == Intent && Entry.Type == EntryType)
		{
			bIntentFound = true;
			break;
		}
	}

	return bIntentFound;
}
