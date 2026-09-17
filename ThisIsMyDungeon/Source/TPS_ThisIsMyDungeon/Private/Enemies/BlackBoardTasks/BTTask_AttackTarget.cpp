// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardTasks/BTTask_AttackTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyCharacter.h"
#include "Environment/ObjectiveActor.h"
#include "Kismet/GameplayStatics.h"

UBTTask_AttackTarget::UBTTask_AttackTarget(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Attack Target");
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(cont->GetPawn());
	if(Cast<AObjectiveActor>(enemy->Target))
	{
		enemy->AcquireTreasure(enemy->GetTreasureDamage());

		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		BlackboardComp->SetValueAsBool(TEXT("IsInteracting"), true);
		
		//frankly i hardly understand whats going on here i just know this is how you set a bool from the anim bp in c++ (called "reflectance"?)
		USkeletalMeshComponent* SkeletalMeshComp = enemy->GetMesh();
		UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
		FProperty* Property = AnimInstance->GetClass()->FindPropertyByName(TEXT("IsInteracting"));
		FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);
		void* BoolAddress = Property->ContainerPtrToValuePtr<void>(AnimInstance);
		BoolProperty->SetPropertyValue(BoolAddress, true);
		UGameplayStatics::ApplyDamage(enemy->Target, enemy->GetTreasureDamage(), enemy->GetController(), enemy, enemy->DamageType);
		return EBTNodeResult::Succeeded;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(!BlackboardComp->GetValueAsBool("CanMove") || !BlackboardComp->GetValueAsBool("CanAttack"))
		return EBTNodeResult::Failed;
	
	BlackboardComp->SetValueAsBool(TEXT("CanMove"), false);
	BlackboardComp->SetValueAsBool(TEXT("CanAttack"), false);
	
	USkeletalMeshComponent* SkeletalMeshComp = enemy->GetMesh();
	UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
	FProperty* Property = AnimInstance->GetClass()->FindPropertyByName(TEXT("IsAttacking"));
	FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);
	void* BoolAddress = Property->ContainerPtrToValuePtr<void>(AnimInstance);
	BoolProperty->SetPropertyValue(BoolAddress, true);

	UGameplayStatics::ApplyDamage(enemy->Target, enemy->GetDamage(), enemy->GetController(), enemy, enemy->DamageType);
	return EBTNodeResult::Succeeded;
}
