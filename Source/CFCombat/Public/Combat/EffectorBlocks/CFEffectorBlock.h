// Copyright Critical Failure Studio Ltd.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CFEffectorBlock.generated.h"

//fwd
class AActor;
class UCFSystemParameterCollection;
class UCFIntentDriver;

/**
 * The context to be passed to the effector block.
 */
USTRUCT(BlueprintType)
struct FCFEffectorBlockContext
{
	GENERATED_BODY()

	/* The actor that the combat component is working with. */
	UPROPERTY(BlueprintReadOnly, Category = "Effector")
	TWeakObjectPtr<AActor> Owner;

	/* The time that the section currently has. */
	UPROPERTY(BlueprintReadOnly, Category = "Effector")
	float CurrentSectionTime;
	
	/* System parameters collections if they exist, can be NULL if not defined. */
	UPROPERTY(BlueprintReadOnly, Category = "Effector")
	UCFSystemParameterCollection* Parameters;

	/* Intent driver that's running on the system, can be null. */
	UPROPERTY(BlueprintReadOnly, Category = "Effector")
	UCFIntentDriver* IntentDriver;

public:
	//ctor
	FCFEffectorBlockContext(AActor* InOwner = nullptr, UCFSystemParameterCollection* InParameters = nullptr, float InCurrentSectionTime = 0.0f, UCFIntentDriver* InIntentDriver = nullptr)
		: Owner(InOwner)
		, CurrentSectionTime(InCurrentSectionTime)
		, Parameters(InParameters)
		, IntentDriver(InIntentDriver)
	{}
};

/**
 * An effect that can be executed by the combat system when inside an state.
 */
UCLASS(Blueprintable, EditInlineNew, abstract)
class CFCOMBAT_API UCFEffectorBlock : public UObject
{
	GENERATED_BODY()

	/* The time at which this effector starts executing. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effector", meta = (AllowPrivateAccess = "true"))
	float StartTime;

	/* Used for grabbing the context of the world when called. */
	TWeakObjectPtr<UWorld> CurrentWorldContext;

	/* Whether this effector has been stopped or not. */
	bool bStopped;

	/* If this effector is currently running. */
	bool bRunning;
	
	/* The accumulated time this effector has been running. */
	UPROPERTY(BlueprintReadOnly, Category = "Effector", meta = (AllowPrivateAccess = "true"))
	float CurrentRunningTime = 0.f;
	
protected:
	/* If > 0, how long this effector lasts before stopping. Protected so subclasses can give themselves a default in their constructor. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effector")
	float Duration;

public:
	//ctor
	UCFEffectorBlock();

	//Getters
	FORCEINLINE float GetStartTime() const { return StartTime; }
	FORCEINLINE float GetDuration() const { return Duration; }
	FORCEINLINE float GetCurrentRunningTime() const { return CurrentRunningTime; }

	/* Clears the effector of any flag and prepares for running. */
	void InitEffector(const FCFEffectorBlockContext& Context);

	/* Called externally to notify that the effector should stop execution. */
	UFUNCTION(BlueprintCallable, Category = "Effector")
	void StopEffector(const FCFEffectorBlockContext& Context);

	/* Called externally by the combat component to tick the effector on the given context. */
	void TickEffector(float DeltaTime, const FCFEffectorBlockContext& Context);

	/* Used to access world-related functions. */
	virtual UWorld* GetWorld() const override;

private:
	/* Starts the effector and flags it as running. */
	void StartEffector(const FCFEffectorBlockContext& Context);

protected:
	/* Called when the effector is initialized, clears every flag that needs to be cleared before running. */
	UFUNCTION(BlueprintNativeEvent, Category = "Effector")
	void OnEffectorInit(const FCFEffectorBlockContext& Context);

	/* Called when the effector starts running, once its start time is reached. */
	UFUNCTION(BlueprintNativeEvent, Category = "Effector")
	void OnEffectorStart(const FCFEffectorBlockContext& Context);

	/* Called when the effector is no longer valid and needs to deactivate. */
	UFUNCTION(BlueprintNativeEvent, Category = "Effector")
	void OnEffectorEnd(const FCFEffectorBlockContext& Context);

	/* Called to process a tick of the effector. */
	UFUNCTION(BlueprintNativeEvent, Category = "Effector")
	void OnEffectorTick(float DeltaTime, const FCFEffectorBlockContext& Context);
};
