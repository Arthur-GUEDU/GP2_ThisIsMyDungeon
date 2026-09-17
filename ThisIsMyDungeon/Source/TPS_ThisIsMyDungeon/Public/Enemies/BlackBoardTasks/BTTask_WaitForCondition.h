// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_WaitForCondition.generated.h"

/**
 * 
 */
UCLASS()
class TPS_THISISMYDUNGEON_API UBTTask_WaitForCondition : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_WaitForCondition();

	UPROPERTY(EditAnywhere, Category = "Task Settings")
	bool ShouldKeyBeSet;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsConditionMet(UBehaviorTreeComponent& OwnerComp) const;
};
