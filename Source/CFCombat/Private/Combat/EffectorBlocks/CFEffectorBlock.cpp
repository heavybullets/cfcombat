// Copyright Critical Failure Studio Ltd.

#include "Combat/EffectorBlocks/CFEffectorBlock.h"

UCFEffectorBlock::UCFEffectorBlock()
{}

void UCFEffectorBlock::InitEffector(const FCFEffectorBlockContext& Context)
{
	bStopped = false;
	bRunning = false;
	CurrentWorldContext = Context.Owner->GetWorld();
	OnEffectorInit(Context);
}

void UCFEffectorBlock::StopEffector(const FCFEffectorBlockContext& Context)
{
	if (bRunning)
	{
		OnEffectorEnd(Context);
	}

	bStopped = true;
	bRunning = false;
}

void UCFEffectorBlock::TickEffector(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	//check if this effector should activate
	const bool bEffectorActive = !bStopped && Context.CurrentSectionTime >= StartTime;
	if (bEffectorActive)
	{
		const bool bEffectorDurationExpired = Duration > 0 ? Context.CurrentSectionTime > (StartTime + Duration) : false;
		const bool bEffectorStartedThisFrame = !bRunning;
		if (!bEffectorDurationExpired)
		{
			if (bEffectorStartedThisFrame)
			{
				StartEffector(Context);
			}

			//Delta time could actually be zero, in which case this tick was triggered only to simulate activation for effectors that should be enabled right away.
			//Ignore zeroed delta times for the purposes of the effectors
			if (DeltaTime > 0.0f)
			{
				OnEffectorTick(DeltaTime, Context);
			}
		}
		else
		{
			//Check if the end should have been called now
			const float PrevFrameTime = Context.CurrentSectionTime - DeltaTime;
			if (Duration > 0 && PrevFrameTime < (StartTime + Duration))
			{
				//It could happen that the delta time from the tick was smaller than the effector duration and the effector would have been ignored
				//Treat this edge case
				if (bEffectorStartedThisFrame)
				{
					StartEffector(Context);

					//Delta time could actually be zero, in which case this tick was triggered only to simulate activation for effectors that should be enabled right away.
					//Ignore zeroed delta times for the purposes of the effectors
					if (DeltaTime > 0.0f)
					{
						OnEffectorTick(DeltaTime, Context);
					}
				}

				//Finally fully close the effector
				StopEffector(Context);
			}
		}
	}
}

UWorld* UCFEffectorBlock::GetWorld() const
{
	return CurrentWorldContext.Get();
}

void UCFEffectorBlock::StartEffector(const FCFEffectorBlockContext& Context)
{
	bRunning = true;
	OnEffectorStart(Context);
}

void UCFEffectorBlock::OnEffectorInit_Implementation(const FCFEffectorBlockContext& Context)
{
	//Empty implementation, can be overriden by child classes
}

void UCFEffectorBlock::OnEffectorStart_Implementation(const FCFEffectorBlockContext& Context)
{
	//Empty implementation, can be overriden by child classes
}

void UCFEffectorBlock::OnEffectorEnd_Implementation(const FCFEffectorBlockContext& Context)
{
	//Empty implementation, can be overriden by child classes
}

void UCFEffectorBlock::OnEffectorTick_Implementation(float DeltaTime, const FCFEffectorBlockContext& Context)
{
	//Empty implementation, can be overriden by child classes
}
