// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyCharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyManagerInstanceSubsystem.h"
#include "Environment/SpawnerActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/PlayerCharacter.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter() 
{
	PrimaryActorTick.bCanEverTick = false; //not needed atm
	GetCharacterMovement()->MaxWalkSpeed=MoveSpeed;
	Health=MaxHealth-10;
	
	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Treasure Parent"));
	ChestMesh->SetupAttachment(GetMesh(), FName("chestSocket"));

	
	UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/LPSDeluxe2TropicalEnvironment/LPSD2_Meshes/Treasure/SM_PirateChest_3.SM_PirateChest_3"));
	if (LoadedMesh)
	{
		ChestMesh->SetStaticMesh(LoadedMesh);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load mesh"));
	}
}



// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::Activate(ASpawnerActor* instigator, FVector Location, FRotator Rotation)
{
	if(!bIsActive)
	{
		SetActorLocation(Location);
		SetActorRotation(Rotation);
		// Enable Rendering
		GetMesh()->SetVisibility(true, true); // Show the mesh and all children
		ChestMesh->SetVisibility(false, true); //we dont want to see the chest mesh at the start
		SetActorHiddenInGame(false); // Show the actor in the game world

		// Enable Movement
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking); // Set to walking or desired mode
		}

		// Enable Collision
		SetActorEnableCollision(true); // Enable collision for the actor

		if(UBrainComponent* brain = GetComponentByClass<UBrainComponent>())
			brain->Deactivate();

		if (UBlackboardComponent* Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent())
		{
			Blackboard->SetValueAsBool(TEXT("Spawning"), true);
			Blackboard->SetValueAsBool(TEXT("CanAttack"), true);
			Blackboard->SetValueAsBool(TEXT("CanMove"), true);
			Blackboard->SetValueAsBool(TEXT("IsDead"), false);
			Blackboard->SetValueAsBool(TEXT("IsActive"), true);
			Blackboard->SetValueAsBool(TEXT("HasTreasure"), false);
			Blackboard->SetValueAsBool(TEXT("IsPlayerClose"), false);
			Blackboard->SetValueAsBool(TEXT("IsExitInRange"), false);
			Blackboard->SetValueAsBool(TEXT("IsObjectiveInRange"), false);
			Blackboard->SetValueAsBool(TEXT("IsInteracting"), false);	
			Blackboard->SetValueAsBool(TEXT("IsPlayerInAttackRange"), false);

			USkeletalMeshComponent* SkeletalMeshComp = GetMesh();
			UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
			FProperty* Property = AnimInstance->GetClass()->FindPropertyByName(TEXT("SpawnFinished"));
			FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);
			void* BoolAddress = Property->ContainerPtrToValuePtr<void>(AnimInstance);
			BoolProperty->SetPropertyValue(BoolAddress, false);
		}
		
		Health=MaxHealth;

		bHasTreasure=false;
		bIsActive = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Tried to instantiate an actor that was already instantiated!"), *instigator->GetName());
	}
}

void AEnemyCharacter::Deactivate()
{
	// Disable Rendering
	GetMesh()->SetVisibility(false, true); // Hide the mesh and all children
	SetActorHiddenInGame(true); // Hide the entire actor in the game world

	// Disable Movement
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->StopMovementImmediately();
	}

	// Disable Collision
	SetActorEnableCollision(false); // Disable collision entirely for the actor

	if(UBrainComponent* brain = GetComponentByClass<UBrainComponent>())
		brain->Deactivate();

	GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>()->OnEnemyDeactivate();
	SetActorLocation(GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>()->GetInactiveLocation()); //get the inactive store location from the enemy manager
	bIsActive=false;
}

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                  AController* EventInstigator, AActor* DamageCauser)
{
	UBlackboardComponent* Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent();
	if(Blackboard && Blackboard->GetValueAsBool("Spawning")) //dont take damage while spawning
	{
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	Health -= (DamageAmount);
	if(!bIsDead && Health<=0.f)
	{
		OnDeath();
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	//Prevent the player from moving while in the hit animation
	if ( Blackboard && !Blackboard->GetValueAsBool("IsInteracting") && Blackboard->GetValueAsBool("CanMove"))
	{
		Blackboard->SetValueAsBool(TEXT("CanMove"), false);
		
		//Set IsHit in the Anim BP
		USkeletalMeshComponent* SkeletalMeshComp = GetMesh();
		UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
		FProperty* Property = AnimInstance->GetClass()->FindPropertyByName(TEXT("IsHit"));
		FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);
		void* BoolAddress = Property->ContainerPtrToValuePtr<void>(AnimInstance);
		BoolProperty->SetPropertyValue(BoolAddress, true);
	}
	int soundCueNB = FMath::RandRange(0,DamageTakenSoundCues.Num()-1); 
	if(DamageTakenSoundCues.Num()!=0 && DamageTakenSoundCues[soundCueNB])
	{
		UGameplayStatics::PlaySoundAtLocation(this, DamageTakenSoundCues[soundCueNB], GetActorLocation());
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

float AEnemyCharacter::Heal(float amount)
{
	if(Health+amount > MaxHealth)
	{
		float rest = amount - (MaxHealth - Health);
		Health = MaxHealth;
		return rest;
	}
	Health += amount;
	return 0;
}

void AEnemyCharacter::OnDeath()
{
	if(bHasTreasure && TreasureAmount > 0)
		GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>()->SpawnCoinDrop(GetActorLocation(), TreasureAmount);
	
	APlayerCharacter* const player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->GainMoney(Value);
	if(DeathCue)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathCue, GetActorLocation());
	}
	Deactivate();
}

void AEnemyCharacter::AcquireTreasure(float amount)
{
	if(!bHasTreasure)
	{
		bHasTreasure = true;
		TreasureAmount = amount;
		ChestMesh->SetVisibility(true, true);
		UBlackboardComponent* Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent();
		if (Blackboard)
		{
			Blackboard->SetValueAsBool(TEXT("HasTreasure"), true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Tried to get treasure when it already had some"), *GetName());
	}
}

void AEnemyCharacter::SetMoveSpeed(float Amount)
{
	GetCharacterMovement()->MaxWalkSpeed = Amount;
}

void AEnemyCharacter::SetMoveSpeedMult(float Mult)
{
	GetCharacterMovement()->MaxWalkSpeed=MoveSpeed*Mult;
	//UE_LOG(LogTemp, Warning, TEXT("%s : Move speed set to mult of %f"), *GetName(), mult);
}

void AEnemyCharacter::ResetMoveSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed=MoveSpeed;
	//UE_LOG(LogTemp, Warning, TEXT("%s : Move speed Reset"), *GetName());
}

UBehaviorTree* AEnemyCharacter::GetBehaviorTree() const
{
	return Tree;
}

ALevelDefinitionActor* AEnemyCharacter::GetLevelDefinition()
{
	return LevelDefinition;
}

bool AEnemyCharacter::IsPlayerInSenseRange() const
{
	GEngine->AddOnScreenDebugMessage(0002, 2.f, FColor::Orange, TEXT("HIIIII"));
	return GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))<200.f;
	
}

bool AEnemyCharacter::CanMove() const
{
	return bCanMove;
}

