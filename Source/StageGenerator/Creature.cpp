// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature.h"
#include "Main1.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatSpawner.h"

// Sets default values
ACreature::ACreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create CameraBoom (gets the camera closer to the player if obstructed)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 800.f;// Sets distance between Camera and player
	CameraBoom->bUsePawnControlRotation = false;

	// Create FollowCamera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attatch the Camera to the end of the Boom and let the Boom Adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	maxHp = 100.f;
	hp = maxHp;

	MaxWalkSpeed = 400.f;
	JumpVelocity = 400.f;

	bIsMainCharacter = false;

	IFrameOn = false;

	InvulnTimer = 0.f;
	InvulnDuration = 0.f;

}

void ACreature::TakeDMG(float damage)
{
	if (IFrameOn != true && InvulnTimer >= InvulnDuration)
	{
		InvulnTimer = 0.f;
		hp -= damage;
		//UE_LOG(LogTemp, Warning, TEXT("Slap"));
		OnHit();
	}
	//hp -= damage;
	//UE_LOG(LogTemp, Warning, TEXT("Slap"));
}

void ACreature::ApplyStunn(float duration)
{
	if (StunTime < duration)
	{
		bStunned = true;
		StunTime = duration;
	}
}

/**void ACreature::OnHit_Implementation()
{

}*/

void ACreature::BeginPlay()
{
	Super::BeginPlay();

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character faces in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // ...at ths rotation rate
	GetCharacterMovement()->JumpZVelocity = JumpVelocity; // Jumping power I guess
	GetCharacterMovement()->AirControl = 0.5f; // Magic power that allows me to change direction in air
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed; // Sets Movement Speed
	
}

void ACreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (hp <= 0.f)
	{
		//Creature dies
		if (Master)
		{
			Master->RemoveServant(this);
		}
		this->Destroy();
	}
	if (bStunned)
	{
		StunTime -= DeltaTime;
	}
	if (StunTime <= 0.f)
	{
		bStunned = false;
		StunTime = 0.f;
	}
	if (InvulnTimer < InvulnDuration)
	{
		InvulnTimer += DeltaTime;
	}
}

// Called to bind functionality to input
void ACreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (LevelName != "CharSelect")
	{
		//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
		//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

		PlayerInputComponent->BindAction("LeftMouseAction", IE_Pressed, this, &ACreature::LeftMousePressed);
		PlayerInputComponent->BindAction("LeftMouseAction", IE_Released, this, &ACreature::LeftMouseReleased);

		PlayerInputComponent->BindAction("RightMouseAction", IE_Pressed, this, &ACreature::RightMousePressed);
		PlayerInputComponent->BindAction("RightMouseAction", IE_Released, this, &ACreature::RightMouseReleased);

		PlayerInputComponent->BindAction("SpecialAction", IE_Pressed, this, &ACreature::SpecialPressed);
		PlayerInputComponent->BindAction("SpecialAction", IE_Released, this, &ACreature::SpecialReleased);

		PlayerInputComponent->BindAxis("MoveForward", this, &ACreature::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ACreature::MoveRight);
	}

}

void ACreature::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// Finds out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACreature::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// Finds out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ACreature::LeftMousePressed()
{
}

void ACreature::LeftMouseReleased()
{
}

void ACreature::RightMousePressed()
{
}

void ACreature::RightMouseReleased()
{
}

void ACreature::SpecialPressed()
{

}
void ACreature::SpecialReleased()
{

}

void ACreature::Aggro(ACreature* target)
{
}