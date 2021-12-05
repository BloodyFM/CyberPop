// Fill out your copyright notice in the Description page of Project Settings.


#include "Main1.h"
#include "Creature.h"
#include "Weapon.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/SkeletalMeshSocket.h"

AMain1::AMain1()
{
	bLeftMousePressed = false;
	bRightMousePressed = false;

	MovementSpeedDash = 600.f;
	DashDistance = 5000.f;
	DashCooldown = 0.5f;
	bCanDash = true;
	SpeedBeforeDash = FVector(0.f);
	DashStop = 0.1f;
	bDashing = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

}

void AMain1::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeedDash; // Sets Movement Speed
}

void AMain1::LeftMousePressed()
{
	//UE_LOG(LogTemp, Warning, TEXT("Left Mouse Pressed"));

	bLeftMousePressed = true;
	Attack();

}
void AMain1::LeftMouseReleased()
{
	bLeftMousePressed = false;
}

void AMain1::RightMousePressed()
{
	bRightMousePressed = true;
	Dash();
}

void AMain1::RightMouseReleased()
{
	bRightMousePressed = false;
}

void AMain1::Attack()
{

}

void AMain1::Dash()
{
	if (bCanDash)
	{
		bDashing = true;
		SpeedBeforeDash = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0.f) * MovementSpeedDash;
		GetCharacterMovement()->BrakingFrictionFactor = 0.f; // Removes friction
		LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0.1f) * DashDistance, true, true);
		bCanDash = false; // Disables Dash
		GetWorldTimerManager().SetTimer(DashHandle, this, &AMain1::StopDashing, DashStop, false); // Sets timer to stop dashing
	}
	UE_LOG(LogTemp, Warning, TEXT("Dash()"));
}

void AMain1::StopDashing()
{
	bDashing = false;
	GetCharacterMovement()->StopMovementImmediately();
	LaunchCharacter(SpeedBeforeDash, true, true);
	GetCharacterMovement()->BrakingFrictionFactor = 2.f; // Sets friction back to default
	GetWorldTimerManager().SetTimer(DashHandle, this, &AMain1::ResetDash, DashCooldown, false); // Sets timer to Enable dash again
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

