// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardServices/BTService_CanMove.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyCharacter.h"

UBTService_CanMove::UBTService_CanMove(FObjectInitializer const& ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Can Move");
}

void UBTService_CanMove::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Controller and NPC
	auto const * const cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	auto const * const enemy = Cast<AEnemyCharacter>(cont->GetPawn());
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), enemy->CanMove());
}
