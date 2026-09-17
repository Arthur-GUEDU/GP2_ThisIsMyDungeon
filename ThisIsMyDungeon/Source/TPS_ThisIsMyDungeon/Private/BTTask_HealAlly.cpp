// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_HealAlly.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyCharacter.h"
#include "Enemies/EnemyManagerInstanceSubsystem.h"
#include "Enemies/MageEnemyCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"

UBTTask_HealAlly::UBTTask_HealAlly(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Heal Allies");
}

EBTNodeResult::Type UBTTask_HealAlly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Cont = Cast<AAIController>(OwnerComp.GetAIOwner());
	AMageEnemyCharacter*  Enemy = Cast<AMageEnemyCharacter>(Cont->GetPawn());
	if(!Enemy) //this will only work on the mage character as we need acess to the niagara particle system
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to do task HealAlly non non-mage class enemy"));
		return EBTNodeResult::Failed;
	}
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	//if theres no enemies to heal theres no point in playing the animation as we wont be healing
	TArray<AEnemyCharacter*> enemiesInRange = Enemy->GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>()->GetEnemiesInRange(Enemy->GetActorLocation(), Enemy->GetHealRange());
	if(enemiesInRange.Num() == 0)
	{
		BlackboardComp->SetValueAsBool(TEXT("IsPlayerInAttackRange"), false); //IsPlayerInAttackRange is just what i use to signify if the enemy can heal, so i can reuse the same blackboard as the other enemies
		return EBTNodeResult::Failed;
	}
	
	//healing is now done in the animation to be properly synced with the anim
	//Enemy->HealAllies();
	
	//we still set CanMove and IsAttacking in the blackboard and animation BP because we still want to lock the enemy in the animation and the heal replaces the attack 
	BlackboardComp->SetValueAsBool(TEXT("CanMove"), false);
	
	USkeletalMeshComponent* SkeletalMeshComp = Enemy->GetMesh();
	UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
	FProperty* Property = AnimInstance->GetClass()->FindPropertyByName(TEXT("IsAttacking"));
	FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);
	void* BoolAddress = Property->ContainerPtrToValuePtr<void>(AnimInstance);
	BoolProperty->SetPropertyValue(BoolAddress, true);
	
	return EBTNodeResult::Succeeded; // even if no enemies are healed, the task is still considered a "sucess"
}
