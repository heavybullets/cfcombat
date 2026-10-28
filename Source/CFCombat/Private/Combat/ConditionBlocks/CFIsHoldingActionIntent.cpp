// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFIsHoldingActionIntent.h"
#include "Combat/CFActionIntentBuffer.h"
#include "Combat/CFActionIntent.h"

UCFIsHoldingActionIntent::UCFIsHoldingActionIntent()
{}

bool UCFIsHoldingActionIntent::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	if (Intent && Intent->GetIntentType() == ECFActionIntentType::MultiFrame)
	{
		bool bIsHoldingIntent = false;
		for (const FCFActionIntentBufferEntry& Entry : Context.IntentBuffer.Entries)
		{
			if (Entry.Intent == Intent)
			{
				bIsHoldingIntent = Entry.Type == ECFActionIntentBufferEntryType::Held;
			}
		}

		return bIsHoldingIntent;
	}

	return false;
}
