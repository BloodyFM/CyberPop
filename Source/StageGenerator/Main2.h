// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "Main2.generated.h"

/**
 * 
 */
UCLASS()
class STAGEGENERATOR_API AMain2 : public ACreature
{
	GENERATED_BODY()
	
public:

	AMain2();

	// Fist hitboxes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
		class UBoxComponent* FistBoxL;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
		class UBoxComponent* FistBoxR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
		class UBoxComponent* ShieldBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shield")
	class UStaticMeshComponent* ShieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float MovementSpeedTank;

	//Lock on
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LockOn")
		class USphereComponent* LockOnSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockOn")
		TArray<ACreature*> LockOnList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LockOn")
		ACreature* LockOnTarget = nullptr;
	UFUNCTION()
		void RemoveLockOnTarget(ACreature* Creature);
	UFUNCTION()
		FRotator GetLookAtRotationYaw(FVector Target);
	UFUNCTION()
		ACreature* FindBestLockOnTarget();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		float MagnetSpeed;


	//Attack functions
	UFUNCTION()
		void Attack();
	UFUNCTION()
		void RangedAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool bCanAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool bNotAttacked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool bAttack1Over;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool bAttack2Over;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool bAttack3Over;

	//Shield floats
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
		bool bShielding;
	UPROPERTY()
		bool bCanShield;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float ShieldCharge;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float ShieldChargeMax;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float ShieldChargeMinimum;

	//Bullet Floats
	UPROPERTY()
		bool bShooting;
	UPROPERTY()
		bool bCanShoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float BulletCharge;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float BulletChargeMax;

	//Left mouse action
	virtual void LeftMousePressed() override;
	virtual void LeftMouseReleased() override;

	//Right mouse action
	virtual void RightMousePressed() override;
	virtual void RightMouseReleased() override;

	//Special Action on E
	virtual void SpecialPressed() override;
	virtual void SpecialReleased() override;

	bool bLeftMousePressed;
	bool bRightMousePressed;
	bool bSpecialPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float DrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
		float ShieldDrainRateUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
		float ShieldDrainRateDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
		bool bAttacking;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
		float Damage;

	void GiveHP();

	void GiveShield();

	void GiveBullets();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		class UAnimMontage* CombatMontage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void FistBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void FistBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void ActivateCollision();

	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();

	UFUNCTION()
		virtual void ShieldBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void ShieldBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void ActivateShield();

	UFUNCTION(BlueprintCallable)
		void DeactivateShield();

	UFUNCTION(BlueprintImplementableEvent, Category = "Shield")
		void ShieldOpacityOn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Shield")
		void ShieldOpacityOff();

	UFUNCTION()
		virtual void LockOnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void LockOnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void LockOnNotify();
};
