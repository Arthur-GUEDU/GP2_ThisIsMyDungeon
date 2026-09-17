#include "Traps/SpikeTrap.h"
#include "Enemies/BaseEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "KismetTraceUtils.h"

void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	SkeletalMesh = GetComponentByClass<USkeletalMeshComponent>();
}

void ASpikeTrap::Attack()
{
	if (SkeletalMesh != nullptr)
	{
		SkeletalMesh->PlayAnimation(AttackAnimation, false);
	}

	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(100, 100, 100));
	TArray<FHitResult> SweepResults;
	TArray< uint32> ResultsIDs;
	bool hit = GetWorld()->SweepMultiByChannel(SweepResults, GetActorLocation() + FVector(0, 0, 100), GetActorLocation() + FVector(0, 0, 100), FQuat::Identity, ECC_GameTraceChannel2, Shape);
	
	if (!hit)
		return;

	for (auto& Hit : SweepResults)
	{
		if (Hit.bBlockingHit)
		{
			int i = 0;
			uint32 ID = Hit.GetActor()->GetUniqueID();

			while (i < ResultsIDs.Num() && ID != ResultsIDs[i])
				++i;

			if (i == ResultsIDs.Num())
			{
				ResultsIDs.Add(ID);

				if (Cast<ABaseEnemyCharacter>(Hit.GetActor()))
				{
					TSubclassOf<UDamageType> DamagesType;
					UGameplayStatics::ApplyDamage(Hit.GetActor(), Damages, nullptr, this, DamagesType);
				}
			}
		}
	}
}
