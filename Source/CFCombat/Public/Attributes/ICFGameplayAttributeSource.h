// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICFGameplayAttributeSource.generated.h"

//fwd
class UCFGameplayAttributesComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCFGameplayAttributeSource : public UInterface
{
	GENERATED_BODY()
};

 /**
 * Simple interface for making it easy to access the attribute component of an actor.
 */
class CFCOMBAT_API ICFGameplayAttributeSource
{
	GENERATED_BODY()

public:	
	/* Obtain the attribute component for this source. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attributes")
	UCFGameplayAttributesComponent* GetCFAttributesComponent() const;
};
