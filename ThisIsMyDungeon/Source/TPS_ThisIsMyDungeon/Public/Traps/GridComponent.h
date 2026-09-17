// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_THISISMYDUNGEON_API UGridComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Set default values for this component's properties
	UGridComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float GridSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	FVector3f Offset;

public:
	/// <summary>
	/// Get the closest location on the grid to the given location
	/// Return the given location if GridSize is set to 0
	/// </summary>
	/// <param name="_Location">: location to approximate on the grid</param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	FVector3f GetGridLocation(FVector3f _Location);
};
