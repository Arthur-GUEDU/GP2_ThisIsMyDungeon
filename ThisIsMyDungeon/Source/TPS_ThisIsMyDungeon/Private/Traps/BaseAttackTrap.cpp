#include "Traps/BaseAttackTrap.h"
#include "Timer/Timer.h"
#include "Enemies/BaseEnemyCharacter.h"

void ABaseAttackTrap::BeginPlay()
{
	Super::BeginPlay();
	TriggerZone = GetComponentByClass<UBoxComponent>();
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ABaseAttackTrap::BeginOverlap);
	TriggerZone->OnComponentEndOverlap.AddDynamic(this, &ABaseAttackTrap::EndOverlap);
	Timer = NewObject<UTimer>(this);
	Timer->OnTimerEnd.BindUFunction(this, FName("TryAttacking"));
	Timer->SetupTimer(Cooldown, Timestamp);
}

void ABaseAttackTrap::Attack()
{
	
}

void ABaseAttackTrap::TryAttacking()
{
	if (OverlapCount > 0 && Timer->IsFinished())
	{
		Timer->SetupTimerAndStart(Cooldown, Timestamp);
		Attack();
	}
}

void ABaseAttackTrap::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ABaseEnemyCharacter>(OtherActor))
		++OverlapCount;
	TryAttacking();
}

void ABaseAttackTrap::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ABaseEnemyCharacter>(OtherActor))
		--OverlapCount;
}
