// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardTasks/BTTask_GetPlayerLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_GetPlayerLocation::UBTTask_GetPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Get Player Location");
}

EBTNodeResult::Type UBTTask_GetPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter*  Enemy = Cast<AEnemyCharacter>(Cont->GetPawn());
	//get player character
	if(auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		Enemy->Target=Player;
		// get player location to use as an origin
		FVector const playerLocation = Player->GetActorLocation();

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), playerLocation);
		return EBTNodeResult::Succeeded;

	}
	GEngine->AddOnScreenDebugMessage(0101,2.f, FColor::Orange, TEXT("FindPlayerLocation Failed")); 
	return EBTNodeResult::Failed;
}
