// Copyright Critical Failure Studio Ltd.

#include "Combat/CFCombatComponent.h"
#include "Combat/CFCombatSystem.h"
#include "Combat/CFActionIntent.h"
#include "Combat/CFIntentDriver.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Combat/ConditionBlocks/CFConditionBlock.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"

UCFCombatComponent::UCFCombatComponent()
	: CurrentSection(nullptr)
	, CurrentSectionTime(0.0f)
	, bPendingResetState(false)
	, bPendingDeactivation(false)
	, bLockSectionProcessOnChange(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//The combat system runs from the start. Unchecking this on an instance makes the component start dormant until it is activated
	bAutoActivate = true;
	// ...
}

void UCFCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	//A component that is not active yet waits for Activate to start ticking
	SetComponentTickEnabled(IsActive());

	//If we have a parameter list, duplicate it so we work on our own copy, even if there's no combat system yet
	if (SystemParameters)
	{
		SystemParameters = DuplicateObject(SystemParameters, this);
	}

	//Sanity check, we need a combat system
	if (!ensureMsgf(CombatSystem, TEXT("Combat component on '%s' has no CombatSystem assigned."), *GetNameSafe(GetOwner())))
	{
		return;
	}
	
	//Initialize the root first, needs to have their buffer created
	SetupCombatSystemInstance();
	RootIntentBuffer.bBufferOpen = true;

	//Create the intent driver
	if (IntentDriverClass)
	{
		IntentDriver = NewObject<UCFIntentDriver> (this, IntentDriverClass);
		IntentDriver->Init(this);
	}
	
	//Initialize the start conditions
	FCFConditionBlockContext Context(GetOwner(), SystemParameters);
	Context.IntentBuffer = RootIntentBuffer;
	for (const FCFConditionalLink& Link : CombatSystemInstance->GetRootNodeLinks())
	{
		//Process this link and see if it passes
		for (UCFConditionBlock* Condition : Link.EntryConditions)
		{
			if (Condition)
			{
				Condition->InitCondition(Context);
			}
		}
	}
}

void UCFCombatComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	//The engine activates auto activate components while registering, before BeginPlay has prepared the combat system. BeginPlay enables the tick by itself in that case
	if (HasBegunPlay() && IsActive())
	{
		SetComponentTickEnabled(true);
	}
}

void UCFCombatComponent::Deactivate()
{
	const bool bWasActive = IsActive();
	Super::Deactivate();

	//The cleanup is deferred like every other operation, since this can be called from inside a section (an effector, a condition, a delegate). The tick keeps running until it is applied
	if (bWasActive && HasBegunPlay())
	{
		bPendingDeactivation = true;
	}
}

void UCFCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Flush any operation needed for update
	ApplyDeferredOperations();

	//A deactivated combat system processes nothing, the deferred operations above are what turn the tick off
	if (!IsActive())
	{
		return;
	}

	//Can only run with a combat system attached to this
	if (CombatSystemInstance)
	{
		//First clean any old intent that shouldn't be processed by the entry conditions anymore due to being expired, and register the 'held' open intents
		FlushExpiredIntentsOnBuffers();
		AddOpenIntentEntriesToBuffers();

		//Check if we're on a given section
		if (CurrentSection)
		{
			ProcessCurrentComboSection(DeltaTime);
		}
		else
		{
			ProcessRootEntryConditions();
		}
	}
}

void UCFCombatComponent::RegisterActionIntent(UCFActionIntent* Intent)
{
	if (Intent && IsActive())
	{
		if (IntentBuffers.Num() > 0 || RootIntentBuffer.bBufferOpen)
		{
			FCFActionIntentBufferEntry Entry;
			Entry.Intent = Intent;
			Entry.Timestamp = GetWorld()->GetTimeSeconds();
			Entry.Type = ECFActionIntentBufferEntryType::Registered;

			//Secondary intent buffers
			for (TPair<FActionIntentBufferKey, FCFActionIntentBuffer>& Kvp : IntentBuffers)
			{
				FCFActionIntentBuffer& Buffer = Kvp.Value;
				if (Buffer.bBufferOpen)
				{
					Buffer.Entries.Add(Entry);
				}
			}

			//Root intent buffer
			if (RootIntentBuffer.bBufferOpen)
			{
				RootIntentBuffer.Entries.Add(Entry);
			}
		}

		//Independent of whether a buffer is open or not, we want to buffer multi-frame intents
		if (Intent->GetIntentType() == ECFActionIntentType::MultiFrame)
		{
			OpenActionIntents.AddUnique(Intent);
		}
	}
}

void UCFCombatComponent::UnregisterMultiFrameActionIntent(UCFActionIntent* Intent)
{
	if (Intent && IsActive() && Intent->GetIntentType() == ECFActionIntentType::MultiFrame)
	{
		//Remove from any open buffer
		if (IntentBuffers.Num() > 0 || RootIntentBuffer.bBufferOpen)
		{
			FCFActionIntentBufferEntry Entry;
			Entry.Intent = Intent;
			Entry.Timestamp = GetWorld()->GetTimeSeconds();
			Entry.Type = ECFActionIntentBufferEntryType::Unregistered;

			//Secondary intent buffers
			for (TPair<FActionIntentBufferKey, FCFActionIntentBuffer>& Kvp : IntentBuffers)
			{
				FCFActionIntentBuffer& Buffer = Kvp.Value;
				if (Buffer.bBufferOpen)
				{
					Buffer.Entries.Add(Entry);
				}
			}

			//Root intent buffer
			if (RootIntentBuffer.bBufferOpen)
			{
				RootIntentBuffer.Entries.Add(Entry);
			}
		}

		//Independent of any buffer available, we need to delete this open intent
		OpenActionIntents.Remove(Intent);
	}
}

void UCFCombatComponent::SetCombatSystem(UCFCombatSystem* NewCombatSystem)
{
	if (NewCombatSystem)
	{
		DeferredCombatSystemChange = NewCombatSystem;
	}
}

void UCFCombatComponent::SetIntentDriverClass(TSubclassOf<UCFIntentDriver> NewIntentDriverClass)
{
	DeferredIntentDriverClassChange = NewIntentDriverClass;
}

void UCFCombatComponent::ResetState()
{
	bPendingResetState = true;
}

void UCFCombatComponent::AppendModifierCombatSystem(UCFCombatSystem* ModifierSystem)
{
	if (CombatSystemInstance && ModifierSystem)
	{
		if (ModifierSystem->GetComboSectionWithId(CurrentSectionName) != nullptr)
		{
			DeferredCombatSystemModifierAdditions.AddUnique(ModifierSystem);
		}
		else
		{
			CombatSystemInstance->AppendModifierCombatSystem(ModifierSystem);
		}
	}
}

void UCFCombatComponent::RemoveModifierCombatSystem(UCFCombatSystem* ModifierSystem)
{
	if (CombatSystemInstance && ModifierSystem)
	{
		//Reset to make sure we're not on an invalid state
		if (ModifierSystem->GetComboSectionWithId(CurrentSectionName) != nullptr)
		{
			ResetState();
			DeferredCombatSystemModifierRemovals.AddUnique(ModifierSystem);
		}
		else
		{
			CombatSystemInstance->RemoveModifierCombatSystem(ModifierSystem);	
		}
	}
}

void UCFCombatComponent::RestoreOriginalCombatSystem()
{
	if (CombatSystemInstance)
	{
		//Reset
		ResetState();
		SetupCombatSystemInstance();
	}
}

void UCFCombatComponent::ProcessCurrentComboSection(float DeltaTime)
{
	check(CurrentSection);
	check(CombatSystemInstance);

	//Update the current time
	CurrentSectionTime += DeltaTime;

	
	CurrentSection = CombatSystemInstance->GetComboSectionWithId(CurrentSectionName);
	if (!CurrentSection)
	{
		//TODO: this is a patch to avoid null pointer. The fix need to be done on the state machine
		// Current combo section was removed from map, skip to avoid null pointer error.
		UE_LOG(LogTemp, Warning, TEXT("ERROR: CurrentSection pointer was removed from map"));
		return;
	}
	
	//Process Effectors
	FCFEffectorBlockContext EffectorContext(GetOwner(), SystemParameters, CurrentSectionTime, IntentDriver);
	
	for (UCFEffectorBlock* Effector : CurrentSection->Effectors)
	{
		if (Effector)
		{
			Effector->TickEffector(DeltaTime, EffectorContext);
		}
	}
	
	CurrentSection = CombatSystemInstance->GetComboSectionWithId(CurrentSectionName);
	if (!CurrentSection)
	{
		//TODO: this is a patch to avoid null pointer. The fix need to be done on the state machine
		// Current combo section was removed from map, skip to avoid null pointer error.
		UE_LOG(LogTemp, Warning, TEXT("ERROR: CurrentSection pointer was removed from map"));
		return;
	}

	//Process Exit Conditions
	FCFConditionBlockContext Context(GetOwner(), SystemParameters, CurrentSectionTime);
	for (UCFConditionBlock const* Condition : CurrentSection->EndConditions)
	{
		//If any of these passes, we close the combo
		if (Condition && Condition->GetConditionResult(Context))
		{
			SetCurrentComboSection(nullptr);
			return;
		}
	}

	//Process Global Interrupts
	for (UCFConditionBlock const* Condition : CombatSystemInstance->GetGlobalSystemInterrupts())
	{
		//If any of these passes, we close the combo
		if (Condition && Condition->GetConditionResult(Context))
		{
			SetCurrentComboSection(nullptr);
			return;
		}
	}

	//Process the intent buffers from the combo windows
	bool bRequiresBufferUpdate = false;
	for (const FCFComboWindow& ComboWindow : CurrentSection->OutWindows)
	{
		//Obtain the buffer that corresponds to this combo window and apply to the context
		FCFActionIntentBuffer* Buffer = GetIntentBufferForKey(GetBufferKey(&ComboWindow));
		//Either open/close the buffer
		if (Buffer)
		{
			Buffer->bBufferOpen = CurrentSectionTime >= ComboWindow.BufferStartTime && (ComboWindow.BufferDuration <= 0 || (CurrentSectionTime - ComboWindow.BufferStartTime) < ComboWindow.BufferDuration);
			bRequiresBufferUpdate |= Buffer->bBufferOpen;
		}
	}
	
	//Process the root buffer if its enabled
	if (CurrentSection->RootBufferSettings.bEnableRootIntentBuffer)
	{
		if (!RootIntentBuffer.bBufferOpen)
		{
			RootIntentBuffer.bBufferOpen = CurrentSectionTime >= CurrentSection->RootBufferSettings.BufferStartTime;
		}

		//Trigger an update on the intent driver if needed
		bRequiresBufferUpdate |= RootIntentBuffer.bBufferOpen;
	}

	//Apply to the intent buffer if there's one setup
	if (bRequiresBufferUpdate && IntentDriver)
	{
		FCFIntentDriverUpdateContext Params(GetOwner(), SystemParameters);
		IntentDriver->ProcessIntentBufferUpdate(Params);
	}

	//Process combo window conditions now that the driver has possibly triggered an update
	for (const FCFComboWindow& ComboWindow : CurrentSection->OutWindows)
	{
		//Obtain the buffer that corresponds to this combo window and apply to the context
		FCFActionIntentBuffer* Buffer = GetIntentBufferForKey(GetBufferKey(&ComboWindow));
		if (Buffer)
		{
			Context.IntentBuffer = *Buffer;
		}
		else
		{
			Context.IntentBuffer = FCFActionIntentBuffer();
		}

		//Search for a valid set of conditions that will move us to the next target
		bool bValidConditions = true;
		for (UCFConditionBlock const* Condition : ComboWindow.Target.EntryConditions)
		{
			if (Condition && !Condition->GetConditionResult(Context))
			{
				bValidConditions = false;
				break;
			}
		}

		//Check if the conditions are valid on this point
		if (bValidConditions)
		{
			SetCurrentComboSection(CombatSystemInstance->GetComboSectionWithId(ComboWindow.Target.SectionId), ComboWindow.Target.SectionId);
			break;
		}
	}
}

void UCFCombatComponent::ProcessRootEntryConditions()
{
	check(!CurrentSection);
	check(CombatSystemInstance);

	//Root always has the intent buffer open
	if (IntentDriver)
	{
		FCFIntentDriverUpdateContext Params(GetOwner(), SystemParameters);
		IntentDriver->ProcessIntentBufferUpdate(Params);
	}

	//Simply run through each conditional link and see if there's one that fits our current state
	FCFConditionBlockContext Context(GetOwner(), SystemParameters);
	Context.IntentBuffer = RootIntentBuffer;

	for (const FCFConditionalLink& Link : CombatSystemInstance->GetRootNodeLinks())
	{
		//Process this link and see if it passes
		bool bPassedTests = true;
		for (UCFConditionBlock const* Condition : Link.EntryConditions)
		{
			if (Condition && !Condition->GetConditionResult(Context))
			{
				bPassedTests = false;
				break;
			}
		}

		//Check if the conditions passed successfully
		if (bPassedTests)
		{
			SetCurrentComboSection(CombatSystemInstance->GetComboSectionWithId(Link.SectionId), Link.SectionId);
			break;
		}
	}
}

void UCFCombatComponent::SetCurrentComboSection(FCFCombatSystemSection const* NewSection, FName SectionName)
{
	//No sense changing if we're already there
	if (CurrentSection != NewSection)
	{
		//Make sure to clean the old section effectors
		if (CurrentSection != nullptr)
		{
			const FCFEffectorBlockContext Context(GetOwner(), SystemParameters, CurrentSectionTime, IntentDriver);
			for (UCFEffectorBlock* Effector : CurrentSection->Effectors)
			{
				if (Effector)
				{
					Effector->StopEffector(Context);
				}
			}

			//Notify the section ended
			OnComboSectionEnd.Broadcast(CurrentSectionName);
		}

		//Set the new section
		CurrentSection = NewSection;
		CurrentSectionName = SectionName;
		CurrentSectionTime = 0.0f;
		
		//Update the buffers and notify, depending on where we are
		if (CurrentSection)
		{
			//Reset the main buffer
			RootIntentBuffer.ClearBuffer();
			RootIntentBuffer.bBufferOpen = false;

			//Process the specialized window buffers
			ClearComboBuffer(CurrentSection->OutWindows.Num());
			for (const FCFComboWindow& Window : CurrentSection->OutWindows)
			{
				if (Window.bEnableIntentBuffer)
				{
					FCFActionIntentBuffer& Buffer = CreateIntentBufferForKey(GetBufferKey(&Window));
					Buffer.bBufferOpen = Window.BufferStartTime <= 0;
					
					//No need to have max intent age if the buffer closes after a while
					if (Window.BufferDuration > 0)
					{
						Buffer.MaxIntentAge = -1.0f;
					}
				}
			}

			//Initializes the effectors
			{
				const FCFEffectorBlockContext Context(GetOwner(), SystemParameters, CurrentSectionTime, IntentDriver);
				for (UCFEffectorBlock* Effector : CurrentSection->Effectors)
				{
					if (Effector)
					{
						Effector->InitEffector(Context);
					}
				}	
			}
			
			//Initializes the conditions
			{
				const FCFConditionBlockContext Context(GetOwner(), SystemParameters, CurrentSectionTime);
				for (UCFConditionBlock* Condition : CurrentSection->EndConditions)
				{
					if (Condition)
					{
						Condition->InitCondition(Context);
					}
				}
				
				for (const FCFComboWindow& OutWindow : CurrentSection->OutWindows)
				{
					for (UCFConditionBlock* Condition : OutWindow.Target.EntryConditions)
					{
						if (Condition)
						{
							Condition->InitCondition(Context);
						}
					}
				}
			}

			//Notify
			OnComboSectionStart.Broadcast(CurrentSectionName);

			//Process a single 'zero' frame to make sure anything we want initialized has time to setup
			if (!bLockSectionProcessOnChange)
			{
				bLockSectionProcessOnChange = true;
				ProcessCurrentComboSection(0.0f);
				bLockSectionProcessOnChange = false;
			}
		}
		else
		{
			//Only clear the window buffers, but leave the main intent buffer filled in case we have intents we wish to carry over
			ClearComboBuffer();
			RootIntentBuffer.bBufferOpen = true;
			OnComboFinished.Broadcast();
		}
	}
}

void UCFCombatComponent::ClearComboBuffer(int32 ExpectedBufferNum /* = 1; */)
{
	//We simply clear the map, any buffer that might want to be opened can re-create their buffer
	IntentBuffers.Empty(ExpectedBufferNum);
}

FCFActionIntentBuffer* UCFCombatComponent::GetIntentBufferForKey(FActionIntentBufferKey Key)
{
	return IntentBuffers.Find(Key);
}

FCFActionIntentBuffer& UCFCombatComponent::CreateIntentBufferForKey(FActionIntentBufferKey Key)
{
	return IntentBuffers.Emplace(Key);
}

void UCFCombatComponent::FlushExpiredIntentsOnBuffers()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	for (TPair<FActionIntentBufferKey, FCFActionIntentBuffer>& Kvp : IntentBuffers)
	{
		FCFActionIntentBuffer& Buffer = Kvp.Value;
		if (Buffer.MaxIntentAge > 0.0f)
		{
			int32 NumIndicesToDelete = 0;
			for (const FCFActionIntentBufferEntry& Entry : Buffer.Entries)
			{
				if ((CurrentTime - Entry.Timestamp) > Buffer.MaxIntentAge)
				{
					NumIndicesToDelete++;
				}
				else
				{
					break;
				}
			}

			//Clean any old entries
			if (NumIndicesToDelete > 0)
			{
				Buffer.Entries.RemoveAt(0, NumIndicesToDelete);
			}
		}
	}
}

void UCFCombatComponent::AddOpenIntentEntriesToBuffers()
{
	if (OpenActionIntents.Num())
	{
		const float CurrentTime = GetWorld()->GetTimeSeconds();
		for (TPair<FActionIntentBufferKey, FCFActionIntentBuffer>& Kvp : IntentBuffers)
		{
			FCFActionIntentBuffer& Buffer = Kvp.Value;
			if (Buffer.bBufferOpen)
			{
				//Add every open intent
				Buffer.Entries.Reserve(Buffer.Entries.Num() + OpenActionIntents.Num());
				for (UCFActionIntent* Intent : OpenActionIntents)
				{
					FCFActionIntentBufferEntry& Entry = Buffer.Entries.AddZeroed_GetRef();
					Entry.Intent = Intent;
					Entry.Timestamp = CurrentTime;
					Entry.Type = ECFActionIntentBufferEntryType::Held;
				}
			}
		}

		//Add for the neutral buffer as well
		if (RootIntentBuffer.bBufferOpen)
		{
			//Add every open intent
			RootIntentBuffer.Entries.Reserve(RootIntentBuffer.Entries.Num() + OpenActionIntents.Num());
			for (UCFActionIntent* Intent : OpenActionIntents)
			{
				FCFActionIntentBufferEntry& Entry = RootIntentBuffer.Entries.AddZeroed_GetRef();
				Entry.Intent = Intent;
				Entry.Timestamp = CurrentTime;
				Entry.Type = ECFActionIntentBufferEntryType::Held;
			}
		}
	}
}

void UCFCombatComponent::ApplyDeferredOperations()
{
	if (DeferredCombatSystemChange.IsValid())
	{
		//Close any section that might be running, so the 'end' method trigger for the effectors
		if (CombatSystemInstance)
		{
			SetCurrentComboSection(nullptr);
		}

		//Change the combat system and initialize it back to neutral
		CombatSystem = DeferredCombatSystemChange.Get();
		SetupCombatSystemInstance();
		RootIntentBuffer.ClearBuffer();
		DeferredCombatSystemChange = nullptr;
	}

	if (DeferredIntentDriverClassChange)
	{
		IntentDriver = NewObject<UCFIntentDriver>(this, DeferredIntentDriverClassChange);
		IntentDriver->Init(this);
		DeferredIntentDriverClassChange = nullptr;
	}
	
	if (bPendingResetState)
	{
		SetCurrentComboSection(nullptr);
		bPendingResetState = false;
	}

	if (bPendingDeactivation)
	{
		//Close the running section so its effectors end, and discard every intent so nothing held fires when the component is activated again
		SetCurrentComboSection(nullptr);
		ClearComboBuffer();
		RootIntentBuffer.ClearBuffer();
		OpenActionIntents.Empty();
		bPendingDeactivation = false;

		//The component could have been activated again before this was applied, in which case it keeps ticking
		SetComponentTickEnabled(IsActive());
	}

	//Without an instance (no combat system set) there is nothing to modify, the requests are just discarded below
	if (CombatSystemInstance)
	{
		for (TWeakObjectPtr<UCFCombatSystem> RemovedSystem : DeferredCombatSystemModifierRemovals)
		{
			CombatSystemInstance->RemoveModifierCombatSystem(RemovedSystem.Get());
		}

		for (TWeakObjectPtr<UCFCombatSystem> AddedSystem : DeferredCombatSystemModifierAdditions)
		{
			CombatSystemInstance->AppendModifierCombatSystem(AddedSystem.Get());
		}
	}
	
	DeferredCombatSystemModifierRemovals.Empty();
	DeferredCombatSystemModifierAdditions.Empty();
}

void UCFCombatComponent::SetupCombatSystemInstance()
{
	//Create a new combat system and append any extra systems to it
	CombatSystemInstance = DuplicateObject(CombatSystem, this);

	//Append any external modifier systems
	for (const UCFCombatSystem* ModifierSystem : ModifierCombatSystems)
	{
		CombatSystemInstance->AppendModifierCombatSystem(ModifierSystem);
	}
}
