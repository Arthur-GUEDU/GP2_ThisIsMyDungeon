// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Timer.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnTimerEvent);

/**
 *
 */
UCLASS(Blueprintable)
class TPS_THISISMYDUNGEON_API UTimer : public UObject
{
    GENERATED_BODY()
public:

    /// <summary>
    /// Timer constructor
    /// </summary>
    UTimer() = default;

    /// <summary>
    /// Starts the timer
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void StartTimer();

    /// <summary>
    /// Setup the timer and its variables
    /// </summary>
    /// <param name="_Countdown">: Time the timer will start to decrement from </param>
    /// <param name="_TimeStamp">: Time of update of the timer </param>
    UFUNCTION(BlueprintCallable)
    void SetupTimer(float _CountdownTime, float _TimeStamp);

    /// <summary>
    /// Setup the timer and its variables
    /// </summary>
    /// <param name="_Countdown">: Time the timer will start to decrement from </param>
    /// <param name="_TimeStamp">: Time of update of the timer </param>
    UFUNCTION(BlueprintCallable)
    void SetupTimerAndStart(float _CountdownTime, float _TimeStamp);

    /// <summary>
    /// Stops the timer
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void StopTimer();

	/// <summary>
	/// 
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ClearAndDestroyTimer();

    UFUNCTION(BlueprintCallable)
    FORCEINLINE bool IsFinished() { return bIsFinished; };

    UPROPERTY(BlueprintReadWrite, Category = "Timer events")
    FOnTimerEvent OnTimerStart;

    UPROPERTY(BlueprintReadWrite, Category = "Timer events")
    FOnTimerEvent OnTimerUpdate;

    UPROPERTY(BlueprintReadWrite, Category = "Timer events")
    FOnTimerEvent OnTimerEnd;

    UPROPERTY(BlueprintReadWrite, Category = "Timer events")
    FOnTimerEvent OnTimerStopped;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer parameters")
    float CountdownTime = 0.f;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer parameters")
    float TimeStamp = 0.01f;


private:
    FTimerHandle m_CountdownTimerHandle;
        
	bool bIsFinished = true;

	/// <summary>
	/// Timer update method
	/// </summary>
	void UpdateTimer();

	/// <summary>
	/// Method called when the timer is finished and triggering a delegate
	/// </summary>
	void TimerFinished();
};
