// Fill out your copyright notice in the Description page of Project Settings.

#include "Grunt.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "AIModule.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bullet.h"


AGrunt::AGrunt()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
	AggroSphere->SetupAttachment(GetRootComponent());
	AggroSphere->InitSphereRadius(600.f);

	Ammo = MaxAmmo;
}

void AGrunt::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	AggroSphere->OnComponentBeginOverlap.AddDynamic(this, &AGrunt::AggroSphereOnOverlapBegin);

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

	if (GruntMovementStatus == EGruntMovementStatus::EMS_Attacking)
	{
		if (timeSinceLastShot >= shootingDelayFocused)
		{
			if (Ammo > 0)
			{
				FRotator Rotation = GetActorRotation();

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
			}
		}
		timeSinceLastShot += DeltaTime;
	}
}

FRotator AGrunt::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator YawLookAtRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
	return YawLookAtRotation;
}

void AGrunt::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGrunt::AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain1* Main = Cast<AMain1>(OtherActor);
		if (Main)
		{
			bInterpToTarget = true;
			CombatTarget = Main;
			SetGruntMovementStatus(EGruntMovementStatus::EMS_Attacking);
		}
	}
}