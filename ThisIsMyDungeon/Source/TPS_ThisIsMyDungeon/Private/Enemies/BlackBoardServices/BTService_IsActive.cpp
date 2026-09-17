// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/BlackBoardServices/BTService_IsActive.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyCharacter.h"

UBTService_IsActive::UBTService_IsActive(FObjectInitializer const& ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsActive::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Controller and NPC
	auto const * const cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	auto const * const enemy = Cast<AEnemyCharacter>(cont->GetPawn());
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), enemy->IsActive());
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}
