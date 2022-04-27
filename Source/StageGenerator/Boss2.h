// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "Boss2.generated.h"


UENUM(BlueprintType)
enum class EBoss2MovementStatus :uint8
{
	EMS_Idle			UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DisplayName = "MoveToTarget"),
	EMS_SplitFire		UMETA(DisplayName = "SplitFire"),
	EMS_HomingSmash		UMETA(DisplayName = "HomingSmash"),
	EMS_ShootPattern	UMETA(DisplayName = "ShootPattern"),
	EMS_Opener			UMETA(DisplayName = "Opener"),
	EMS_Stun			UMETA(DisplayName = "Stun"),

	EMS_MAX				UMETA(DisplayName = "DefaultMAX")
};


UCLASS()
class STAGEGENERATOR_API ABoss2 : public ACreature
{
	GENERATED_BODY()
	
public:

	ABoss2();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		EBoss2MovementStatus Boss2MovementStatus;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement") // thinking this could be handy to remember where to continnue after getting stunned
		EBoss2MovementStatus Boss2MemoryStatus;
	
	FORCEINLINE void SetBoss2MovementStatus(EBoss2MovementStatus Status) { Boss2MovementStatus = Status; }
	FORCEINLINE void SetBoss2MemoryStatus(EBoss2MovementStatus Status) { Boss2MovementStatus = Status; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class AAIController* AIController;

	//Movementy values
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float TurnRate{ 250.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bInterpToTarget{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TSubclassOf<class ABullet> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "combat")
		float AOERange{ 250.f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "combat")
		FVector JumpTargetLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		FRotator GetLookAtRotationYaw(FVector Target);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	ACreature* CombatTarget{ nullptr };

	UFUNCTION(BlueprintCallable)
	void AOEDamage();

	UFUNCTION(BlueprintCallable)
	void InitiateJumpToCombatTarget();

	virtual void Aggro(class ACreature* target) override;

};
