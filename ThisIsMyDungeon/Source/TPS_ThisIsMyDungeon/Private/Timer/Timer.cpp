#include "Timer/Timer.h"

#include "Engine/World.h"
#include "TimerManager.h"

void UTimer::StartTimer()
{
    GetWorld()->GetTimerManager().SetTimer(m_CountdownTimerHandle, this, &UTimer::UpdateTimer, TimeStamp, true);
    OnTimerStart.ExecuteIfBound();

    bIsFinished = false;
}

void UTimer::SetupTimer(float _CountdownTime, float _TimeStamp)
{
    CountdownTime = _CountdownTime;
    TimeStamp = _TimeStamp;
}

void UTimer::SetupTimerAndStart(float _CountdownTime, float _TimeStamp)
{
    SetupTimer(_CountdownTime, _TimeStamp);
    StartTimer();
}

void UTimer::StopTimer()
{
    bIsFinished = true;
    CountdownTime = 0.f;

    OnTimerStopped.ExecuteIfBound();
}

void UTimer::UpdateTimer()
{
    if (bIsFinished)
        return;

    CountdownTime -= TimeStamp;

    OnTimerUpdate.ExecuteIfBound();

    if (CountdownTime <= 0.f)
    {
        bIsFinished = true;
        TimerFinished();
    }
}

void UTimer::TimerFinished()
{
    OnTimerEnd.ExecuteIfBound();
}

void UTimer::ClearAndDestroyTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(m_CountdownTimerHandle);
    bIsFinished = false;
}