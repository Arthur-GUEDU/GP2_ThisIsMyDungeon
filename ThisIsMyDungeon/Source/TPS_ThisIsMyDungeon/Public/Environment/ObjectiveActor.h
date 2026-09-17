// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveActor.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveEvent);

UCLASS()
class TPS_THISISMYDUNGEON_API AObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveActor();
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="DamageAmount"></param>
	/// <param name="DamageEvent"></param>
	/// <param name="EventInstigator"></param>
	/// <param name="DamageCauser"></param>
	/// <returns></returns>
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void AddTreasure(int amount);
	/// <summary>
	/// 
	/// </summary>
	void ObjectiveDeath();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Objective parameters")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective parameters")
	float MaxHealth = 5000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective parameters")
	float Health = 5000.f;

	UPROPERTY(BlueprintReadWrite, Category = "Objective parameters")
	bool bIsDead = false;

	UPROPERTY(BlueprintAssignable, Category = "Objective events")
	FObjectiveEvent OnObjectiveHit;

	UPROPERTY(BlueprintAssignable, Category = "Objective events")
	FObjectiveEvent OnObjectiveDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta=(AllowPrivateAccess))
	USoundBase* TreasureTakenCue;
	
	/// <summary>
	/// 
	/// </summary>
	virtual void BeginPlay() override;
};
