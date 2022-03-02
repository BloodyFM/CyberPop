// Fill out your copyright notice in the Description page of Project Settings.


#include "Leaper.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIController.h"
#include "AIModule.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


ALeaper::ALeaper()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
	//AggroSphere->SetupAttachment(GetRootComponent());
	//AggroSphere->InitSphereRadius(600.f);

	//CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	//CombatSphere->SetupAttachment(GetRootComponent());
	//CombatSphere->InitSphereRadius(75.f);

	DashSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DashSphere"));
	DashSphere->SetupAttachment(GetRootComponent());
	DashSphere->InitSphereRadius(100.f);

	AttackBoxL = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBoxL"));
	AttackBoxL->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftClawSocket"));
	AttackBoxR = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBoxR"));
	AttackBoxR->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightClawSocket"));

	bOverlappingDashSphere = false;

	Damage = 50.f;
	MovementSpeed = 400.f;
	TurnRate = 250.f;

}

void ALeaper::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	//AggroSphere->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::AggroSphereOnOverlapBegin);
	//AggroSphere->OnComponentEndOverlap.AddDynamic(this, &ALeaper::AggroSphereOnOverlapEnd);

	//CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::CombatSphereOnOverlapBegin);
	//CombatSphere->OnComponentEndOverlap.AddDynamic(this, &ALeaper::CombatSphereOnOverlapEnd);

	DashSphere->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::DashSphereOnOverlapBegin);
	DashSphere->OnComponentEndOverlap.AddDynamic(this, &ALeaper::DashSphereOnOverlapEnd);

	AttackBoxL->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::AttackBoxOnOverlapBegin);
	AttackBoxR->OnComponentBeginOverlap.AddDynamic(this, &ALeaper::AttackBoxOnOverlapBegin);

	AttackBoxL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackBoxL->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackBoxL->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackBoxL->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AttackBoxR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackBoxR->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackBoxR->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackBoxR->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

void ALeaper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget && bInterp)
	{
		//UE_LOG(LogTemp, Warning, TEXT("spin"));
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, TurnRate);

		SetActorRotation(InterpRotation);
	}
	if (bStunned && LeaperMovementStatus != ELeaperMovementStatus::EMS_Stun)
	{
		SetLeaperMovementStatus(ELeaperMovementStatus::EMS_Stun);
		DeactivateCollision();
		bInterp = false;
		AIController->StopMovement();
		UE_LOG(LogTemp, Warning, TEXT("Stun"));
	}
	else if (!bStunned && LeaperMovementStatus == ELeaperMovementStatus::EMS_Stun)
	{
		UE_LOG(LogTemp, Warning, TEXT("stun disabled"));
		if (bInDashSphere)
		{
			UE_LOG(LogTemp, Warning, TEXT("stun to attack"));
			SetLeaperMovementStatus(ELeaperMovementStatus::EMS_Attacking);
			PrepareDash(CombatTarget);
			bInterp = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("stun to move"));
			MoveToTarget(CombatTarget);
			bInterp = true;
		}
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

/**
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
*/

/**
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
*/

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
				bInDashSphere = true;
				if (!bIsDashing && StunTime <= 0.f)
				{
					SetLeaperMovementStatus(ELeaperMovementStatus::EMS_Attacking);
					PrepareDash(CombatTarget);
				}
			}
		}
	}
}
void ALeaper::DashSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
			{
				bInDashSphere = false;
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
		MoveRequest.SetAcceptanceRadius(MoveToTolerance); // minmum distance between EnemyHitBox and TargetHitBox before the enemy stops moving

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

void ALeaper::PrepareDash(class ACreature* Target)
{
	bIsDashing = true;
	DashTargetLocation = Target->GetActorLocation();
	AIController->StopMovement();
	bInterp = false;
}

void ALeaper::Dash()
{
	FVector distance = DashTargetLocation - GetActorLocation();
	distance.Normalize();
	distance.Z = 0.1f;
	FVector force = distance * DashPower;
	//SetActorLocation(GetActorLocation() + (force * DeltaTime), true);
	GetCharacterMovement()->AddImpulse(force, true);
}


void ALeaper::AttackBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
				HitPlayer();

			UE_LOG(LogTemp, Warning, TEXT("AttackBoxOnOverlapBegin()"));
		}
	}
}

void ALeaper::HitPlayer()
{
	// cast TakeDMG function on player
	if (CombatTarget)
	{
		CombatTarget->TakeDMG(Damage);
	}
	UE_LOG(LogTemp, Warning, TEXT("HitPlayer()"));
}

void ALeaper::Aggro(ACreature* target)
{
	AIController = Cast<AAIController>(GetController());
	MoveToTarget(target);
}

void ALeaper::ActivateCollision()
{
	AttackBoxL->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackBoxR->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ALeaper::DeactivateCollision()
{
	AttackBoxL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackBoxR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
