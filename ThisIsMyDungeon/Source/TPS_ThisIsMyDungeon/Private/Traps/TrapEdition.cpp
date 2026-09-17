#include "Traps/TrapEdition.h"
#include "Traps/GridComponent.h"


// Sets default values
ATrapEdition::ATrapEdition()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATrapEdition::BeginPlay()
{
	Super::BeginPlay();
	Mesh = GetComponentByClass<UStaticMeshComponent>();
	Grid = GetComponentByClass<UGridComponent>();
	CollisionBox = GetComponentByClass<UBoxComponent>();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATrapEdition::BeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ATrapEdition::EndOverlap);
	SwitchTrapType(0);
	SwitchMaterial(0);
	Disable();
}

void ATrapEdition::EnablePlacement()
{
	if (!bCanPlace)
	{
		bCanPlace = true;
		if (bIsMoneyAvailable)
			SwitchMaterial(0);
	}
}

void ATrapEdition::DisablePlacement()
{
	if (bCanPlace)
	{
		bCanPlace = false;
		SwitchMaterial(1);
	}
}

void ATrapEdition::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	++CollisionNbr;
	DisablePlacement();
}

void ATrapEdition::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	--CollisionNbr;

	if(CollisionNbr == 0)
		EnablePlacement();
}

void ATrapEdition::SwitchTrapType(int _NewIndex)
{
	if (_NewIndex == TrapTypeIndex || _NewIndex >= TrapTypes.Num() || _NewIndex == -1)
		return;

	TrapTypeIndex = _NewIndex;
	FTrapTypeInfo Infos = TrapTypes[TrapTypeIndex];
	Mesh->SetStaticMesh(Infos.TrapMesh);
	Mesh->SetRelativeLocation(Infos.MeshOffset);
	Mesh->SetRelativeRotation(Infos.MeshRotation);
	Mesh->SetRelativeScale3D(Infos.MeshScale);
	SwitchMaterial(MaterialIndex);
}

void ATrapEdition::SwitchMaterial(int _NewIndex)
{
	if (_NewIndex >= Materials.Num() || _NewIndex == -1)
		return;

	MaterialIndex = _NewIndex;
	int meshNum = Mesh->GetMaterials().Num();
	for (int i = 0; i < meshNum; ++i)
		Mesh->SetMaterial(i, Materials[MaterialIndex]);
}

void ATrapEdition::UpdateLocation(FVector3f _NewLocation)
{
	FVector3f GridLocation = Grid->GetGridLocation(_NewLocation);
	FVector TransformLocation = GetActorLocation();
	if (GridLocation.X != TransformLocation.X || GridLocation.Y != TransformLocation.Y)
	{
		SetActorLocation(FVector(GridLocation.X, GridLocation.Y, GridLocation.Z));
	}
}

void ATrapEdition::Enable()
{
	SetActorHiddenInGame(false);
}

void ATrapEdition::Disable()
{
	SetActorHiddenInGame(true);
}

void ATrapEdition::SpawnTrap()
{
	TrapManager->Spawn(&TrapTypes[TrapTypeIndex], GetActorLocation(), TrapRotation);
}

void ATrapEdition::CheckTrapCost(int _Money)
{
	if (_Money < TrapTypes[TrapTypeIndex].TrapCost)
	{
		bIsMoneyAvailable = false;
		SwitchMaterial(1);
	}
	else
	{
		bIsMoneyAvailable = true;
		if (bCanPlace)
			SwitchMaterial(0);
	}
}

void ATrapEdition::RotateTrap()
{
	TrapRotation += FRotator(0, 90, 0);
	SetActorRotation(GetActorRotation() + FRotator(0, 90, 0));
}
