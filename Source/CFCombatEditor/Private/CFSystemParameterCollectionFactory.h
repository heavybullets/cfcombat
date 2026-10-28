// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CFSystemParameterCollectionFactory.generated.h"

/**
 * Factory for creating "Action Intents" assets.
 */
UCLASS()
class UCFSystemParameterCollectionFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	//ctor
	UCFSystemParameterCollectionFactory(const FObjectInitializer& ObjectInitializer);

	/* Create a new object. */
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
