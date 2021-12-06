// Fill out your copyright notice in the Description page of Project Settings.


#include "Leaper.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "AIController.h"
#include "AIModule.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Main1.h"


ALeaper::ALeaper()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
	AggroSphere->SetupAttachment(GetRootComponent());
	AggroSphere->InitSphereRadius(600.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);

	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	AttackBox->SetupAttachment(GetRootComponent());

	bOverlappingCombatSphere = false;
	MainInHitRange = false;

	Damage = 15.f;
	MovementSpeed = 450.f;
	TurnRate = 250.f;

}

void ALeaper::BeginPlay()
{
	Super::BeginPlay();

	//AIController = Cast<AAIController>(GetController());

	AggroSphere->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::AggroSphereOnOverlapBegin);
	AggroSphere->OnComponentEndOverlap.AddDynamic(this, &ALeaper::AggroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &ALeaper::CombatSphereOnOverlapEnd);

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::AttackBoxOnOverlapBegin);
	AttackBox->OnComponentEndOverlap.AddDynamic(this, &ALeaper::AttackBoxOnOverlapEnd);

}

void ALeaper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeaper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALeaper::AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain1* Main = Cast<AMain1>(OtherActor);
		if (Main)
		{
			MoveToTarget(Main);
		}
	}
}

void ALeaper::AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ALeaper::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain1* Main = Cast<AMain1>(OtherActor);
		{
			if (Main)
			{
				CombatTarget = Main;
				bOverlappingCombatSphere = true;
				SetLeaperMovementStatus(ELeaperMovementStatus::EMS_Attacking);
			}
		}
	}
}

void ALeaper::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain1* Main = Cast<AMain1>(OtherActor);
		{
			if (Main)
			{
				SetLeaperMovementStatus(ELeaperMovementStatus::EMS_MoveToTarget);
				bOverlappingCombatSphere = false;
				MoveToTarget(Main);
				CombatTarget = nullptr;
			}
		}
	}
}

void ALeaper::MoveToTarget(class AMain1* Target)
{
	SetLeaperMovementStatus(ELeaperMovementStatus::EMS_MoveToTarget);

	//if (AIController)
	//{
		/*FAIMoveRequest MoveRequest; // Struct containing the target
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(25.0f); // minmum distance between EnemyHitBox and TargetHitBox before the enemy stops moving

		FNavPathSharedPtr NavPath; // Will store navigatin data for the Enemy AI to use

		AIController->MoveTo(MoveRequest, &NavPath); // Makes the Enemy follow player until it gets close enough to attack
		*/
		/**
		auto PathPoints = NavPath->GetPathPoints();
		for (auto Point : PathPoints)
		{
			FVector Location = Point.Location;

			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);
		}
		*/
	//}
}

void ALeaper::AttackBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MainInHitRange = true;

	UE_LOG(LogTemp, Warning, TEXT("AttackBoxOnOverlapBegin()"));
}

void ALeaper::AttackBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MainInHitRange = false;
	UE_LOG(LogTemp, Warning, TEXT("AttackBoxOnOverlapEnd()"));
}

void ALeaper::HitPlayer()
{
	if (MainInHitRange)
	{
		// cast TakeDMG function on player
		if (CombatTarget)
		{
			Cast<ACreature>(CombatTarget)->TakeDMG(Damage);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("HitPlayer()"));
}