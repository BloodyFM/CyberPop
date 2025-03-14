// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "Main1.generated.h"

/**
 * 
 */
UCLASS()
class STAGEGENERATOR_API AMain1 : public ACreature
{
	GENERATED_BODY()

public:

	AMain1();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "WeaponStaticMesh")
		class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float MovementSpeedDash;


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
		void SpecialAttack();

	// Dash functions
	UFUNCTION()
		void Dash();
	UFUNCTION()
		void StopDashing();
	UFUNCTION()
		void ResetDash();
	
	// Dash values
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
		float DashDistance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
		float DashCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
		bool bCanDash;
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
	UPROPERTY()
		bool bDashing;
	UPROPERTY()
		FVector SpeedBeforeDash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
		float DashStop;
	UPROPERTY()
		FTimerHandle DashHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
		bool bIsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float DashCharge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float DashChargeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float ResetCombo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float ResetRate;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
		class AItem* ActiveOverlappingItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Items")
		class AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TSubclassOf<class ASwordSlice> SwordSliceClass;

	FORCEINLINE void SetEquippedWeapon(AWeapon* WeaponToSet) { EquippedWeapon = WeaponToSet; }

	void Equip();

	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

	UFUNCTION(BlueprintCallable)
		void SwordSlice();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		bool bSendOutSwordSlice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	void GiveHP();

	void GiveDash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void LockOnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void LockOnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void LockOnNotify();

};
