// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordSlice.h"
#include "Components/BoxComponent.h"
#include "Main1.h"
#include "Creature.h"

ASwordSlice::ASwordSlice()
{
	PrimaryActorTick.bCanEverTick = true;

	SquareHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SquareHitBox"));
	SquareHitBox->SetupAttachment(GetRootComponent());

	bDestroyOnHit = false;
}

void ASwordSlice::BeginPlay()
{
	Super::BeginPlay();

	SquareHitBox->OnComponentBeginOverlap.AddDynamic(this, &ASwordSlice::SquareBoxOnOverlapBegin);
	SquareHitBox->OnComponentEndOverlap.AddDynamic(this, &ASwordSlice::SquareBoxOnOverlapEnd);
	/*SquareHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SquareHitBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SquareHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SquareHitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);*/
}

void ASwordSlice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + (ForwardVector * DeltaTime * MovementSpeed), true);
}

void ASwordSlice::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Enemy = Cast<ACreature>(OtherActor);
		if (Enemy)
		{
			if (HitEnemies.Contains(Enemy))
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit enemy with slice"));
			}
		}
	}
}

void ASwordSlice::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ASwordSlice::SquareBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Enemy = Cast<ACreature>(OtherActor);
		if (Enemy)
		{
			if(!Enemy->bIsMainCharacter)
			HitEnemies.Add(Enemy);
		}
	}
}

void ASwordSlice::SquareBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ACreature* Enemy = Cast<ACreature>(OtherActor);
		if (Enemy)
		{
			HitEnemies.Remove(Enemy);
		}
	}

}