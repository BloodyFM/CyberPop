// Fill out your copyright notice in the Description page of Project Settings.


#include "Main1.h"
#include "Creature.h"
#include "Weapon.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

AMain1::AMain1()
{
	bLeftMousePressed = false;
	bRightMousePressed = false;
	bSpecialPressed = false;

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
	if (DashCharge >= 30.f)
	{
		Dash();
	}
}

void AMain1::RightMouseReleased()
{
	bRightMousePressed = false;
}

void AMain1::SpecialPressed()
{
	bSpecialPressed = true;
	if (EquippedWeapon && bCanAttack)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		SpecialAttack();
		bCanAttack = false;
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

	}
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

