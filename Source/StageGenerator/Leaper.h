// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "Leaper.generated.h"


UENUM(BlueprintType)
enum class ELeaperMovementStatus :uint8
{
	EMS_Idle			UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking		UMETA(DisplayName = "Attacking"),
	EMS_Stun			UMETA(DisplayName = "Stun"),

	EMS_MAX				UMETA(DisplayName = "DefaultMAX")
};


UCLASS()
class STAGEGENERATOR_API ALeaper : public ACreature
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALeaper();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	ELeaperMovementStatus LeaperMovementStatus;

	FORCEINLINE void SetLeaperMovementStatus(ELeaperMovementStatus Status) { LeaperMovementStatus = Status; }

	// Sphere that will aggro the ai if the player overlaps
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	//class USphereComponent* AggroSphere;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	//USphereComponent* CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* DashSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	// The hitbox of the attack
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	class UBoxComponent* AttackBoxL;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	class UBoxComponent* AttackBoxR;

	// Movement values
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool bInterp{ false };

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

	//UFUNCTION()
		//virtual void AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION()
		//virtual void AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//UFUNCTION()
		//virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION()
		//virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void DashSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void DashSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bInDashSphere{ false };


	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class ACreature* Target);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	float MoveToTolerance{ 25.f };

	UFUNCTION(BlueprintCallable)
	void PrepareDash(class ACreature* Target);
	UFUNCTION(BlueprintCallable)
	void Dash();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsDashing { false };
	FVector DashTargetLocation { 0.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DashPower{ 500.f };
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	float DashTolerance{100.f};

	// tells the AI if the player is within mele range
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bOverlappingDashSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	ACreature* CombatTarget { nullptr };

	UFUNCTION()
	virtual void AttackBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void HitPlayer();

	virtual void Aggro(class ACreature* target) override;

	UFUNCTION(BlueprintCallable)
		void ActivateCollision();

	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();
	
};
