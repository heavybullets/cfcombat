// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFApplyMovementToPawn.h"
#include "Combat/CFSystemParameterCollection.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

UCFApplyMovementCurveToPawn::UCFApplyMovementCurveToPawn()
	: MovementScale(1.0f)
	, bLocalSpace(true)
	, bScaleTimeToDuration(false)
	, bIgnoreZAxis(true)
	, bResetVelocityOnEnd(true)
{}

void UCFApplyMovementCurveToPawn::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	if (MovementCurve)
	{
		//Can only work on pawns
		APawn* Pawn = Cast<APawn>(Context.Owner);
		if (Pawn && Pawn->GetMovementComponent())
		{
			float EffectiveMovementScale = MovementScale;
			if (ScaleParameter.IsValid() && Context.Parameters)
			{
				float ParameterValue;
				if (Context.Parameters->GetFloatParameter(ScaleParameter, ParameterValue))
				{
					EffectiveMovementScale *= ParameterValue;
				}
			}

			//Obtain the movement component
			UPawnMovementComponent* PawnMovement = Pawn->GetMovementComponent();
			const float DurationScale = bScaleTimeToDuration && GetDuration() > 0.0f ? 1.0f / GetDuration() : 1.0f;

			const float CurrentTimeNormalized = (Context.CurrentSectionTime - GetStartTime()) * DurationScale;
			const float PrevTimeNormalized = (Context.CurrentSectionTime - GetStartTime() - DeltaTime) * DurationScale;
			const FVector CurrentOffset = MovementCurve->GetVectorValue(CurrentTimeNormalized) * EffectiveMovementScale;
			const FVector PrevOffset = MovementCurve->GetVectorValue(PrevTimeNormalized) * EffectiveMovementScale;
			FVector Velocity = (CurrentOffset - PrevOffset) / DeltaTime;

			//Adjust for local movement, Velocity is always on World Space
			if (bLocalSpace)
			{
				Velocity = Pawn->GetActorTransform().TransformVector(Velocity);
			}

			//Check whether we should use or not the Z component
			if (bIgnoreZAxis)
			{
				Velocity.Z = PawnMovement->Velocity.Z;
			}

			PawnMovement->Velocity = Velocity;

			//Special case when using the character movement, if we have a non zero Z velocity we might want to make sure the character is set to falling
			UCharacterMovementComponent* CharacterMovementComp = Cast<UCharacterMovementComponent>(PawnMovement);
			if (Velocity.Z != 0 && PawnMovement && CharacterMovementComp->MovementMode == EMovementMode::MOVE_Walking)
			{
				CharacterMovementComp->SetMovementMode(MOVE_Falling);
			}
		}
	}
}

void UCFApplyMovementCurveToPawn::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	if (MovementCurve && bResetVelocityOnEnd)
	{
		//Can only work on pawns
		APawn* Pawn = Cast<APawn>(Context.Owner);
		if (Pawn && Pawn->GetMovementComponent())
		{
			UPawnMovementComponent* PawnMovement = Pawn->GetMovementComponent();
			PawnMovement->Velocity = FVector::ZeroVector;
		}
	}
}
