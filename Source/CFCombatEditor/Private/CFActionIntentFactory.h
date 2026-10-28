// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CFActionIntentFactory.generated.h"

/**
 * Factory for creating "Action Intents" assets.
 */
UCLASS()
class UCFActionIntentFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	//ctor
	UCFActionIntentFactory(const FObjectInitializer& ObjectInitializer);

	/* Create a new object. */
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
