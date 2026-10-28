// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CFCombatSystemFactory.generated.h"

/**
 * Factory for creating "Combat System" assets.
 */
UCLASS()
class UCFCombatSystemFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	//ctor
	UCFCombatSystemFactory(const FObjectInitializer& ObjectInitializer);

	/* Create a new object. */
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
