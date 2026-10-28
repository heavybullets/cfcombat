// Copyright Critical Failure Studio Ltd.

#include "Combat/CFCombatSystem.h"

UCFCombatSystem::UCFCombatSystem()
{}

const TArray<FCFConditionalLink>& UCFCombatSystem::GetRootNodeLinks() const
{ 
	return Entry.Links;
}

FCFCombatSystemSection const* UCFCombatSystem::GetComboSectionWithId(FName Id) const
{
	return ComboSections.Find(Id);
}

void UCFCombatSystem::AppendModifierCombatSystem(const UCFCombatSystem* ModifierSystem)
{
	if(ModifierSystem && !ActiveModifierInstanceMap.Contains(ModifierSystem))
	{
		//Duplicate the modifier system so we don't modify the original one
		UCFCombatSystem* NewModifierSystem = DuplicateObject(ModifierSystem, this);
		ActiveModifierInstanceMap.Add(ModifierSystem, NewModifierSystem);
		
		//Append root conditions
		Entry.Links.Append(NewModifierSystem->Entry.Links);

		//Append Combo Sections
		ComboSections.Append(NewModifierSystem->ComboSections);

		//Append interrupts
		SystemInterrupts.Append(NewModifierSystem->SystemInterrupts);
	}
}

void UCFCombatSystem::RemoveModifierCombatSystem(const UCFCombatSystem* ModifierSystem)
{
	if(ModifierSystem && ActiveModifierInstanceMap.Contains(ModifierSystem))
	{
		//Find the actual instance of this modifier
		UCFCombatSystem* ActiveModifierSystem = ActiveModifierInstanceMap[ModifierSystem];
		
		//Remove root conditions
		for (FCFConditionalLink& Link : ActiveModifierSystem->Entry.Links)
		{
			const int32 FoundLink = Entry.Links.IndexOfByPredicate([Link](const FCFConditionalLink& TestLink) { return TestLink.SectionId == Link.SectionId; });
			if (FoundLink != INDEX_NONE)
			{
				Entry.Links.RemoveAt(FoundLink);	
			}
		}
		
		//Remove Combo Sections
		for (const TPair<FName, FCFCombatSystemSection>& KVPair : ActiveModifierSystem->ComboSections)
		{
			ComboSections.Remove(KVPair.Key);
		}

		//Append interrupts
		for (const TObjectPtr<UCFConditionBlock>& Interrupt : ActiveModifierSystem->SystemInterrupts)
		{
			const int32 FoundInterrupt = SystemInterrupts.Find(Interrupt);
			if (FoundInterrupt != INDEX_NONE)
			{
				SystemInterrupts.RemoveAt(FoundInterrupt);
			}
		}

		//Remove from the active mapping
		ActiveModifierInstanceMap.Remove(ModifierSystem);
	}
}
