// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyCharacter.h"
#include "NiagaraSystem.h"
#include "MageEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TPS_THISISMYDUNGEON_API AMageEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	USceneComponent* HealBeamOrigin;
	
public:
	/**
	 * Returns the heal beam location, and just the actors location if the heal beam is not defined
	 */
	FVector GetHealBeamOrigin();
	
	float GetHealRange() {return HealRange;}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> HealBeamEffect;

	UFUNCTION(BlueprintCallable)
	void HealAllies();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float HealRange{200};
};
