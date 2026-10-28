// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Attributes/Effectors/CFAttributeEffectorBase.h"
#include "CFOverrideAttributeEffector.generated.h"

/**
 * A simple effector that overrides the current value of an attribute.
 */
UCLASS()
class CFCOMBAT_API UCFOverrideAttributeEffector : public UCFAttributeEffectorBase
{
	GENERATED_BODY()

	/* The new value to override. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	float NewValue;

public:
	//ctor
	UCFOverrideAttributeEffector();

	//Getters
	FORCEINLINE float GetValue() const { return NewValue; }

	/* Initializes the value to be used on the effector. */
	void SetValue(float Value);

	//~ Begin UCFAttributeEffectorBase Interface
	virtual float GetModifiedFloatValue_Implementation(FGameplayTag AttributeTag, float AttributeCurrentValue) const override;
	//~ End UCFAttributeEffectorBase Interface
};
