// Fill out your copyright notice in the Description page of Project Settings.


#include "Main1.h"
#include "Creature.h"
#include "Weapon.h"
#include "SwordSlice.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/Decalcomponent.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"

AMain1::AMain1()
{

	LockOnSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LockOnSphere"));
	LockOnSphere->SetupAttachment(GetRootComponent()); 
	

	bLeftMousePressed = false;
	bRightMousePressed = false;
	bSpecialPressed = false;

	MagnetSpeed = 400.f;
	MovementSpeedDash = 600.f;
	DashDistance = 5000.f;
	DashCooldown = 0.1f;
	bCanDash = true;
	SpeedBeforeDash = FVector(0.f);
	DashStop = 0.1f;
	bDashing = false;

	DashCharge = 90.f;

	DashCharge = DashChargeMax;

	DrainRate = 2.f;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	bIsMainCharacter = true;

	bCanAttack = true;

	//LevelName = GetWorld()->GetMapName();

	bNotAttacked = true;
	bAttack1Over = false;
	bAttack2Over = false;
	bAttack3Over = false;

	bSendOutSwordSlice = false;
	bIsAiming = false;

	AimProjectile = CreateDefaultSubobject<UDecalComponent>("AimProjectile");
	AimProjectile->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/HUD/StickerArrows/Pink_Arrow_Decal_Mat.Pink_Arrow_Decal_Mat'"));
	if (DecalMaterialAsset.Succeeded())
	{
		AimProjectile->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	AimProjectile->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	AimProjectile->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	InvulnDuration = 1.f;
}

void AMain1::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeedDash; // Sets Movement Speed
	DashCharge = 90.f;

	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *LevelName);

	FTransform WeaponTransform;
	WeaponTransform.SetLocation(GetActorLocation());
	WeaponTransform.SetRotation(FRotator(0.f).Quaternion());
	WeaponTransform.SetScale3D(FVector(1.f));
	EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FTransform(GetActorLocation()));
	EquippedWeapon->Equip(this);
	//SetEquippedWeapon(nullptr);

	LockOnSphere->OnComponentBeginOverlap.AddDynamic(this, &AMain1::LockOnSphereOverlapBegin);
	LockOnSphere->OnComponentEndOverlap.AddDynamic(this, &AMain1::LockOnSphereOverlapEnd);

	LockOnList.Empty();

}


void AMain1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (LevelName != "CharSelect")
	{
		hp -= DeltaTime * DrainRate;
		DashCharge += DeltaTime * DrainRate;
	}
	if (DashCharge > DashChargeMax)
	{
		DashCharge = DashChargeMax;
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

	if (bIsAiming)
	{
		//AimProjectile->SetHiddenInGame(false);
		FHitResult Hit;
		bool HitResult = false;


		HitResult = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_WorldStatic), true, Hit);

		if (HitResult)
		{
			if (AimProjectile)
			{
				AimProjectile->SetHiddenInGame(false);
				if (FollowCamera)
				{
					FRotator AimDirection = FollowCamera->GetComponentRotation();
					AimDirection.Pitch = 0.f;
					SetActorRotation(AimDirection);
				}
			}
		}
	}

}

void AMain1::LeftMousePressed()
{
	//UE_LOG(LogTemp, Warning, TEXT("Left Mouse Pressed"));

	bLeftMousePressed = true;
	if (EquippedWeapon && bCanAttack)
	{
		Attack();
		bCanAttack = false;
	}
}
void AMain1::LeftMouseReleased()
{
	bLeftMousePressed = false;
}

void AMain1::RightMousePressed()
{
	bRightMousePressed = true;
	if (EquippedWeapon && bCanAttack && DashCharge >= 30.f)
	{
		bIsAiming = true;
	}
}

void AMain1::RightMouseReleased()
{
	bRightMousePressed = false;
	if (bIsAiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		SpecialAttack();
		bCanAttack = false;
	}
}

void AMain1::SpecialPressed()
{
	bSpecialPressed = true;
	if (DashCharge >= 30.f)
	{
		Dash();
	}

}

void AMain1::SpecialReleased()
{
	bSpecialPressed = false;
}

void AMain1::GiveHP()
{
	hp += 50.f;

	if (hp > maxHp)
	{
		hp = maxHp;
	}
}

void AMain1::GiveDash()
{
	DashCharge += 30.f;
	if (DashCharge > DashChargeMax)
	{
		DashCharge = DashChargeMax;
	}
}

void AMain1::Attack()
{
	bAttacking = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		if (bNotAttacked || bAttack3Over)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.f);
			AnimInstance->Montage_JumpToSection(("Attack3"), CombatMontage);
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
			AnimInstance->Montage_JumpToSection(("Attack"), CombatMontage);
			bAttack3Over = true;
			bAttack2Over = false;
		}

	}
	
}

void AMain1::SpecialAttack()
{
	bAttacking = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(("SpecialAttack"), CombatMontage);

		bSendOutSwordSlice = true;
		//AimProjectile->SetHiddenInGame(true);
	}
}

void AMain1::SwordSlice()
{
	if (bSendOutSwordSlice && DashCharge >= 30.f)
	{
		DashCharge -= 30.f;
		FRotator Rotation = GetActorRotation();
		FTransform SwordSliceTransform;
		SwordSliceTransform.SetLocation(GetActorLocation() + (GetActorForwardVector() * 10.f) + FVector(0.f, 0.f, 100.f));
		SwordSliceTransform.SetRotation(Rotation.Quaternion());
		SwordSliceTransform.SetScale3D(FVector(1.f));

		GetWorld()->SpawnActor<ASwordSlice>(SwordSliceClass, SwordSliceTransform);
	}

	bSendOutSwordSlice = false;
}

void AMain1::Dash()
{
	if (bCanDash)
	{
		IFrameOn = true;
		bDashing = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.f, 0.0f);
		SpeedBeforeDash = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0.f) * MovementSpeedDash;
		GetCharacterMovement()->BrakingFrictionFactor = 0.f; // Removes friction
		LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0.0f) * DashDistance, true, true);
		bCanDash = false; // Disables Dash
		GetWorldTimerManager().SetTimer(DashHandle, this, &AMain1::StopDashing, DashStop, false); // Sets timer to stop dashing

		DashCharge -= 30.f;
	}
	UE_LOG(LogTemp, Warning, TEXT("Dash()"));
}

void AMain1::StopDashing()
{
	bDashing = false;
	GetCharacterMovement()->StopMovementImmediately();
	LaunchCharacter(SpeedBeforeDash, true, true);
	GetCharacterMovement()->BrakingFrictionFactor = 2.f; // Sets friction back to default
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);
	GetWorldTimerManager().SetTimer(DashHandle, this, &AMain1::ResetDash, DashCooldown, false); // Sets timer to Enable dash again
	IFrameOn = false;
}

void AMain1::ResetDash()
{
	bCanDash = true; // Enables Dash
}

void AMain1::LockOnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

void AMain1::LockOnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

void AMain1::RemoveLockOnTarget(ACreature* Creature)
{
	if (LockOnList.Contains(Creature))
	{
		LockOnList.Remove(Creature);
	}
}

ACreature* AMain1::FindBestLockOnTarget()
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

void AMain1::LockOnNotify()
{
	LockOnTarget = FindBestLockOnTarget();
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

FRotator AMain1::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator YawLookAtRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
	return YawLookAtRotation;
}


void AMain1::Equip()
{
	/*WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	WeaponMesh->SetSimulatePhysics(false);

	const USkeletalMeshSocket* RightHandSocket = this->GetMesh()->GetSocketByName("RightHandSocket");
	if (RightHandSocket)
	{
		RightHandSocket->AttachActor(this, Char->GetMesh());
		Char->SetEquippedWeapon(this);
	}*/
}

