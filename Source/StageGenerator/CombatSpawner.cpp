// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h" // to get player
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
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
	//TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ACombatSpawner::TriggerBoxOnOverlapEnd);


	ACreature* player = Cast<ACreature>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player)
	{
		// insert player speciffic modifiers
	}
	
	Difficulty += DifficultyBonus;

	for (int32 i = 0; i < CreatureClasses.Num(); i++)
	{
		CreatureClasses[i];
		while (true)
		{
			if ((PointsOnField + CreatureClasses[i].GetDefaultObject()->points) > Difficulty)
				break;

			float Roll = FMath::RandRange(0.f, 100.f);
			if (Roll > CreatureClasses[i].GetDefaultObject()->SpawnRate)
				break;

			PointsOnField += CreatureClasses[i].GetDefaultObject()->points;
			FVector Extent = SpawnBox->GetScaledBoxExtent();
			FVector Origin = SpawnBox->GetComponentLocation();

			FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(Point);
			ACreature* SpawnedCreature = GetWorld()->SpawnActor<ACreature>(CreatureClasses[i], SpawnTransform);
			SpawnedCreature->SpawnDefaultController();
			SpawnedCreature->Master = this;
			SavePointerToCreature(SpawnedCreature);
		}
		/**
		int32 ammount = FMath::RandRange(2, 8);
		for (int32 j = 0; j < ammount; j++)
		{
			FVector Extent = SpawnBox->GetScaledBoxExtent();
			FVector Origin = SpawnBox->GetComponentLocation();

			FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(Point);
			ACreature* SpawnedCreature = GetWorld()->SpawnActor<ACreature>(CreatureClasses[i], SpawnTransform);
			SpawnedCreature->SpawnDefaultController();
			SpawnedCreature->Master = this;
			SavePointerToCreature(SpawnedCreature);
		}
		*/
	}
	
}

// Called every frame
void ACombatSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnedCreatures.Num() == 0)
	{
		DestroyBarriers();
	}
}

void ACombatSpawner::TriggerBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
			{
				//UE_LOG(LogTemp, Warning, TEXT("barriers: %d"), BarrierSpawns.Num());
				for (int32 i = 0; i < BarrierSpawns.Num(); i++)
				{
					FTransform transform = BarrierSpawns[i];
					FRotator rotation = transform.GetRotation().Rotator() + GetActorRotation();
					FVector position = transform.GetLocation().RotateAngleAxis(GetActorRotation().Yaw, FVector(0.f, 0.f, 1.f));
					position += GetActorLocation();
					transform.SetRotation(rotation.Quaternion());
					transform.SetLocation(position);
					SavePointerToBarrier(GetWorld()->SpawnActor<ABarrier>(BarrierClass, transform));
				}
				TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				for (int32 i = 0; i < SpawnedCreatures.Num(); i++)
				{
					//give enemies the ref to the player and aggro them
					SpawnedCreatures[i]->Aggro(Main);
				}
			}
		}
	}
}

/*void ACombatSpawner::TriggerBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			DestroyBarriers();
		}
	}
}*/


void ACombatSpawner::SavePointerToBarrier(ABarrier* barrier)
{
	SpawnedBarriers.Add(barrier);
}

void ACombatSpawner::SavePointerToCreature(ACreature* creature)
{
	SpawnedCreatures.Add(creature);
}

void ACombatSpawner::DestroyBarriers()
{
	for (int32 i = 0; i < SpawnedBarriers.Num(); i++)
	{
		SpawnedBarriers[i]->Unlock();
	}
	SpawnedBarriers.Empty();
}

void ACombatSpawner::RemoveServant(ACreature* servant)
{
	for (int32 i = 0; i < SpawnedCreatures.Num(); i++)
	{
		if (SpawnedCreatures[i] == servant)
		{
			SpawnedCreatures.RemoveAt(i);
			break;
		}
	}
}