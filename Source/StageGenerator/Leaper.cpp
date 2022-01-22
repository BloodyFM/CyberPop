// Fill out your copyright notice in the Description page of Project Settings.


#include "Leaper.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "AIController.h"
#include "AIModule.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


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

	DashSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DashSphere"));
	DashSphere->SetupAttachment(GetRootComponent());
	DashSphere->InitSphereRadius(100.f);

	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	AttackBox->SetupAttachment(GetRootComponent());

	bOverlappingCombatSphere = false;
	MainInHitRange = false;

	Damage = 50.f;
	MovementSpeed = 450.f;
	TurnRate = 250.f;

}

void ALeaper::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	AggroSphere->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::AggroSphereOnOverlapBegin);
	AggroSphere->OnComponentEndOverlap.AddDynamic(this, &ALeaper::AggroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &ALeaper::CombatSphereOnOverlapEnd);

	DashSphere->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::DashSphereOnOverlapBegin);

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::AttackBoxOnOverlapBegin);
	AttackBox->OnComponentEndOverlap.AddDynamic(this, &ALeaper::AttackBoxOnOverlapEnd);

}

void ALeaper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDashing)
	{
		FVector distance = DashTargetLocation - GetActorLocation();
		if (distance.Size() <= 70.f)
		{
			bIsDashing = false;
		}
		else
		{
			distance.Normalize();
			FVector force = distance * 500.f;
			SetActorLocation(GetActorLocation() + (force * DeltaTime));
		}
	}

	if (bOverlappingCombatSphere && CombatTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("spin"));
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, TurnRate);

		SetActorRotation(InterpRotation);
	}
}

FRotator ALeaper::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator YawLookAtRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
	return YawLookAtRotation;
}

void ALeaper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALeaper::AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
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
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
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
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
			{
				//SetLeaperMovementStatus(ELeaperMovementStatus::EMS_MoveToTarget);
				bOverlappingCombatSphere = false;
				//MoveToTarget(Main);
				if (LeaperMovementStatus != ELeaperMovementStatus::EMS_Attacking)
				{
					MoveToTarget(CombatTarget);
					CombatTarget = nullptr;
				}
			}
		}
	}
}

void ALeaper::DashSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
			{
				CombatTarget = Main;
				SetLeaperMovementStatus(ELeaperMovementStatus::EMS_Attacking);
				Dash(CombatTarget);
			}
		}
	}
}


void ALeaper::MoveToTarget(class ACreature* Target)
{
	SetLeaperMovementStatus(ELeaperMovementStatus::EMS_MoveToTarget);
	CombatTarget = nullptr;

	if (AIController)
	{
		FAIMoveRequest MoveRequest; // Struct containing the target
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(25.0f); // minmum distance between EnemyHitBox and TargetHitBox before the enemy stops moving

		FNavPathSharedPtr NavPath; // Will store navigatin data for the Enemy AI to use

		AIController->MoveTo(MoveRequest, &NavPath); // Makes the Enemy follow player until it gets close enough to attack
		
		/**
		auto PathPoints = NavPath->GetPathPoints();
		for (auto Point : PathPoints)
		{
			FVector Location = Point.Location;

			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);
		}
		*/
	}
}

void ALeaper::Dash(class ACreature* Target)
{
	bIsDashing = true;
	DashTargetLocation = Target->GetActorLocation();
}

void ALeaper::AttackBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
				MainInHitRange = true;

			UE_LOG(LogTemp, Warning, TEXT("AttackBoxOnOverlapBegin()"));
		}
	}
}

void ALeaper::AttackBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
				MainInHitRange = false;

			UE_LOG(LogTemp, Warning, TEXT("AttackBoxOnOverlapEnd()"));
		}
	}
}

void ALeaper::HitPlayer()
{
	if (MainInHitRange)
	{
		// cast TakeDMG function on player
		if (CombatTarget)
		{
			CombatTarget->TakeDMG(Damage);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("HitPlayer()"));
}

void ALeaper::Aggro(ACreature* target)
{
	MoveToTarget(target);
}