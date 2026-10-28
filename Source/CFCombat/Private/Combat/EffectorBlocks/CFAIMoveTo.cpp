// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFAIMoveTo.h"
#include "Combat/CFSystemParameterCollection.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

UCFAIMoveTo::UCFAIMoveTo()
	: AcceptanceRadius(-1.0f)
	, bAllowPartialPaths(true)
	, bStopOnOverlap(true)
	, bCanStrafe(false)
{}

void UCFAIMoveTo::OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context)
{
	APawn* PawnOwner = Cast<APawn>(Context.Owner);
	if (PawnOwner && Context.Parameters && (TargetActorParameterTag.IsValid() || TargetLocationParameterTag.IsValid()))
	{
		AAIController* AIController = Cast<AAIController>(PawnOwner->GetController());
		if (AIController)
		{
			//Setup the goal
			FAIMoveRequest MoveReq;
			bool bValidMovement = false;
			AActor * TargetActor = Cast<AActor>(Context.Parameters->GetObjectParameter(TargetActorParameterTag));
			if (TargetActor)
			{
				MoveReq.SetGoalActor(TargetActor);
				bValidMovement = true;
			}
			else 
			{
				FVector TargetLocation;
				if (Context.Parameters->GetVectorParameter(TargetLocationParameterTag, TargetLocation))
				{
					MoveReq.SetGoalLocation(TargetLocation);
					bValidMovement = true;
				}
			}

			//Apply movement if we can
			if (bValidMovement)
			{
				MoveReq.SetUsePathfinding(true);
				MoveReq.SetAllowPartialPath(bAllowPartialPaths);
				MoveReq.SetAcceptanceRadius(AcceptanceRadius);
				MoveReq.SetReachTestIncludesAgentRadius(bStopOnOverlap);
				MoveReq.SetCanStrafe(bCanStrafe);
				MoveReq.SetNavigationFilter(FilterClass);
				AIController->MoveTo(MoveReq);
			}
			else
			{
				//Effector failed due to parameters not being passed correctly, stop it
				StopEffector(Context);
			}
		}
	}
}

void UCFAIMoveTo::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	APawn* PawnOwner = Cast<APawn>(Context.Owner);
	if (PawnOwner)
	{
		AAIController* AIController = Cast<AAIController>(PawnOwner->GetController());
		if (AIController)
		{
			AIController->StopMovement();
		}
	}
}
