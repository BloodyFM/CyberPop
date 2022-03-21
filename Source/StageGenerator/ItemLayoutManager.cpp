// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemLayoutManager.h"

// Sets default values
AItemLayoutManager::AItemLayoutManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemLayoutManager::BeginPlay()
{
	Super::BeginPlay();

	int x = ItemLayouts.Num();
	
	if (x > 0)
	{
		x = FMath::RandRange(0, x - 1);
		for (int i = 0; i < ItemLayouts[x].Items.Num(); i++)
		{
			FTransform SpawnTransform = ItemLayouts[x].Items[i].Transform;
			FRotator rotation = SpawnTransform.GetRotation().Rotator() + GetActorRotation();
			FVector position = SpawnTransform.GetLocation().RotateAngleAxis(GetActorRotation().Yaw, FVector(0.f, 0.f, 1.f));
			position += GetActorLocation();
			SpawnTransform.SetRotation(rotation.Quaternion());
			SpawnTransform.SetLocation(position);
			AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(ItemLayouts[x].Items[i].ItemClass, SpawnTransform);
		}
	}
}

// Called every frame
void AItemLayoutManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

