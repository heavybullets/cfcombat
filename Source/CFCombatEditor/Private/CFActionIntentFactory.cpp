// Copyright Critical Failure Studio Ltd.

#include "CFActionIntentFactory.h"
#include "Combat/CFActionIntent.h"

UCFActionIntentFactory::UCFActionIntentFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = false;
	SupportedClass = UCFActionIntent::StaticClass();
}

UObject* UCFActionIntentFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UCFActionIntent* NewIntent = NewObject<UCFActionIntent>(InParent, InClass, InName, Flags);
	return NewIntent;
}

