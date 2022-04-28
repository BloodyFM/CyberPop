// Fill out your copyright notice in the Description page of Project Settings.


#include "Main2.h"
#include "Leaper.h"
#include "Grunt.h"
#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"


AMain2::AMain2()
{

	LockOnSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LockOnSphere"));
	LockOnSphere->SetupAttachment(GetRootComponent());

	FistBoxL = CreateDefaultSubobject<UBoxComponent>(TEXT("FistBoxL"));
	FistBoxR = CreateDefaultSubobject<UBoxComponent>(TEXT("FistBoxR"));

	ShieldBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldBox"));
	ShieldBox->SetupAttachment(GetRootComponent());

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(ShieldBox);

	bLeftMousePressed = false;
	bRightMousePressed = false;
	bSpecialPressed = false;

	MagnetSpeed = 400.f;
	MovementSpeedTank = 600.f;

	ShieldCharge = 90.f;

	ShieldCharge = ShieldChargeMax;

	ShieldChargeMinimum = 30.f;

	BulletChargeMax = 90.f;

	BulletCharge = 0.f;

	DrainRate = 2.f;

	ShieldDrainRateUp = 10.f;
	ShieldDrainRateDown = 20.f;

	bIsMainCharacter = true;

	bCanAttack = true;

	bNotAttacked = true;
	bAttack1Over = false;
	bAttack2Over = false;
	bAttack3Over = false;

	bShielding = false;

	Damage = 100.f;

	InvulnDuration = 1.f;

	//FistBoxL->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftFistSocket"));
	//FistBoxR->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightFistSocket"));
}

void AMain2::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeedTank; // Sets Movement Speed

	LockOnSphere->OnComponentBeginOverlap.AddDynamic(this, &AMain2::LockOnSphereOverlapBegin);
	LockOnSphere->OnComponentEndOverlap.AddDynamic(this, &AMain2::LockOnSphereOverlapEnd);

	FistBoxL->OnComponentBeginOverlap.AddDynamic(this, &AMain2::FistBoxOverlapBegin);
	FistBoxL->OnComponentEndOverlap.AddDynamic(this, &AMain2::FistBoxOverlapEnd);

	FistBoxR->OnComponentBeginOverlap.AddDynamic(this, &AMain2::FistBoxOverlapBegin);
	FistBoxR->OnComponentEndOverlap.AddDynamic(this, &AMain2::FistBoxOverlapEnd);

	ShieldBox->OnComponentBeginOverlap.AddDynamic(this, &AMain2::ShieldBoxOverlapBegin);
	ShieldBox->OnComponentEndOverlap.AddDynamic(this, &AMain2::ShieldBoxOverlapEnd);

	FistBoxL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FistBoxL->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	FistBoxL->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	FistBoxL->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	FistBoxR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FistBoxR->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	FistBoxR->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	FistBoxR->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ShieldBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShieldBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ShieldBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ShieldBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	LockOnList.Empty();

	FistBoxL->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftFistSocket"));
	FistBoxR->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightFistSocket"));

}

void AMain2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (LevelName != "CharSelect")
	{
		hp -= DeltaTime * DrainRate;
		if(bShielding)
		ShieldCharge -= DeltaTime * ShieldDrainRateDown;
		else
		ShieldCharge += DeltaTime * ShieldDrainRateUp;
	}
	if (ShieldCharge > ShieldChargeMax)
	{
		ShieldCharge = ShieldChargeMax;
	}
	if (ShieldCharge <= 0.f)
	{
		RightMouseReleased();
	}

	if (LockOnTarget)
	{
		//float radius = LockOnTarget->GetRootComponent().GetScaledCapsuleRadius();
		FVector Direction = LockOnTarget->GetActorLocation() - GetActorLocation();
		if (Direction.Size() > 90.f)
		{
			Direction.Normalize();
			SetActorLocation(GetActorLocation() + Direction * MagnetSpeed * DeltaTime, true);
		}
		FRotator LookAtYaw = GetLookAtRotationYaw(LockOnTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, 250.f);

		SetActorRotation(InterpRotation);
	}
}

void AMain2::LeftMousePressed()
{
	bLeftMousePressed = true;
	if (!bShielding && bCanAttack)
	{
		Attack();
		bCanAttack = false;
	}
}

void AMain2::LeftMouseReleased()
{
	bLeftMousePressed = false;
}

void AMain2::RightMousePressed()
{
	
	if (ShieldCharge >= ShieldChargeMinimum)
	{
	bShielding = true;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CombatMontage, 1.f);
	AnimInstance->Montage_JumpToSection(("Shield_2"), CombatMontage);
	ShieldOpacityOn();
	}

}
void AMain2::RightMouseReleased()
{

	bShielding = false;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(0.3f, CombatMontage);
	ShieldOpacityOff();
}

void AMain2::SpecialPressed()
{

}
void AMain2::SpecialReleased()
{

}

void AMain2::GiveHP()
{
	hp += 50.f;

	if (hp > maxHp)
	{
		hp = maxHp;
	}
}

void AMain2::GiveShield()
{
	ShieldCharge += 30.f;
	if (ShieldCharge > ShieldChargeMax)
	{
		ShieldCharge = ShieldChargeMax;
	}
}

void AMain2::GiveBullets()
{

}

void AMain2::Attack()
{
	bAttacking = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		if (bNotAttacked || bAttack3Over)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.f);
			AnimInstance->Montage_JumpToSection(("Attack"), CombatMontage);
			bNotAttacked = false;
			bAttack1Over = true;
			bAttack3Over = false;
		}
		else if (bAttack1Over)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.f);
			AnimInstance->Montage_JumpToSection(("Attack2"), CombatMontage);
			bAttack2Over = true;
			bAttack1Over = false;
		}
		else if (bAttack2Over)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.f);
			AnimInstance->Montage_JumpToSection(("Attack3"), CombatMontage);
			bAttack3Over = true;
			bAttack2Over = false;
		}

	}
}

void AMain2::RangedAttack()
{

}

void AMain2::FistBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ALeaper::StaticClass()) || OtherActor->IsA(AGrunt::StaticClass()))
	{
		ACreature* Enemy = Cast<ACreature>(OtherActor);
		if (!Enemy->bIsMainCharacter)
		{
			if (Enemy->getHp() - Damage <= 0.f)
			{
				GiveHP();
				GiveShield();
			}
			Enemy->TakeDMG(Damage);
		}
	}
}

void AMain2::FistBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMain2::ActivateCollision()
{
	FistBoxL->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FistBoxR->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMain2::DeactivateCollision()
{
	FistBoxL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FistBoxR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMain2::ShieldBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABullet* Bullet = Cast<ABullet>(OtherActor);
		//AGrunt* Grunt = Cast<AGrunt>(OtherActor);
		if (Bullet)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Bullet"));
			GiveBullets();
			Bullet->Destroy();
		}
	}
}

void AMain2::ShieldBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMain2::ActivateShield()
{
	ShieldBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UE_LOG(LogTemp, Warning, TEXT("Collision Activated"));
}

void AMain2::DeactivateShield()
{
	ShieldBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UE_LOG(LogTemp, Warning, TEXT("Collision Deactivated"));
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

		if (NewAngle < 0)
		{
			NewAngle = -NewAngle;

		}
		if (CurrentAngle > NewAngle && NewAngle <= 90.f)
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