// Fill out your copyright notice in the Description page of Project Settings.

#include "Exit.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Creature.h"


// Sets default values
AExit::AExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
	RootComponent = HitBox;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AExit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExit::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
