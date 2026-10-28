// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EffectorBlocks/CFEffectorBlock.h"
#include "CFPlayMontageOnOwner.generated.h"

//fwd
class UAnimMontage;
class UAnimInstance;

/**
 * Play a montage on the owner.
 * 
 * If a SkeletalMeshComponent is given as parameter, it will use said AnimInstance to run it, otherwise it will search
 * for the first SkeletalMesh and cache it instead. 
 */
UCLASS()
class CFCOMBAT_API UCFPlayMontageOnOwner : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* Optional object parameter holding the skeletal mesh component to use, if empty it will search for the first available SkeletalMesh instead.  */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag SkeletalMeshComponentParameter;
	
	/* The montage to play. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	TObjectPtr<UAnimMontage> Montage;
	
	/* Whether we should use a parameter for the play rate or not. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bUsePlayRateParameter = false;
	
	/* The play rate to use. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "!bUsePlayRateParameter", EditConditionToggle))
	float PlayRate = 1.0f;
	
	/* A float parameter to use for the PlayRate */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bUsePlayRateParameter", EditConditionToggle))
	FGameplayTag PlayRateParameter;
	
	/* Starting position of the montage. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	float StartingPosition = 0.0f;
	
	/* Starting position of the montage. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FName StartingSection;
	
	/* Whether this should stop every other montage before playing or not. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bShouldStopAllMontages = false;
	
	/* If true, the montage is interrupted when the effector ends prematurely. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bStopMontageOnEffectorEnd = true;
	
	/* Blend out time to use if the effector stop prematurely and the montage is interrupted. */
	UPROPERTY(EditAnywhere, Category = "Effector", meta = (EditCondition = "bStopMontageOnEffectorEnd"))
	float InterruptBlendOutTime = 0.0f;
	
	/* A float/bool parameter that will be setup to true as long as the montage is playing. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	FGameplayTag IsPlayingParameter;
	
	/* The skeletal mesh component we're currently using for playing the montage. */
	TWeakObjectPtr<UAnimInstance> AnimInstancePtr;
	
	/* Cached context for stopping the effector when the montage ends. */
	FCFEffectorBlockContext CachedContext;
	
public:
	//ctor
	UCFPlayMontageOnOwner()
	{}
	
	//~ Begin UCFEffectorBlock Interface. 
	virtual void OnEffectorInit_Implementation(const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End UCFEffectorBlock Interface.
	
private:
	/* Called when the montage ends for whatever reason. */
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted);
};
