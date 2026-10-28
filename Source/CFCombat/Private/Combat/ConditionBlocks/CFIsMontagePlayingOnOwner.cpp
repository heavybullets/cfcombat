// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFIsMontagePlayingOnOwner.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

void UCFIsMontagePlayingOnOwner::OnConditionInit_Implementation(const FCFConditionBlockContext& Context)
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

bool UCFIsMontagePlayingOnOwner::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	return AnimInstancePtr.IsValid() && AnimInstancePtr->Montage_IsPlaying(Montage);
}