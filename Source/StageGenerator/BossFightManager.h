// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossFightManager.generated.h"

UCLASS()
class STAGEGENERATOR_API ABossFightManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossFightManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
		class UBoxComponent* RootBox; // nothing to se here just a tiny hack because unreal in being unreal

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
		class UBoxComponent* SpawnBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
		class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TArray<TSubclassOf<class ACreature>> CreatureClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<class ACreature> Boss1Class;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<class ACreature> Boss2Class;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BossManagement")
		class ACreature* Boss{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		int32 WaveDifficulty { 25 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
		int32 PointsSpawned { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		float SpawnDelay{ 30.f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
		float TimeSinceLastSpawn{ 0.f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
		bool bIsActive{ false };
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning")
		class ACreature* MainRef{ nullptr };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnWave();

	UFUNCTION()
		virtual void TriggerBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
