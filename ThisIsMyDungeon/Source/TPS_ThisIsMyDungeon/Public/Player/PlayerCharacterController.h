// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class TPS_THISISMYDUNGEON_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float MinViewPitch = -45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float MaxViewPitch = 20.f;
};
