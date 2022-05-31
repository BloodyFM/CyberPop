// Fill out your copyright notice in the Description page of Project Settings.


#include "Main2.h"
#include "Leaper.h"
#include "Grunt.h"
#include "Bullet.h"
#include "MC2Bullet.h"
#include "Boss1.h"
#include "Boss2.h"
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
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"


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

	BulletCharge = 90.f;

	DrainRate = 2.f;

	ShieldDrainRateUp = 10.f;
	ShieldDrainRateDown = 20.f;

	bIsMainCharacter = true;

	bCanAttack = true;
	bCanShoot = true;

	bNotAttacked = true;
	bAttack1Over = false;
	bAttack2Over = false;
	bAttack3Over = false;

	bShielding = false;

	bAiming = false;
	bShooting = false;

	Damage = 100.f;

	InvulnDuration = 1.f;

	//FistBoxL->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftFistSocket"));
	//FistBoxR->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightFistSocket"));

	ResetCombo = 0.f;
	ResetRate = 3.f;
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
	ShieldBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

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
		if (hp >= 1.f)
		{
			hp -= DeltaTime * DrainRate;
		}
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

	if (ResetCombo <= 4.f)
	{
		ResetCombo += DeltaTime * DrainRate;
		UE_LOG(LogTemp, Warning, TEXT("The drainrate is: %f"), ResetCombo);
	}
	else
	{
		bAttack3Over = true;
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
		bCanShoot = false;
		ResetCombo = 0.f;
	}
}

void AMain2::LeftMouseReleased()
{
	bLeftMousePressed = false;
}

void AMain2::RightMousePressed()
{
	
	if (!bShielding && bCanAttack && (ShieldCharge >= ShieldChargeMinimum))
	{
	bShielding = true;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CombatMontage, 1.f);
	AnimInstance->Montage_JumpToSection(("Shield_2"), CombatMontage);
	ShieldOpacityOn();

	bCanAttack = false;
	bCanShoot = false;
	}

}
void AMain2::RightMouseReleased()
{
	if (bShielding)
	{
		bShielding = false;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Stop(0.3f, CombatMontage);
		ShieldOpacityOff();
		bCanAttack = true;
		bCanShoot = true;
	}
	
}

void AMain2::SpecialPressed()
{
	bSpecialPressed = true;
	if (!bShielding && bCanAttack && BulletCharge >= 10.f)
	{
		bAiming = true;
		bShooting = true;
		bCanAttack = false;
		//GetCharacterMovement()->MaxWalkSpeed = 0.f;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.f);
			AnimInstance->Montage_JumpToSection(("Blaster_Idle"), CombatMontage);

			bSendOutBullet = true;
		}
		
	}
}
void AMain2::SpecialReleased()
{
	bSpecialPressed = false;
	if (bAiming)
	{
		bCanShoot = false;
		bAiming = false;
		RangedAttack();
	}
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
	BulletCharge += 5.f;
	if (BulletCharge > BulletChargeMax)
	{
		BulletCharge = BulletChargeMax;
	}
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
	bAttacking = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(("Blaster"), CombatMontage);

		bSendOutBullet = true;
	}
}

void AMain2::DeliverAoeDamage()
{
	// Set what actors to seek out from it's collision channel
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	// Ignore any specific actors
	TArray<AActor*> ignoreActors;
	// Ignore self or remove this line to not ignore any
	ignoreActors.Init(this, 1);

	// Array of actors that are inside the radius of the sphere
	TArray<AActor*> outActors;

	FVector sphereSpawnLocation = GetActorLocation();
	// Class that the sphere should hit against and include in the outActors array (Can be null)
	UClass* seekClass = ACreature::StaticClass();
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpawnLocation, AoeRange, traceObjectTypes, seekClass, ignoreActors, outActors);

	// Optional: Use to have a visual representation of the SphereOverlapActors
	//DrawDebugSphere(GetWorld(), sphereSpawnLocation, AoeRange, 12, FColor::Red, true, 10.0f);

	// Finally iterate over the outActor array
	for (AActor* overlappedActor : outActors) {
		//UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *overlappedActor->GetName());
		ACreature* target = Cast<ACreature>(overlappedActor);
		if (target)
		{
			if (target->hp <= Damage && !target->IFrameOn)
			{
				GiveHP();
			}
			target->TakeDMG(Damage);
		}
	}
}

void AMain2::BulletSpawn()
{
	if (bSendOutBullet && BulletCharge >= 10.f)
	{
		BulletCharge -= 10.f;
		FRotator Rotation = GetActorRotation();
		FTransform BulletTransform;
		FVector BulletDisplacement = GetActorForwardVector();
		BulletDisplacement = FVector(BulletDisplacement.Y, -BulletDisplacement.X, 0.f); //Rotates 90 degrees. (only works on that tho)
		BulletTransform.SetLocation(GetActorLocation() + (GetActorForwardVector() * 70.f) + (BulletDisplacement * 30.f) + FVector(0.f, 0.f, 100.f));
		BulletTransform.SetRotation(Rotation.Quaternion());
		BulletTransform.SetScale3D(FVector(1.f));

		GetWorld()->SpawnActor<AMC2Bullet>(MC2BulletClass, BulletTransform);

		BulletTransform.SetLocation(GetActorLocation() + (GetActorForwardVector() * 70.f) + (BulletDisplacement * -30.f) + FVector(0.f, 0.f, 100.f));
		BulletTransform.SetRotation(Rotation.Quaternion());
		BulletTransform.SetScale3D(FVector(1.f));
		GetWorld()->SpawnActor<AMC2Bullet>(MC2BulletClass, BulletTransform);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *GetActorForwardVector().ToString());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *BulletDisplacement.ToString());
	}

	bSendOutBullet = false;
}

void AMain2::FistBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ALeaper::StaticClass()) || OtherActor->IsA(AGrunt::StaticClass()) 
		|| OtherActor->IsA(ABoss1::StaticClass()) || OtherActor->IsA(ABoss2::StaticClass()))
	{
		ACreature* Enemy = Cast<ACreature>(OtherActor);
		if (!Enemy->bIsMainCharacter)
		{
			if (Enemy->getHp() - Damage <= 0.f)
			{
				GiveHP();
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
	UE_LOG(LogTemp, Warning, TEXT("something overlapped"));
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