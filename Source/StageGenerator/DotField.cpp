// Fill out your copyright notice in the Description page of Project Settings.


#include "DotField.h"
#include "Components/BoxComponent.h"
#include "Creature.h"


ADotField::ADotField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DotCollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("DotCollisionVolume"));
	DotCollisionVolume->SetupAttachment(GetRootComponent());

}

void ADotField::BeginPlay()
{
	Super::BeginPlay();

	DotCollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ADotField::DotOnOverlapBegin);
	DotCollisionVolume->OnComponentEndOverlap.AddDynamic(this, &ADotField::DotOnOverlapEnd);
}

void ADotField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		CombatTarget->TakeDMG(30.f);
	}
}

void ADotField::DotOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
				CombatTarget = Main;
		}
	}
}

void ADotField::DotOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
				CombatTarget = nullptr;
		}
	}
}