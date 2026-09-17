// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Environment/ObjectiveActor.h"
#include "Actor_Drop.generated.h"

UCLASS()
class TPS_THISISMYDUNGEON_API AActor_Drop : public AActor
{
	GENERATED_BODY()
	
public:
	AActor_Drop();

	bool GetActive();
	UFUNCTION(BlueprintCallable)
	void Activate(FVector Location, int treasureAmount);
	UFUNCTION(BlueprintCallable)
	void Deactivate();
	
	UPROPERTY(BlueprintReadOnly)
	AObjectiveActor* Objective;
protected:
	virtual void BeginPlay() override;
private:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(AllowPrivateAccess))
	int32 TreasureAmount;

	bool bIsActive = false;

	FTimerHandle despawnTimer;
};
