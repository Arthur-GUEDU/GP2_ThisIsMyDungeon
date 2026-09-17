// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardServices/BTService_IsExitInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyCharacter.h"
#include "Enemies/EnemyManagerInstanceSubsystem.h"

UBTService_IsExitInRange::UBTService_IsExitInRange(FObjectInitializer const& ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Exit In Range");
}

void UBTService_IsExitInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter*  Enemy = Cast<AEnemyCharacter>(Cont->GetPawn());
	FVector ExitLocation = Enemy->GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>()->GetLevelDef()->ExitLocation;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), FVector::Dist(Enemy->GetActorLocation(), ExitLocation)<200.f);
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}
