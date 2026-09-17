// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardServices/BTService_IsObjectiveInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyCharacter.h"
#include "Enemies/EnemyManagerInstanceSubsystem.h"

UBTService_IsObjectiveInRange::UBTService_IsObjectiveInRange(FObjectInitializer const& ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Objective In Range");
}

void UBTService_IsObjectiveInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Controller and NPC
	AAIController*  Cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter * Enemy = Cast<AEnemyCharacter>(Cont->GetPawn());
	
	FVector EnemyLocation = Enemy->GetActorLocation();
	FVector ObjectiveLocation = Enemy->GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>()->GetLevelDef()->ObjectiveActor->GetActorLocation();
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), FVector::Distance(EnemyLocation,ObjectiveLocation)<300.f);
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_IsObjectiveInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Get Controller and NPC
	AAIController*  Cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter * Enemy = Cast<AEnemyCharacter>(Cont->GetPawn());
	
	FVector EnemyLocation = Enemy->GetActorLocation();
	FVector ObjectiveLocation = Enemy->GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>()->GetLevelDef()->ObjectiveActor->GetActorLocation();
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), FVector::Distance(EnemyLocation,ObjectiveLocation)<300.f);
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
