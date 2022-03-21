// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "GameFramework/Actor.h"
#include "ItemLayoutManager.generated.h"


USTRUCT() struct FItemStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AActor> ItemClass;
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"), Category = "Spawning")
	FTransform Transform;
};

USTRUCT() struct FLayoutStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<FItemStruct> Items;
};

UCLASS()
class STAGEGENERATOR_API AItemLayoutManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemLayoutManager();

	
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<FLayoutStruct> ItemLayouts;
	//TArray<TPair<FString, uint32>> ItemLayouts;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
