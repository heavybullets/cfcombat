// Copyright Critical Failure Studio Ltd.

#include "Combat/ConditionBlocks/CFCharacterHasMovementMode.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCFCharacterHasMovementMode::UCFCharacterHasMovementMode()
	: MovementMode(EMovementMode::MOVE_Walking)
{}

bool UCFCharacterHasMovementMode::ProcessCondition_Implementation(const FCFConditionBlockContext& Context) const
{
	if (ACharacter* Character = Cast<ACharacter>(Context.Owner))
	{
		const EMovementMode CharMoveMode = Character->GetCharacterMovement()->MovementMode;
		const bool bValidCustomMode = CharMoveMode == EMovementMode::MOVE_Custom ? Character->GetCharacterMovement()->CustomMovementMode == CustomMovementMode : true;
		return CharMoveMode == MovementMode && bValidCustomMode;
	}

	return false;
}
