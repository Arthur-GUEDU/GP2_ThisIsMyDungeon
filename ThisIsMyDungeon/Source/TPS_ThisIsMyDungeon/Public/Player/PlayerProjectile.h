// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerProjectile.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProjectileEvent);

UCLASS()
class TPS_THISISMYDUNGEON_API APlayerProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	class URotatingMovementComponent* RotatingMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	bool bIsActive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float DamageAmount = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ProjectileResetTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ProjectileResetTimeStamp = 0.1f;

	UPROPERTY()
	class UTimer* ProjectileResetTimer;

	FVector SpawnPosition;

	//SOUNDS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta=(AllowPrivateAccess))
	USoundBase* FireSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta=(AllowPrivateAccess))
	USoundBase* HitSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta=(AllowPrivateAccess))
	TArray<USoundBase*> FlySoundsCues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta=(AllowPrivateAccess))
	USoundBase* SecretFlySoundsCue;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Activate a pre spawned projectile and launches it from a position to a certain direction
	/// </summary>
	/// <param name="Position">: Launch position</param>
	/// <param name="Direction">: Launch direction</param>
	void ActivateProjectile(FVector Position, FVector Direction);

	/// <summary>
	/// Deactivate the projectile when it hit something
	/// </summary>
	UFUNCTION()
	void DeactivateProjectile();

	/// <summary>
	/// On hit method - Will be called when the projectile hit something
	/// </summary>
	/// <param name="HitComp">: Hit component</param>
	/// <param name="OtherActor">: Other actor collided</param>
	/// <param name="OtherComp">: Other component collided</param>
	/// <param name="NormalImpulse">: Normal vector on the impact</param>
	/// <param name="Hit">: Hit result infos</param>
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:

	UPROPERTY(BlueprintAssignable, Category = "Projectile events")
	FProjectileEvent OnProjectileFired;
	
	UPROPERTY(BlueprintAssignable, Category = "Projectile events")
	FProjectileEvent OnProjectileDestroyed;

	UPROPERTY(BlueprintAssignable, Category = "Projectile events")
	FProjectileEvent OnProjectileHit;

	/// <summary>
	/// Launch projectile method publicly accessible
	/// </summary>
	/// <param name="LaunchPosition">: Position where the projectile starts from</param>
	/// <param name="LaunchDirection">: Direction where the projectile is going</param>
	UFUNCTION(BlueprintCallable)
	bool LaunchProjectile(FVector LaunchPosition, FVector LaunchDirection);
};
