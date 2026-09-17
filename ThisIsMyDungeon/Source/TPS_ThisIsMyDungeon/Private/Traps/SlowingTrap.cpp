#include "Traps/SlowingTrap.h"
#include "Enemies/EnemyCharacter.h"

ASlowingTrap::ASlowingTrap()
{
}

void ASlowingTrap::BeginPlay(void)
{
	Super::BeginPlay();
	TriggerZone = GetComponentByClass<UBoxComponent>();
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ASlowingTrap::BeginOverlap);
	TriggerZone->OnComponentEndOverlap.AddDynamic(this, &ASlowingTrap::EndOverlap);
}

void ASlowingTrap::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
	if (Enemy != nullptr)
	{
		Enemy->SetMoveSpeedMult(SlowingRate);
	}
}

void ASlowingTrap::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
	if (Enemy != nullptr)
	{
		Enemy->ResetMoveSpeed();
	}
}
