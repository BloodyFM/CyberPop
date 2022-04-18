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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
