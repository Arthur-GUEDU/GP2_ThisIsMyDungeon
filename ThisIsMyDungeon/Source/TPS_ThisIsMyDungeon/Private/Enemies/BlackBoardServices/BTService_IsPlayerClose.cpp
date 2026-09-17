// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardServices/BTService_IsPlayerClose.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTService_IsPlayerClose::UBTService_IsPlayerClose(FObjectInitializer const& ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player Close");
}

void UBTService_IsPlayerClose::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Controller and Enemy
	auto const * const cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	auto const * const enemy = Cast<AEnemyCharacter>(cont->GetPawn());

	//Get Player Character
	auto const * const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), FVector::Distance(enemy->GetActorLocation(),player->GetActorLocation())<300.f);
}

void UBTService_IsPlayerClose::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Get Controller and Enemy
	AAIController * cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter * enemy = Cast<AEnemyCharacter>(cont->GetPawn());
	
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (cont && enemy && player)
	{
		bool bPlayerClose = FVector::Distance(enemy->GetActorLocation(),player->GetActorLocation())<300.f;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bPlayerClose);
		//UE_LOG(LogTemp, Warning, TEXT("Service ticking: Player is %s"), bPlayerClose ? TEXT("Close") : TEXT("Far"));
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("Service ticking: Controller or Enemy is null!"));
	}
}
