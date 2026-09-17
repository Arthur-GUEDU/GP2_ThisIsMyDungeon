// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/MageEnemyCharacter.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemies/EnemyManagerInstanceSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"

FVector AMageEnemyCharacter::GetHealBeamOrigin()
{
	if(HealBeamOrigin)
		return HealBeamOrigin->GetComponentLocation();
	return GetActorLocation();
}

void AMageEnemyCharacter::HealAllies()
{
	float healAmount = Damage;
	TArray<AEnemyCharacter*> enemiesInRange = GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>()->GetEnemiesInRange(GetActorLocation(), HealRange);

	if(enemiesInRange.Num() == 0)
	{
		return;
	}
	
	int healCount = 0;
	for(AEnemyCharacter* enemyToHeal : enemiesInRange)
	{
		int rest = enemyToHeal->Heal(healAmount);
		if(rest != healAmount)
		{
			UNiagaraComponent* HealBeam = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				HealBeamEffect,    
				GetActorLocation(),
				FRotator::ZeroRotator
			);
			HealBeam->SetVectorParameter(TEXT("BeamStart"), GetHealBeamOrigin());
			HealBeam->SetVectorParameter(TEXT("BeamEnd"), enemyToHeal->GetActorLocation());
			
			healAmount = rest;
			healCount++;
		}
		if(healAmount<=0)
		{
			break;
		}
	}
}

void AMageEnemyCharacter::BeginPlay()
{
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	for (UActorComponent* Component : GetComponents())
	{
		if (Component && Component->GetFName() == TEXT("RayCastPoint"))
		{
			HealBeamOrigin = Cast<USceneComponent>(Component);
			break;
		}
	}
	if(!HealBeamOrigin)
		UE_LOG(LogTemp, Warning, TEXT("Mage Enemy Character: No heal beam origin found"))
	Super::BeginPlay();
}
