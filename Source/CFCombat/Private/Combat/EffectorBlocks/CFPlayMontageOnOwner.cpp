// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFPlayMontageOnOwner.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

void UCFPlayMontageOnOwner::OnEffectorInit_Implementation(const FCFEffectorBlockContext& Context)
{
	if (Context.Parameters && SkeletalMeshComponentParameter.IsValid())
	{
		USkeletalMeshComponent* SkelMeshComp = Cast<USkeletalMeshComponent>(Context.Parameters->GetObjectParameter(SkeletalMeshComponentParameter));
		AnimInstancePtr = SkelMeshComp->GetAnimInstance();
	}
	else
	{
		USkeletalMeshComponent* SkelMeshComp = Context.Owner->FindComponentByClass<USkeletalMeshComponent>();
		AnimInstancePtr = SkelMeshComp->GetAnimInstance();
	}
}

void UCFPlayMontageOnOwner::OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context)
{
	bool bMontagePlayedSuccesfully = false;
	if (Montage && AnimInstancePtr.IsValid())
	{
		//Obtain the play rate if it is parameterized
		float EffectivePlayRate = PlayRate;
		if (bUsePlayRateParameter && Context.Parameters && !Context.Parameters->GetFloatParameter(PlayRateParameter, EffectivePlayRate))
		{
			//Play rate not found - Use default instead
			EffectivePlayRate = PlayRate;
		}			
			
		const float MontageLength = AnimInstancePtr->Montage_Play(Montage, EffectivePlayRate, EMontagePlayReturnType::MontageLength, StartingPosition, bShouldStopAllMontages);
		if (MontageLength > 0.f)
		{
			if (StartingSection != NAME_None)
			{
				AnimInstancePtr->Montage_JumpToSection(StartingSection, Montage);
			}
			
			//Bind end delegates
			FOnMontageEnded OnMontageEndedDelegate;
			OnMontageEndedDelegate.BindUObject(this, &UCFPlayMontageOnOwner::OnMontageEnded);
			AnimInstancePtr->Montage_SetEndDelegate(OnMontageEndedDelegate, Montage);
			
			//Setup the 'IsPlaying' parameter if required
			if (Context.Parameters && IsPlayingParameter.IsValid())
			{
				Context.Parameters->SetFloatParameter(IsPlayingParameter, 1.0f);
			}
			
			//Montage correct, cache the context so when the montage ends the effector does as well
			bMontagePlayedSuccesfully = true;
			CachedContext = Context;
		}
	}
	
	//If we haven't played the montage, stop the effector right away
	if (!bMontagePlayedSuccesfully)
	{
		StopEffector(Context);
	}
}

void UCFPlayMontageOnOwner::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	if (Montage && AnimInstancePtr.IsValid())
	{
		if (bStopMontageOnEffectorEnd && AnimInstancePtr->Montage_IsPlaying(Montage))
		{
			AnimInstancePtr->Montage_Stop(InterruptBlendOutTime, Montage);	
		}
		
		//Setup the 'IsPlaying' parameter if required
		if (Context.Parameters && IsPlayingParameter.IsValid())
		{
			Context.Parameters->SetFloatParameter(IsPlayingParameter, 0.0f);
		}
	}
}

void UCFPlayMontageOnOwner::OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted)
{
	StopEffector(CachedContext);
}
