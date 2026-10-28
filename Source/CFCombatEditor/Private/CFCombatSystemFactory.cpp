// Copyright Critical Failure Studio Ltd.

#include "CFCombatSystemFactory.h"
#include "Combat/CFCombatSystem.h"

UCFCombatSystemFactory::UCFCombatSystemFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = false;
	SupportedClass = UCFCombatSystem::StaticClass();
}

UObject* UCFCombatSystemFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UCFCombatSystem* NewCombatSystem = NewObject<UCFCombatSystem>(InParent, InClass, InName, Flags);
	return NewCombatSystem;
}

