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

	UFUNCTION()
		void Attack();

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
	UPROPERTY()
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float DashCharge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float DashChargeMax;

	//Left mouse action
	virtual void LeftMousePressed() override;
	virtual void LeftMouseReleased() override;

	//Right mouse action
	virtual void RightMousePressed() override;
	virtual void RightMouseReleased() override;

	bool bLeftMousePressed;
	bool bRightMousePressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DrainRate;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Items")
		class AWeapon* EquippedWeapon;

	FORCEINLINE void SetEquippedWeapon(AWeapon* WeaponToSet) { EquippedWeapon = WeaponToSet; }

	void Equip();

	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

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

};
