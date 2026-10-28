// Copyright Critical Failure Studio Ltd.

#include "CFSystemParameterCollectionFactory.h"
#include "Combat/CFSystemParameterCollection.h"

UCFSystemParameterCollectionFactory::UCFSystemParameterCollectionFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = false;
	SupportedClass = UCFSystemParameterCollection::StaticClass();
}

UObject* UCFSystemParameterCollectionFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UCFSystemParameterCollection* NewSystemParams = NewObject<UCFSystemParameterCollection>(InParent, InClass, InName, Flags);
	return NewSystemParams;
}

