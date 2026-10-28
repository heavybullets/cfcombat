// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CFDamageProcessorBase.generated.h"

//fwd
class AActor;
class UDamageType;
class AController;

/**
 * A process block that can be added to the 'durability' component that will pre-process the damage before being applied.
 */
UCLASS(Blueprintable)
class CFCOMBAT_API UCFDamageProcessorBase : public UObject
{
	GENERATED_BODY()

	/* Whether this processor is enabled or not. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Processor", meta = (AllowPrivateAccess = "true"))
	bool bEnabled;
	
public:
	UCFDamageProcessorBase();

	//getters
	FORCEINLINE bool IsEnabled() const { return bEnabled; }

	/* Sets whether this processor should be considered or not. */
	void SetEnabled(bool bNewEnabled);

	/* Process the damage input and returns the damage to apply on the base durability component. */
	UFUNCTION(BlueprintNativeEvent, Category = "Damage Processor")
	float ProcessDurabilityDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
};
