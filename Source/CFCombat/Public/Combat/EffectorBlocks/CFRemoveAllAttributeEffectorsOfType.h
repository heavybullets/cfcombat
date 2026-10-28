// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CFEffectorBlock.h"
#include "GameplayTagContainer.h"
#include "CFRemoveAllAttributeEffectorsOfType.generated.h"


/* Clears all effectors for a certain attribute */
UCLASS()
class CFCOMBAT_API UCFRemoveAllAttributeEffectorsOfType : public UCFEffectorBlock
{
	GENERATED_BODY()
	
	/* The attribute that all attribute effectors will be cleared */
	UPROPERTY(EditAnywhere, Category="Effector")
	FGameplayTag EffectorType;
	
	/* whether it should clear on effector start or on end */
	UPROPERTY(EditAnywhere, Category="Effector")
	bool bRemoveOnEnd = false;
	
	/* Max amount of attribute effectors it can remove, 0 is all of them. */
	UPROPERTY(EditAnywhere, Category="Effector")
	int32 RemoveLimit = 0;
	
public:
	/* start UCFEffectorBlock overrides */
	void OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context) override;
	void OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context) override;
	/* end UCFEffectorBlock overrides */
};
