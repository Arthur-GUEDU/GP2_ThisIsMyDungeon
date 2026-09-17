#pragma once

#include "CoreMinimal.h"
#include "Traps/TrapManagerInstanceSubsystem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TrapEdition.generated.h"

UCLASS()
class TPS_THISISMYDUNGEON_API ATrapEdition : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapEdition();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Preview")
	int TrapTypeIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Preview")
	int MaterialIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Preview")
	TArray<FTrapTypeInfo> TrapTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Preview")
	TArray<UMaterial*> Materials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Placement")
	UTrapManagerInstanceSubsystem* TrapManager; 

	/// <summary>
	/// Enable Trap placement
	/// </summary>
	void EnablePlacement();
	/// <summary>
	/// Disable Trap placement
	/// </summary>
	void DisablePlacement();

	/// <summary>
	/// Method called when an Object starts overlapping with TrapEdition, adding to the collision counter
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/// <summary>
	/// Method called when an object ends overlapping with TrapEdition, substracting 1 to the collision counter
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	/// <summary>
	/// Switch mesh and current trap type to one at _NewIndex in TrapTypes array
	/// </summary>
	/// <param name="_NewIndex">: index of new trap type selected</param>
	UFUNCTION(BlueprintCallable)
	void SwitchTrapType(int _NewIndex);

	/// <summary>
	/// Switch material to one at _NewIndex in Materials array
	/// </summary>
	/// <param name="_NewIndex">: index of new material used</param>
	UFUNCTION(BlueprintCallable)
	void SwitchMaterial(int _NewIndex);

	FORCEINLINE TArray<FTrapTypeInfo> GetTrapTypes() { return TrapTypes; }

	/// <summary>
	/// Move to location on grid nearest to given location, if not already at said location
	/// </summary>
	/// <param name="_NewLocation">: location to move to approximation on the grid</param>
	void UpdateLocation(FVector3f _NewLocation);

	/// <summary>
	/// Enable Trap preview
	/// </summary>
	void Enable();
	/// <summary>
	/// Disable Trap preview
	/// </summary>
	void Disable();

	FORCEINLINE FTrapTypeInfo GetCurrentTrapInfo() { return TrapTypes[TrapTypeIndex]; }

	FORCEINLINE bool CanPlace() { return bCanPlace; }

	/// <summary>
	/// Spawn Trap of current Trap type
	/// </summary>
	void SpawnTrap();

	/// <summary>
	/// Check if current Trap type cost doesn't exceed available money
	/// </summary>
	/// <param name="_Money">: money available to buy Trap</param>
	void CheckTrapCost(int _Money);

	/// <summary>
	/// Rotate TrapEdition of 90 degrees along the Y axis
	/// </summary>
	void RotateTrap();

private:
	UStaticMeshComponent* Mesh;
	class UGridComponent* Grid;
	UBoxComponent* CollisionBox;
	bool bCanPlace = true;
	bool bIsMoneyAvailable = true;
	FRotator TrapRotation = FRotator::ZeroRotator;
	int CollisionNbr = 0;
};
