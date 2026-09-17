// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardServices/BTService_StartHealing.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTService_StartHealing::UBTService_StartHealing(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Start Healing");
}

void UBTService_StartHealing::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(BlackboardComp->GetValueAsBool("CanMove") && BlackboardComp->GetValueAsBool("CanAttack"))
	{
		//we simply want to set a HealEnemy blackboard node to true so that we can make the mage heal allies at a random interval
		BlackboardComp->SetValueAsBool(GetSelectedBlackboardKey(), true);
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
