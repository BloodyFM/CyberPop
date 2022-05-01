// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFightManager.h"
#include "Components/BoxComponent.h"
#include "Creature.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABossFightManager::ABossFightManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBox"));
	SetRootComponent(RootBox);

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnBox->SetupAttachment(GetRootComponent());

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ABossFightManager::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossFightManager::TriggerBoxOnOverlapBegin);

	//Will check what character player is playing in bp and spawn boss there instead
	
}

// Called every frame
void ABossFightManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive)
	{
		TimeSinceLastSpawn += DeltaTime;
		if (TimeSinceLastSpawn >= SpawnDelay)
		{
			TimeSinceLastSpawn = 0.f;
			//Spawn Wave
			SpawnWave();
		}
	}
}

void ABossFightManager::SpawnWave()
{
	PointsSpawned = 0.f;
	for (int32 i = 0; i < CreatureClasses.Num(); i++)
	{
		CreatureClasses[i];
		while (true)
		{
			if ((PointsSpawned + CreatureClasses[i].GetDefaultObject()->points) > WaveDifficulty)
				break;

			float Roll = FMath::RandRange(0.f, 100.f);
			if (Roll > CreatureClasses[i].GetDefaultObject()->SpawnRate)
				break;

			PointsSpawned += CreatureClasses[i].GetDefaultObject()->points;
			FVector Extent = SpawnBox->GetScaledBoxExtent();
			FVector Origin = SpawnBox->GetComponentLocation();

			FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
			FRotator Rotation = GetActorRotation();
			Point = Origin + Rotation.RotateVector(Origin - Point); // account for rotation
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(Point);
			ACreature* SpawnedCreature = GetWorld()->SpawnActor<ACreature>(CreatureClasses[i], SpawnTransform);
			if (SpawnedCreature)
			{
				SpawnedCreature->SpawnDefaultController();
				SpawnedCreature->Aggro(MainRef);
			}
		}
	}
}

void ABossFightManager::TriggerBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter && Boss)
			{
				//Activate Boss
				Boss->Aggro(Main);
				MainRef = Main;
				Boss->SpawnDefaultController();
				bIsActive = true;
			}
		}
	}
}

