// Fill out your copyright notice in the Description page of Project Settings.

#include "Grunt.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "AIModule.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Bullet.h"


AGrunt::AGrunt()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RetreatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
	RetreatSphere->SetupAttachment(GetRootComponent());
	RetreatSphere->InitSphereRadius(600.f);

	Ammo = MaxAmmo;
}

void AGrunt::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	RetreatSphere->OnComponentBeginOverlap.AddDynamic(this, &AGrunt::RetreatSphereOnOverlapBegin);
	RetreatSphere->OnComponentEndOverlap.AddDynamic(this, &AGrunt::RetreatSphereOnOverlapEnd);

}

void AGrunt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterpToTarget && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, TurnRate);

		SetActorRotation(InterpRotation);
	}

	/**
	if (GruntMovementStatus == EGruntMovementStatus::EMS_Attacking)
	{
		if (timeSinceLastShot >= shootingDelayFocused)
		{
			if (Ammo > 0)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Yaw += FMath::RandRange(-inaccuracy, inaccuracy);

				FTransform BulletTransform;
				BulletTransform.SetLocation(GetActorLocation() + (GetActorForwardVector() * 10.f) + FVector(0.f, 0.f, 100.f));
				BulletTransform.SetRotation(Rotation.Quaternion());
				BulletTransform.SetScale3D(FVector(1.f));

				GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);
				Ammo -= 1;
				timeSinceLastShot = 0.f;
			}
			else
			{
				SetGruntMovementStatus(EGruntMovementStatus::EMS_Reload);
				timeSinceLastShot = 0.f;
			}
		}
		timeSinceLastShot += DeltaTime;
	}

	if (GruntMovementStatus == EGruntMovementStatus::EMS_Retreat)
	{
		if (timeSinceLastShot >= shootingDelaySpread)
		{
			if (Ammo > 0)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Yaw -= 10.f;

				FTransform BulletTransform;
				BulletTransform.SetLocation(GetActorLocation() + (GetActorForwardVector() * 10.f) + FVector(0.f, 0.f, 100.f));
				BulletTransform.SetRotation(Rotation.Quaternion());
				BulletTransform.SetScale3D(FVector(1.f));

				GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);
				Rotation.Yaw += 5.f;
				BulletTransform.SetRotation(Rotation.Quaternion());
				GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);
				Rotation.Yaw += 5.f;
				BulletTransform.SetRotation(Rotation.Quaternion());
				GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);
				Rotation.Yaw += 5.f;
				BulletTransform.SetRotation(Rotation.Quaternion());
				GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);
				Rotation.Yaw += 5.f;
				BulletTransform.SetRotation(Rotation.Quaternion());
				GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);

				Ammo -= 5;
				timeSinceLastShot = 0.f;
			}
			else
			{
				SetGruntMovementStatus(EGruntMovementStatus::EMS_Reload);
				timeSinceLastShot = 0.f;
				bInterpToTarget = false;
			}
		}
		timeSinceLastShot += DeltaTime;
		FVector Movement =  -(GetActorForwardVector() * MovementSpeed/2);
		SetActorLocation(GetActorLocation() + (Movement * DeltaTime), true);
	}

	if (GruntMovementStatus == EGruntMovementStatus::EMS_Reload)
	{
		timeSinceLastShot += DeltaTime;
		if (timeSinceLastShot >= ReloadDelay)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Ready to shoot"));
			Ammo = MaxAmmo;
			if (bEnemyToClose)
			{
				SetGruntMovementStatus(EGruntMovementStatus::EMS_Retreat);
			}
			else
			{
				SetGruntMovementStatus(EGruntMovementStatus::EMS_Attacking);
			}
			bInterpToTarget = true;
		}
	}
	*/
}

FRotator AGrunt::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator YawLookAtRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
	return YawLookAtRotation;
}

void AGrunt::ShootFocused()
{
	FRotator Rotation = GetActorRotation();
	//Rotation.Yaw += FMath::RandRange(-inaccuracy, inaccuracy);

	FTransform BulletTransform;
	BulletTransform.SetLocation(GetActorLocation() + (GetActorForwardVector() * 10.f) + FVector(0.f, 0.f, 20.f));
	BulletTransform.SetRotation(Rotation.Quaternion());
	BulletTransform.SetScale3D(FVector(1.f));

	GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);
	Ammo -= 1;
}

void AGrunt::ShootSpread()
{
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw -= 10.f;

	FTransform BulletTransform;
	BulletTransform.SetLocation(GetActorLocation() + (GetActorForwardVector() * 10.f) + FVector(0.f, 0.f, 20.f));
	BulletTransform.SetRotation(Rotation.Quaternion());
	BulletTransform.SetScale3D(FVector(1.f));

	GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);
	Rotation.Yaw += 5.f;
	BulletTransform.SetRotation(Rotation.Quaternion());
	GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);
	Rotation.Yaw += 5.f;
	BulletTransform.SetRotation(Rotation.Quaternion());
	GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);
	Rotation.Yaw += 5.f;
	BulletTransform.SetRotation(Rotation.Quaternion());
	GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);
	Rotation.Yaw += 5.f;
	BulletTransform.SetRotation(Rotation.Quaternion());
	GetWorld()->SpawnActor<ABullet>(BulletClass, BulletTransform);

	Ammo -= 5;
}

void AGrunt::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGrunt::RetreatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
			{
				bEnemyToClose = true;
			}
		}
	}
}

void AGrunt::RetreatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Focus"));
	if (OtherActor)
	{
		ACreature* Main = Cast<ACreature>(OtherActor);
		if (Main)
		{
			if (Main->bIsMainCharacter)
			{
				bEnemyToClose = false;
			}
		}
	}
}

void AGrunt::Aggro(ACreature* target)
{
	AIController = Cast<AAIController>(GetController());
	CombatTarget = target;
	bInterpToTarget = true;
}
