// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFApplyVelocityToPawn.h"
#include "Combat/CFSystemParameterCollection.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

UCFApplyVelocityToPawn::UCFApplyVelocityToPawn()
	: bUseSystemParameterVelocity(false)
	, bLocalSpace(true)
	, bTreatAsImpulse(false)
{}

void UCFApplyVelocityToPawn::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	//Can only work on pawns
	APawn* Pawn = Cast<APawn>(Context.Owner);
	if (Pawn && Pawn->GetMovementComponent())
	{		
		//Adjust for local movement, Velocity is always on World Space
		FVector AppliedVelocity = FVector::ZeroVector;
		if (bUseSystemParameterVelocity && Context.Parameters)
		{
			Context.Parameters->GetVectorParameter(VelocityParameterTag, AppliedVelocity);
		}
		else
		{
			AppliedVelocity = Velocity;
		}

		//Transform to local space if needed
		if (bLocalSpace)
		{
			AppliedVelocity = Pawn->GetActorTransform().TransformVector(AppliedVelocity);
		}

		//Scale the velocity if we have the parameter for it
		float Scale = 1.0f;
		if (Context.Parameters && Context.Parameters->GetFloatParameter(ScaleParameterTag, Scale))
		{
			AppliedVelocity *= Scale;
		}

		//Patch Z velocity if we're not overriding it
		UPawnMovementComponent* PawnMovement = Pawn->GetMovementComponent();
		if (bIgnoreZAxis)
		{
			AppliedVelocity.Z = PawnMovement->Velocity.Z;
		}

		//Apply the resulting velocity
		PawnMovement->Velocity = AppliedVelocity;

		//Special case when using the character movement, if we have a non zero Z velocity we might want to make sure the character is set to falling
		UCharacterMovementComponent* CharacterMovementComp = Cast<UCharacterMovementComponent>(PawnMovement);
		if ((AppliedVelocity.Z != 0 && PawnMovement && CharacterMovementComp->MovementMode == EMovementMode::MOVE_Walking) || bForceFalling)
		{
			CharacterMovementComp->SetMovementMode(MOVE_Falling);
		}

		//Check whether this is meant to be a one shot, if so... force finish this effector
		if (bTreatAsImpulse)
		{
			StopEffector(Context);
		}
	}
}
