#pragma once

#include "CoreMinimal.h"
#include "Traps/BaseTrap.h"
#include "Components/BoxComponent.h"
#include "SawTrap.generated.h"

/**
 * 
 */
UCLASS()
class TPS_THISISMYDUNGEON_API ASawTrap : public ABaseTrap
{
	GENERATED_BODY()

public:
	ASawTrap();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Parameters")
	float Damages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Parameters")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Parameters")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Parameters")
	float MotionRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Parameters")
	FString SceneComponentName;

	void BeginPlay() override;

public:
	/// <summary>
	/// Method called every tick, moving the saw along an axis
	/// </summary>
	/// <param name="DeltaTime"></param>
	void Tick(float DeltaTime) override;

private:
	UStaticMeshComponent* Mesh;
	UBoxComponent* DamageZone;
	USceneComponent* SceneComp;

	bool bIsMovingForward = true;

	/// <summary>
	/// Method called when an Enemy enters its DamageZone, damaging it
 	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
