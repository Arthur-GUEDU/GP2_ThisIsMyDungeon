// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardServices/BTService_IsPlayerInAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTService_IsPlayerInAttackRange::UBTService_IsPlayerInAttackRange(FObjectInitializer const& ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Attack Range");
}

void UBTService_IsPlayerInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Controller and Enemy
	AAIController* cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(cont->GetPawn());

	//Get Player Character
	auto const * const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), FVector::Distance(enemy->GetActorLocation(),player->GetActorLocation())<AttackRange);
}

void UBTService_IsPlayerInAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	//Get Controller and Enemy
	AAIController* cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(cont->GetPawn());

	//Get Player Character
	auto const * const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), FVector::Distance(enemy->GetActorLocation(),player->GetActorLocation())<AttackRange);
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
