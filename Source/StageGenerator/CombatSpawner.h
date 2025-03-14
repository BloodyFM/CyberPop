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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<class ACreature*> SpawnedCreatures;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<TSubclassOf<class ACreature>> CreatureClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	int32 Difficulty{ 100 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	int32 PointsOnField{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		int32 DifficultyBonus{ 0 };

	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	//TArray<int> SpawnNumber;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SavePointerToBarrier(class ABarrier* barrier);

	UFUNCTION()
	void SavePointerToCreature(class ACreature* creature);

	UFUNCTION()
	void DestroyBarriers();

	UFUNCTION()
	virtual void TriggerBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//virtual void TriggerBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void RemoveServant(class ACreature* servant);
};
