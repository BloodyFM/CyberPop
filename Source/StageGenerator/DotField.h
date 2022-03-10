// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DotField.generated.h"

/**
 * 
 */
UCLASS()
class STAGEGENERATOR_API ADotField : public AItem
{
	GENERATED_BODY()

public:
	ADotField();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DotField | Collision")
		class UBoxComponent* DotCollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Target")
		class ACreature* CombatTarget { nullptr };


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void DotOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void DotOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
