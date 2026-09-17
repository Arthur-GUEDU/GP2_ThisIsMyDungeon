// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/GridComponent.h"

// Sets default values for this component's properties
UGridComponent::UGridComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UGridComponent::BeginPlay()
{
	Super::BeginPlay();	
}

FVector3f UGridComponent::GetGridLocation(FVector3f _Location)
{
	if (GridSize == 0.f)
		return _Location;

	float HalfSize = GridSize / 2.f;
	FVector3f TempVec = FVector3f((_Location.X - HalfSize - Offset.X) / GridSize, (_Location.Y - HalfSize - Offset.Y) / GridSize, _Location.Z);
	TempVec.X = roundf(TempVec.X) * GridSize + HalfSize;
	TempVec.Y = roundf(TempVec.Y) * GridSize + HalfSize;

	return TempVec + Offset;
}
