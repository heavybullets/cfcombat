// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "Combat/EffectorBlocks/CFEffectorBlock.h"
#include "Templates/SubclassOf.h"
#include "CFApplyAttributeEffector.generated.h"

//fwd
class UCFAttributeEffectorBase;

/**
 * Applies the given attribute effector to the owner for the duration of the effector.
 */
UCLASS()
class CFCOMBAT_API UCFApplyAttributeEffector : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* Class of the effector we wish to apply. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	TSubclassOf<UCFAttributeEffectorBase> EffectorClass;

	/* Whether the effector clears itself on execution end. */
	UPROPERTY(EditAnywhere, Category = "Effector")
	bool bClearOnEnd;

public:
	//ctor
	UCFApplyAttributeEffector();
	
	//~ Begin UCFEffectorBlock Interface
	virtual void OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context) override;
	virtual void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	//~ End UCFEffectorBlock Interface
};
