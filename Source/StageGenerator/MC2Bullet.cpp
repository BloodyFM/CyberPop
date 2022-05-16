// Fill out your copyright notice in the Description page of Project Settings.


#include "MC2Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Creature.h"
#include "Leaper.h"
#include "Grunt.h"
#include "Main2.h"

// Sets default values
AMC2Bullet::AMC2Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
	SetRootComponent(HitBox);

	Mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	IdleParticlesComponent = CreateAbstractDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticlesComponent"));
	IdleParticlesComponent->SetupAttachment(GetRootComponent());

	Damage = 30.f;
	MovementSpeed = 800.f;
	MaxLifeSpan = 10.f;
	CurrentLifeSpan = 0.f;
	bDestroyOnHit = true;
	bHarmsEnemy = true;

}

// Called when the game starts or when spawned
void AMC2Bullet::BeginPlay()
{
	Super::BeginPlay();
	
	ForwardVector = GetActorForwardVector();

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AMC2Bullet::OnOverlapBegin);
}

// Called every frame
void AMC2Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLifeSpan += DeltaTime;
	if (CurrentLifeSpan >= MaxLifeSpan)
	{
		Destroy();
	}
	SetActorLocation(GetActorLocation() + (ForwardVector * DeltaTime * MovementSpeed), true);
}

void AMC2Bullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* HitTarget = Cast<ACreature>(OtherActor);
		if (HitTarget)
		{
			bool Ignore = OtherComp->ComponentHasTag("Ignore");
			if (bHarmsEnemy != HitTarget->bIsMainCharacter && !Ignore)
			{
				if(HitTarget->hp <= Damage)
				{
					AMain2* Main = Cast<AMain2>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
					if(Main)
					{
						Main->GiveHP();
					}
				}
				HitTarget->TakeDMG(Damage);
				OverlapUtility();
				//UE_LOG(LogTemp, Warning, TEXT("Overlap"));
			}
		}
	}
}

void AMC2Bullet::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMC2Bullet::OverlapUtility()
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

