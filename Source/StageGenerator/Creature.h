// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Creature.generated.h"

UCLASS()
class STAGEGENERATOR_API ACreature : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACreature();

	// Positions the camera behind the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Character Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UFUNCTION(BlueprintCallable)
	void TakeDMG(float damage);
	void ApplyStunn(float duration);

	// event for hit fx
	UFUNCTION(BlueprintImplementableEvent, Category = "Event", meta = (DisplayName = "OnHit"))
	void OnHit();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float JumpVelocity;

	float getHp() { return hp; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	bool IFrameOn;

	float InvulnTimer;

	float InvulnDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bCanMove;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float maxHp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float hp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		int32 points { 1 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float SpawnRate { 100.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		float StunTime{ 0.f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		bool bStunned{ false };

private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called for forward and backward input
	void MoveForward(float Value);

	// Called for left and right input
	void MoveRight(float Value);

	//Left mouse action
	virtual void LeftMousePressed();
	virtual void LeftMouseReleased();

	//Right mouse action
	virtual void RightMousePressed();
	virtual void RightMouseReleased();

	//Special Action on E button
	virtual void SpecialPressed();
	virtual void SpecialReleased();

	virtual void Aggro(class ACreature* target);

	bool bLeftMousePressed;
	bool bRightMousePressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	bool bIsMainCharacter;

	class ACombatSpawner* Master{ nullptr };

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
		class AMain1* user{ nullptr };*/

};
