#pragma once

#include "CoreMinimal.h"
#include "Traps/BaseAttackTrap.h"
#include "SpikeTrap.generated.h"

/**
 * 
 */
UCLASS()
class TPS_THISISMYDUNGEON_API ASpikeTrap : public ABaseAttackTrap
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;

	/// <summary>
	/// Attack all Enemies in a Box Cast
	/// </summary>
	void Attack() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimationAsset* AttackAnimation;

private:
	USkeletalMeshComponent* SkeletalMesh;
};
