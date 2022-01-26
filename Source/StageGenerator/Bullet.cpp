// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Creature.h"
#include "Main1.h"


ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
	SetRootComponent(HitBox);

	Mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	IdleParticlesComponent = CreateAbstractDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticlesComponent"));
	IdleParticlesComponent->SetupAttachment(GetRootComponent());

	Damage = 10.f;
	MovementSpeed = 200.f;
	MaxLifeSpan = 10.f;
	CurrentLifeSpan = 0.f;
	bDestroyOnHit = true;
	bHarmsMain = true;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	ForwardVector = GetActorForwardVector();

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLifeSpan += DeltaTime;
	if (CurrentLifeSpan >= MaxLifeSpan)
	{
		Destroy();
	}
	SetActorLocation(GetActorLocation() + (ForwardVector * DeltaTime * MovementSpeed), true);
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* HitTarget = Cast<ACreature>(OtherActor);
		if (HitTarget)
		{
			bool Ignore = OtherComp->ComponentHasTag("Ignore");
			if (bHarmsMain == HitTarget->bIsMainCharacter && !Ignore)
			{
				AMain1* MainTarget = Cast<AMain1>(OtherActor);
				if (MainTarget)
				{
					if (MainTarget->bDashing == true)
					{
						UE_LOG(LogTemp, Warning, TEXT("Player dodges bullets"));
					}
					else
					{
						HitTarget->TakeDMG(Damage);
						OverlapUtility();
					}
				}
				else
				{
					HitTarget->TakeDMG(Damage);
					OverlapUtility();
				}
				UE_LOG(LogTemp, Warning, TEXT("Overlap"));
			}
		}
	}
}

void ABullet::OverlapUtility()
{
	if (OverlapParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
	}
	if (OverlapSound)
	{
		UGameplayStatics::PlaySound2D(this, OverlapSound);
	}
	if (bDestroyOnHit)
	{
		Destroy();
	}
}