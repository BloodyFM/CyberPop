// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature.h"
#include "Main1.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACreature::ACreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create CameraBoom (gets the camera closer to the player if obstructed)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;// Sets distance between Camera and player
	CameraBoom->bUsePawnControlRotation = true;// Rotate Camera based on controller

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

}

void ACreature::TakeDMG(float damage)
{
	hp -= damage;
}

// Called when the game starts or when spawned
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

// Called every frame
void ACreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (hp <= 0.f)
	{
		//Creature dies
		this->Destroy();
	}

}

// Called to bind functionality to input
void ACreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("LeftMouseAction", IE_Pressed, this, &ACreature::LeftMousePressed);
	PlayerInputComponent->BindAction("LeftMouseAction", IE_Released, this, &ACreature::LeftMouseReleased);

	PlayerInputComponent->BindAction("RightMouseAction", IE_Pressed, this, &ACreature::RightMousePressed);
	PlayerInputComponent->BindAction("RightMouseAction", IE_Released, this, &ACreature::RightMouseReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACreature::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACreature::MoveRight);


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