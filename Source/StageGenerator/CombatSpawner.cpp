// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSpawner.h"
#include "Components/BoxComponent.h"
#include "Barrier.h"
#include "Creature.h"


// Sets default values
ACombatSpawner::ACombatSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SetRootComponent(SpawnBox);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ACombatSpawner::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACombatSpawner::TriggerBoxOnOverlapBegin);
	
}

// Called every frame
void ACombatSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatSpawner::TriggerBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			for (int32 i = 0; i < BarrierSpawns.Num(); i++)
			{
				GetWorld()->SpawnActor<ABarrier>(BarrierClass, BarrierSpawns[0]);
			}
		}
	}
}