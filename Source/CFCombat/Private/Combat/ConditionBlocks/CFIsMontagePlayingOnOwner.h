// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "ConditionBlocks/CFConditionBlock.h"
#include "GameplayTagContainer.h"
#include "CFIsMontagePlayingOnOwner.generated.h"

//fwd
class UAnimMontage;
class UAnimInstance;

/**
 * Checks if the owner is currently playing the given montage or not.
 * 
 * If a SkeletalMeshComponent is given as parameter, it will use said AnimInstance to run it, otherwise it will search
 * for the first SkeletalMesh and cache it instead. 
 */
UCLASS()
class CFCOMBAT_API UCFIsMontagePlayingOnOwner : public UCFConditionBlock
{
	GENERATED_BODY()
	
	/* Optional object parameter holding the skeletal mesh component to use, if empty it will search for the first available SkeletalMesh instead.  */
	UPROPERTY(EditAnywhere, Category = "Condition")
	FGameplayTag SkeletalMeshComponentParameter;
	
	/* The montage to play. */
	UPROPERTY(EditAnywhere, Category = "Condition")
	TObjectPtr<UAnimMontage> Montage;	
	
	/* The skeletal mesh component we're currently using for playing the montage. */
	TWeakObjectPtr<UAnimInstance> AnimInstancePtr;
	
public:
	//ctor
	UCFIsMontagePlayingOnOwner()
	{}
	
	//~ Begin UCFConditionBlock
	virtual void OnConditionInit_Implementation(const FCFConditionBlockContext& Context) override;
	virtual bool ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const override;
	//~ End UCFConditionBlock
};
