// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsExitInRange.generated.h"

/**
 * 
 */
UCLASS()
class TPS_THISISMYDUNGEON_API UBTService_IsExitInRange : public UBTService_BlackboardBase
{
	explicit UBTService_IsExitInRange(FObjectInitializer const& ObjectInitializer);
	GENERATED_BODY()
public:
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
