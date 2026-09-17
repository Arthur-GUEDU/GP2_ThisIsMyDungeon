// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardTasks/BTTask_FindPathToObjective.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyManagerInstanceSubsystem.h"

UBTTask_FindPathToObjective::UBTTask_FindPathToObjective(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Path To Objective");
}

EBTNodeResult::Type UBTTask_FindPathToObjective::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter*  Enemy = Cast<AEnemyCharacter>(Cont->GetPawn());
	if(AObjectiveActor* Objective = Enemy->GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>()->GetLevelDef()->ObjectiveActor)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Objective->GetActorLocation());
		Enemy->Target = Objective;
		return EBTNodeResult::Succeeded;
	}
	UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPathToObjective: Failed to find path!"));
	return EBTNodeResult::Failed;
}
