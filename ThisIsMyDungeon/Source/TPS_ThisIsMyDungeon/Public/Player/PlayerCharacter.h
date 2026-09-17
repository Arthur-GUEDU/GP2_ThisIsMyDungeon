// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerEvent);

UCLASS()
class TPS_THISISMYDUNGEON_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnhancedInput)
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnhancedInput)
	class UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnhancedInput)
	UInputAction* LookInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnhancedInput)
	UInputAction* ShootInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnhancedInput)
	UInputAction* SwitchModeInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnhancedInput)
	UInputAction* TrapUINavInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnhancedInput)
	UInputAction* ConfirmTrapInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnhancedInput)
	UInputAction* RotateTrapInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnhancedInput)
	UInputAction* PauseInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile parameters")
	USceneComponent* ParrotLaunchPosition;

	UPROPERTY(BlueprintReadWrite)
	class ATrapEdition* TrapEdition;

	UPROPERTY(BlueprintReadWrite)
	class APlayerProjectile* Projectile;

	UPROPERTY(BlueprintReadOnly, Category = "Edit Mode Parameters")
	bool bIsEditMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Mode Parameters")
	float MaxEditDistance = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player parameters")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player parameters")
	float Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player parameters")
	int MaxMoney = 999999;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player parameters")
	int Money = 800;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player parameters")
	int MoneyGain = 10;

	UPROPERTY(BlueprintReadOnly, Category = "Player parameters")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ProjectileCDTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ProjectileCDTimeStamp = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float MoneyGainTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float MoneyGainTimeStamp = 0.1f;

	UPROPERTY(BlueprintReadOnly)
	class UTimer* ProjectileCDTimer;

	UPROPERTY(BlueprintReadOnly)
	class UTimer* MoneyGainTimer;

	UPROPERTY(BlueprintReadOnly)
	int CurrentTrapIndex = 0;

	FVector SpawnPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TEnumAsByte<ECollisionChannel> Channel;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Respawn the player to it's first position and resets all the health values
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void RespawnPlayer();

	/// <summary>
	/// Will try to get money from the player
	/// </summary>
	/// <param name="MoneyAmount">: Amount of money to spend </param>
	/// <returns> Returns true if succeded and false if failed </returns>
	UFUNCTION(BlueprintCallable)
	bool TryToSpendMoney(int MoneyAmount);

	/// <summary>
	/// Method called in the cpp method when a trap is being placed - Allows to implement animations in blueprint with the boolean
	/// </summary>
	/// <param name="TrapPlaced">: Send if the trap has been placed or not</param>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnTrapPlaced(bool IsTrapPlaced);

	virtual void OnTrapPlaced_Implementation(bool IsTrapPlaced);
public:	

	UPROPERTY(BlueprintAssignable, Category = "Player events")
	FPlayerEvent OnPlayerHit;

	UPROPERTY(BlueprintAssignable, Category = "Player events")
	FPlayerEvent OnPlayerDeath;

	UPROPERTY(BlueprintAssignable, Category = "Player events")
	FPlayerEvent OnPlayerRespawn;

	UPROPERTY(BlueprintAssignable, Category = "Player events")
	FPlayerEvent OnPlayerMoneyUpdated;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// Gives money to the player
	/// </summary>
	/// <param name="MoneyAmount">: Amount of money to gain </param>
	UFUNCTION(BlueprintCallable)
	void GainMoney(int MoneyAmount);
private:

	/// <summary>
	/// Creates and setups the player camera
	/// </summary>
	void SetupPlayerCamera();

	/// <summary>
	/// Method called when the move inputs are pressed
	/// </summary>
	/// <param name="_Value">: Value of movement </param>
	void MoveAction(const FInputActionValue& Value);

	/// <summary>
	/// Method called when the look inputs are triggered
	/// </summary>
	/// <param name="_Value">: Value of look movement </param>
	void LookAction(const FInputActionValue& Value);

	/// <summary>
	/// Method called when the player shoots
	/// </summary>
	/// <param name="Value">: Bool value </param>
	void ShootAction(const FInputActionValue& Value);

	/// <summary>
	/// Method called when the player switches between trap and comabat mode
	/// </summary>
	/// <param name="Value">: Bool value </param>
	void SwitchModeAction(const FInputActionValue& Value);

	/// <summary>
	/// Method called for the navigation in the traps UI
	/// </summary>
	/// <param name="Value">: Value for the next or previous trap in the UI </param>
	void TrapUINavAction(const FInputActionValue& Value);

	/// <summary>
	/// Method called when the player wants to confirm the position of a trap
	/// </summary>
	/// <param name="Value">: Bool value </param>
	void ConfirmTrapAction(const FInputActionValue& Value);

	/// <summary>
	/// Method called when the player wants to rotate preview of a trap
	/// </summary>
	/// <param name="Value">: Bool value </param>
	void RotateTrapAction(const FInputActionValue& Value);

	/// <summary>
	/// Method called when the player wants to pause or unpause the game
	/// </summary>
	/// <param name="Value">: Bool value </param>
	void PauseAction(const FInputActionValue& Value);

	/// <summary>
	/// Checks if the direction where the player is looking is a valid position for a trap
	/// </summary>
	void CheckTrapPosition();

	/// <summary>
	/// Take damage method overrided - Deals damagages to the player and eventually kills it
	/// </summary>
	/// <param name="DamageAmount">: Number of damages to deal </param>
	/// <param name="DamageEvent">: Damage type class </param>
	/// <param name="EventInvestigator">: Controller responsible for damages</param>
	/// <param name="DamageCauser">: Actor responsible for damages </param>
	/// <returns>Returns the number of damages or the player health (0) if dead</returns>
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInvestigator, AActor* DamageCauser) override;

	/// <summary>
	/// Method called when the player dies
	/// </summary>
	void PlayerDeath();

	/// <summary>
	/// Method called each X seconds to gives the player money
	/// </summary>
	UFUNCTION()
	void GainMoneyTime();

	/// <summary>
	/// Method called when the player tries to place a trap
	/// </summary>
	void TryPlaceTrap();
};
