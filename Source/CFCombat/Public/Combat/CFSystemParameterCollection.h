// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "CFSystemParameterCollection.generated.h"

/**
 * Defines a collection of parameters that can be passed around by effectors and conditions.
 * Parameters are tagged with GameplayTags for easier lookups.
 * 
 * Default implementation allows for 'float', 'vector' and 'object' parameters to be passed.
 */
UCLASS()
class CFCOMBAT_API UCFSystemParameterCollection : public UObject
{
	GENERATED_BODY()
	
	/* Parameter list: float values. */
	UPROPERTY(EditAnywhere, Category = "System Parameters")
	TMap<FGameplayTag, float> FloatParameters;

	/* Parameter list: vector values. */
	UPROPERTY(EditAnywhere, Category = "System Parameters")
	TMap<FGameplayTag, FVector> VectorParameters;

	/* Parameter list: objects. */
	UPROPERTY(EditAnywhere, Category = "System Parameters")
	TMap<FGameplayTag, UObject*> ObjectParameters;

public:
	//ctor
	UCFSystemParameterCollection();

	/* Obtains the given float parameter. */
	UFUNCTION(BlueprintPure, Category = "System Parameters")
	bool GetFloatParameter(FGameplayTag ParameterTag, float &OutValue) const;

	/* Obtains the given vector parameter. */
	UFUNCTION(BlueprintPure, Category = "System Parameters")
	bool GetVectorParameter(FGameplayTag ParameterTag, FVector& OutValue) const;

	/* Obtains the given object parameter. */
	UFUNCTION(BlueprintPure, Category = "System Parameters")
	UObject* GetObjectParameter(FGameplayTag ParameterTag) const;

	/* Sets the given parameter value if it was already registered. */
	UFUNCTION(BlueprintCallable, Category = "System Parameters")
	void SetFloatParameter(FGameplayTag ParameterTag, float NewValue);

	/* Sets the given parameter value if it was already registered. */
	UFUNCTION(BlueprintCallable, Category = "System Parameters")
	void SetVectorParameter(FGameplayTag ParameterTag, const FVector& NewValue);

	/* Sets the given parameter value if it was already registered. */
	UFUNCTION(BlueprintCallable, Category = "System Parameters")
	void SetObjectParameter(FGameplayTag ParameterTag, UObject* NewValue);

	/* A cast version of the object parameter getter. */
	template <typename T>
	T* GetObjectParameter(FGameplayTag ParameterTag) const
	{
		return Cast<T>(GetObjectParameter(ParameterTag));
	}
};
