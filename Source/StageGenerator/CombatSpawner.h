// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatSpawner.generated.h"

UCLASS()
class STAGEGENERATOR_API ACombatSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatSpawner();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	class UBoxComponent* SpawnBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"), Category = "Spawning")
	TArray<FTransform> BarrierSpawns;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<class ABarrier*> SpawnedBarriers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class ABarrier> BarrierClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SavePointerToBarrier(class ABarrier* barrier);

	UFUNCTION()
	void DestroyBarriers();

	UFUNCTION()
	virtual void TriggerBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//virtual void TriggerBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
