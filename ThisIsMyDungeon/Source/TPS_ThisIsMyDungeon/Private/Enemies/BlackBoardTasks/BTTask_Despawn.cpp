// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardTasks/BTTask_Despawn.h"

#include "AIController.h"
#include "Enemies/EnemyCharacter.h"


UBTTask_Despawn::UBTTask_Despawn(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Despawn");
}

EBTNodeResult::Type UBTTask_Despawn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Controller and Enemy
	AAIController * cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter * enemy = Cast<AEnemyCharacter>(cont->GetPawn());

	enemy->Deactivate();
	return EBTNodeResult::Succeeded;
}

