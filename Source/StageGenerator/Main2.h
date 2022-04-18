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
