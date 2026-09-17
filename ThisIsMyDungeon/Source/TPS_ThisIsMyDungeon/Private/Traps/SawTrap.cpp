#include "Traps/SawTrap.h"
#include "Enemies/BaseEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

ASawTrap::ASawTrap()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASawTrap::BeginPlay()
{
	Super::BeginPlay();
	Mesh = GetComponentByClass<UStaticMeshComponent>();
	Mesh->SetMobility(EComponentMobility::Movable);

	DamageZone = GetComponentByClass<UBoxComponent>();
	DamageZone->OnComponentBeginOverlap.AddDynamic(this, &ASawTrap::BeginOverlap);
	
	TArray<UActorComponent*> Components;
	GetComponents(Components);

	for (int i = 0; i < Components.Num(); ++i)
	{
		USceneComponent* TempComp = Cast<USceneComponent>(Components[i]);
		if (TempComp && TempComp->GetName() == SceneComponentName)
			SceneComp = TempComp;
	}
}

void ASawTrap::Tick(float DeltaTime)
{
	if (!bIsActive)
		return;

	FRotator Rotation = Mesh->GetRelativeRotation();
	Mesh->SetRelativeRotation(Rotation + FRotator(0, RotationSpeed * DeltaTime, 0));

	FVector NewLocation;
	if (bIsMovingForward)
	{
		NewLocation = SceneComp->GetRelativeLocation() + FVector(0, MovementSpeed * DeltaTime, 0);
		if (NewLocation.Y > MotionRange)
			bIsMovingForward = false;
	}
	else
	{
		NewLocation = SceneComp->GetRelativeLocation() + FVector(0, -MovementSpeed * DeltaTime, 0);
		if (NewLocation.Y < -MotionRange)
			bIsMovingForward = true;
	}
	SceneComp->SetRelativeLocation(NewLocation);
}

void ASawTrap::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
	if (Enemy != nullptr)
	{
		TSubclassOf<UDamageType> DamagesType;
		UGameplayStatics::ApplyDamage(Enemy, Damages, nullptr, this, DamagesType);
	}
}
