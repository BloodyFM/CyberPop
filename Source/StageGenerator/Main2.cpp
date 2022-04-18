// Fill out your copyright notice in the Description page of Project Settings.


#include "Main2.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"

AMain2::AMain2()
{

	LockOnSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LockOnSphere"));
	LockOnSphere->SetupAttachment(GetRootComponent());

	FistBoxL = CreateDefaultSubobject<UBoxComponent>(TEXT("FistBoxL"));
	FistBoxR = CreateDefaultSubobject<UBoxComponent>(TEXT("FistBoxR"));


	MagnetSpeed = 400.f;
	MovementSpeedTank = 600.f;

	FistBoxL->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftFistSocket"));
	FistBoxR->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightFistSocket"));
}

void AMain2::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeedTank; // Sets Movement Speed

	LockOnSphere->OnComponentBeginOverlap.AddDynamic(this, &AMain2::LockOnSphereOverlapBegin);
	LockOnSphere->OnComponentEndOverlap.AddDynamic(this, &AMain2::LockOnSphereOverlapEnd);

	LockOnList.Empty();

}

void AMain2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMain2::LockOnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Creature = Cast<ACreature>(OtherActor);
		if (Creature)
		{
			if (!Creature->bIsMainCharacter)
			{
				UE_LOG(LogTemp, Warning, TEXT("Added possible target."));
				LockOnList.Add(Creature);
			}
		}
	}
}

void AMain2::LockOnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ACreature* Creature = Cast<ACreature>(OtherActor);
		if (Creature)
		{
			LockOnList.Remove(Creature);
		}
	}
}

void AMain2::RemoveLockOnTarget(ACreature* Creature)
{
	if (LockOnList.Contains(Creature))
	{
		LockOnList.Remove(Creature);
	}
}

ACreature* AMain2::FindBestLockOnTarget()
{
	FRotator ForwardRotator = GetActorRotation();
	ACreature* BestMatch = nullptr;

	//FRotator LookAtYaw = GetLookAtRotationYaw(Bestmatch->GetActorLocation());

	float CurrentAngle = 1000;

	for (int i = 0; i < LockOnList.Num(); i++)
	{
		float NewAngle;
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockOnList[i]->GetActorLocation());
		FRotator YawLookAtRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);

		NewAngle = YawLookAtRotation.Yaw - ForwardRotator.Yaw;

		if (NewAngle > 0)
		{
			NewAngle = -NewAngle;

		}
		if (CurrentAngle > NewAngle)
		{
			CurrentAngle = NewAngle;
			BestMatch = LockOnList[i];
		}

	}
	return BestMatch;
}

void AMain2::LockOnNotify()
{
	LockOnTarget = FindBestLockOnTarget();
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

FRotator AMain2::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator YawLookAtRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
	return YawLookAtRotation;
}