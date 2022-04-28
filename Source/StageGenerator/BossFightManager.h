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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TArray<TSubclassOf<class ACreature>> CreatureClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<class ACreature> Boss1Class;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<class ACreature> Boss2Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		int32 WaveDifficulty { 25 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
		int32 PointsOnSpawned { 0 };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
