// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attributes/CFGameplayAttributeSet.h"
#include "CFGameplayAttributesComponent.generated.h"

//fwd
class UCFAttributeEffectorBase;

//Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCFOnAttributeEffectorSignature, UCFAttributeEffectorBase*, Effector);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FCFOnAttributeValueChangeSignature, float, OldValue, float, NewValue);

/**
 * A component that manages a set of attributes that can be assigned to GameplayTags and modified using simple effectors.
 * Utilizes the same principles as 'GAS' but on a more simple manner and implementation.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CFCOMBAT_API UCFGameplayAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

	/* The attribute set to use by default when the game starts. */
	UPROPERTY(EditAnywhere, Category = "Attributes")
	FDataTableRowHandle DefaultAttributeSet;

	/* Per-instance attributes that override the values from the default set, or add new ones if the default set doesn't define them. */
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TMap<FGameplayTag, float> InstanceAttributes;

	/* Cached attribute storage. */
	UPROPERTY(Transient)
	TMap<FGameplayTag, float> AttributeStorage;

	/* Effectors currently affecting attributes. */
	UPROPERTY(Transient)
	TArray<UCFAttributeEffectorBase*> RegisteredEffectors;

	/* Holds the listeners for changes for specific attributes. */
	typedef TArray<FCFOnAttributeValueChangeSignature> FCFAttributeListenerArray;
	TMap<FGameplayTag, FCFAttributeListenerArray> AttributeChangeListeners;

	/* Locks updates on attribute changes when doing bulk modifications. */
	bool bLockAttributeChangeListeners;

public:
	/* Delegate called whenever this attribute instance gets added to a gameplay attribute component. */
	UPROPERTY(BlueprintAssignable, Category = "Attributes|Effectors")
	FCFOnAttributeEffectorSignature OnEffectorAdded;

	/* Delegate called whenever this attribute instance gets removed from a gameplay attribute component. */
	UPROPERTY(BlueprintAssignable, Category = "Attributes|Effectors")
	FCFOnAttributeEffectorSignature OnEffectorRemoved;

public:	
	// Sets default values for this component's properties
	UCFGameplayAttributesComponent();
	
	//Getters
	FORCEINLINE const FDataTableRowHandle& GetDefaultAttributeSet() const { return DefaultAttributeSet; }
	FORCEINLINE const TMap<FGameplayTag, float>& GetInstanceAttributes() const { return InstanceAttributes; }

	/* Add the attributes defined inside the set onto the storage. */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void RegisterAttributesFromSet(FDataTableRowHandle AttributeSet);

	/* Removes every attribute defined on the set from the storage. */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void RemoveAttributesFromSet(FDataTableRowHandle AttributeSet);

	/* Whether the given attribute is currently registered on the system or not. */
	UFUNCTION(BlueprintPure, Category = "Attributes")
	bool HasAttribute(FGameplayTag AttributeTag) const;

	/* Obtains the given float attribute. */
	UFUNCTION(BlueprintPure, Category = "Attributes")
	bool GetFloatAttribute(FGameplayTag AttributeTag, float& OutValue) const;

	/* Obtains the given float attribute without taking effectors into account. */
	UFUNCTION(BlueprintPure, Category = "Attributes")
	bool GetRawFloatAttribute(FGameplayTag AttributeTag, float& OutValue) const;
	
	/* Obtains the given attribute as a boolean. Checks for either existence of the tag and a value other than zero. */
	UFUNCTION(BlueprintPure, Category = "Attributes")
	bool GetBoolAttribute(FGameplayTag AttributeTag) const;
	
	/* Obtains the given attribute as a boolean without taking effectors into account. Checks for either existence of the tag and a value other than zero. */
	UFUNCTION(BlueprintPure, Category = "Attributes")
	bool GetRawBoolAttribute(FGameplayTag AttributeTag) const;

	/* Registers a new float attribute to the storage. */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void RegisterFloatAttribute(FGameplayTag AttributeTag, float Value);

	/* Registers the given attribute effectors using the class to create an instance. */
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DeterminesOutputType = "EffectorClass"))
	UCFAttributeEffectorBase* RegisterEffectorFromClass(TSubclassOf<UCFAttributeEffectorBase> EffectorClass);

	/* Adds an instance of an attribute effector. */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddAttributeEffector(UCFAttributeEffectorBase* Effector);

	/* Removes the given attribute effector. */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void RemoveAttributeEffector(UCFAttributeEffectorBase* Effector);
	
	/* Notifies that a given attribute effector has changed in a way that makes its bound attributes modified as well. */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void NotifyAttributeEffectorChange(UCFAttributeEffectorBase* Effector);

	/* Obtains the list of effectors that can affect the given gameplay tag. */
	UFUNCTION(BlueprintPure, Category = "Attributes")
	TArray<UCFAttributeEffectorBase*> GetEffectorsForTag(FGameplayTag AttributeTag) const;

	/* Obtains the list of effectors of the given type. */
	UFUNCTION(BlueprintPure, Category = "Attributes")
	TArray<UCFAttributeEffectorBase*> GetEffectorsForType(FGameplayTag EffectorTypeTag) const;

	/* Obtains the list of all the effectors currently registered to this component. */
	UFUNCTION(BlueprintPure, Category = "Attributes")
	TArray<UCFAttributeEffectorBase*> GetEffectors() const;	

	/* Removes every effector of the given type. */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void RemoveEffectorsOfType(FGameplayTag EffectorTypeTag, int32 RemoveLimit = 0);

	/**
	 * Adds a listener for value changes (triggered either by registration or effectors being added/removed) to the given attribute.
	 * This does not catch dynamic changes due to the effectors having dynamic changes.
	 */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddAttributeChangeListener(FGameplayTag AttributeTag, FCFOnAttributeValueChangeSignature Listener);

	/* Removes the given listener for value changes on attributes. */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void RemoveAttributeChangeListener(FGameplayTag AttributeTag, FCFOnAttributeValueChangeSignature Listener);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//Called when the specific attribute changes
	virtual void NotifyAttributeChange(FGameplayTag AttributeTag, float OldValue, float NewValue);
};
