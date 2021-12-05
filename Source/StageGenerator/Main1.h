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
	UPROPERTY()
		bool bDashing;
	UPROPERTY()
		FVector SpeedBeforeDash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
		float DashStop;
	UPROPERTY()
		FTimerHandle DashHandle;

	//Left mouse action
	virtual void LeftMousePressed() override;
	virtual void LeftMouseReleased() override;

	//Right mouse action
	virtual void RightMousePressed() override;
	virtual void RightMouseReleased() override;

	bool bLeftMousePressed;
	bool bRightMousePressed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
