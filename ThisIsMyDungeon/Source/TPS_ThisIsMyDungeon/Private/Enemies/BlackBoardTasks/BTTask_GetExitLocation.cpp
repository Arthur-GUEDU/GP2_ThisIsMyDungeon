// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardTasks/BTTask_GetExitLocation.h"

#include "AIController.h"
#include "VectorTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyCharacter.h"
#include "Enemies/EnemyManagerInstanceSubsystem.h"
#include "Environment/ObjectiveActor.h"

UBTTask_GetExitLocation::UBTTask_GetExitLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Get Exit Location");
}

EBTNodeResult::Type UBTTask_GetExitLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter*  Enemy = Cast<AEnemyCharacter>(Cont->GetPawn());
	FVector ExitLocation = Enemy->GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>()->GetLevelDef()->ExitLocation;
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ExitLocation);
	return EBTNodeResult::Succeeded;
}
