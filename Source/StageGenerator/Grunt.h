// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "Grunt.generated.h"


UENUM(BlueprintType)
enum class EGruntMovementStatus :uint8
{
	EMS_Idle			UMETA(DisplayName = "Idle"),
	EMS_Retreat			UMETA(DisplayName = "Retreat"),
	EMS_Attacking		UMETA(DisplayName = "Attacking"),
	EMS_Reload			UMETA(DisplayName = "Reload"),

	EMS_MAX				UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class STAGEGENERATOR_API AGrunt : public ACreature
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGrunt();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EGruntMovementStatus GruntMovementStatus;

	FORCEINLINE void SetGruntMovementStatus(EGruntMovementStatus Status) { GruntMovementStatus = Status; }

	// Sphere that will aggro the ai if the player overlaps
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* AggroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	// Movement values
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed{ 350.f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float TurnRate{ 250.f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bInterpToTarget{ false };

	// Combat values
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 MaxAmmo = 15;
	int32 Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float shootingDelayFocused{ 0.1f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float shootingDelaySpread{ 1.f };
	float timeSinceLastShot{ 0.f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float ReloadDelay{ 3.f }; // temporary untill I get animation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float inaccuracy{ 10.f };
	bool bEnemyToClose{ false };


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class ABullet> BulletClass;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
		FRotator GetLookAtRotationYaw(FVector Target);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		virtual void AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	ACreature* CombatTarget { nullptr };
};
