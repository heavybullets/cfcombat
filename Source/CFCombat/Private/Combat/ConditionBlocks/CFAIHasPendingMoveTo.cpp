// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFAIHasPendingMoveTo.h"
#include "GameFramework/Pawn.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"

bool UCFAIHasPendingMoveTo::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	APawn* PawnOwner = Cast<APawn>(Context.Owner);
	if (PawnOwner)
	{
		AAIController* Controller = Cast<AAIController>(PawnOwner->Controller);
		if (Controller && Controller->GetPathFollowingComponent())
		{
			return Controller->GetPathFollowingComponent()->GetStatus() != EPathFollowingStatus::Idle;
		}
	}

	return false;
}
