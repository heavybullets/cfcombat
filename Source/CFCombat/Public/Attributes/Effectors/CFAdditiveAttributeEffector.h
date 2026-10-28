// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Attributes/Effectors/CFAttributeEffectorBase.h"
#include "CFAdditiveAttributeEffector.generated.h"

/**
 * A simple attribute effector that adds its value to the attribute
 */
UCLASS()
class CFCOMBAT_API UCFAdditiveAttributeEffector : public UCFAttributeEffectorBase
{
	GENERATED_BODY()

protected:
	/* The value to add. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	float Value;

public:
	//ctor
	UCFAdditiveAttributeEffector();

	//Getters
	FORCEINLINE float GetValue() const { return Value; }

	/* Initializes the value to be used on the effector. */
	void SetValue(float Value);


	//~ Begin UCFAttributeEffectorBase Interface
	virtual float GetModifiedFloatValue_Implementation(FGameplayTag AttributeTag, float AttributeCurrentValue) const override;
	//~ End UCFAttributeEffectorBase Interface
	
};
