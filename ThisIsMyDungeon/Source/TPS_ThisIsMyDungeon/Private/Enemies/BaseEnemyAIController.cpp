// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemyAIController.h"

#include "Enemies/BaseEnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"

ABaseEnemyAIController::ABaseEnemyAIController(const FObjectInitializer& ObjectInitializer)
{
}

void ABaseEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(AEnemyCharacter* const Enemy= Cast<AEnemyCharacter>(InPawn))
	{
		if(UBehaviorTree* const tree = Enemy->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
			//GEngine->AddOnScreenDebugMessage(0001, 2.f, FColor::Orange, TEXT("OnPosess Suceeded"));
		}
	}
}
