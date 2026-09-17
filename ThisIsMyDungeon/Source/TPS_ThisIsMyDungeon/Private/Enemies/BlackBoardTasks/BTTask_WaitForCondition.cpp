// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BlackBoardTasks/BTTask_WaitForCondition.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_WaitForCondition::UBTTask_WaitForCondition()
{
	BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_WaitForCondition, BlackboardKey));
}

EBTNodeResult::Type UBTTask_WaitForCondition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Begin ticking to monitor the condition
	return EBTNodeResult::InProgress;
}

void UBTTask_WaitForCondition::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (IsConditionMet(OwnerComp))
	{
		// Finish task successfully when condition is met
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

bool UBTTask_WaitForCondition::IsConditionMet(UBehaviorTreeComponent& OwnerComp) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey()) == ShouldKeyBeSet;
}
