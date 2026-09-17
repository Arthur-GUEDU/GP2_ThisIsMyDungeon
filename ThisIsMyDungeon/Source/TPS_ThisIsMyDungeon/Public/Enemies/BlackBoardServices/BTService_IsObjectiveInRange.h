// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsObjectiveInRange.generated.h"

/**
 * 
 */
UCLASS()
class TPS_THISISMYDUNGEON_API UBTService_IsObjectiveInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	explicit UBTService_IsObjectiveInRange(FObjectInitializer const& ObjectInitializer);
public:
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
