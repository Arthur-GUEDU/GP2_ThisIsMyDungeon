// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_StartHealing.generated.h"

/**
 * 
 */
UCLASS()
class TPS_THISISMYDUNGEON_API UBTService_StartHealing : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	explicit UBTService_StartHealing(FObjectInitializer const& ObjectInitializer);
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
