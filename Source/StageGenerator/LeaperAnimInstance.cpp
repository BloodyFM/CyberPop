// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaperAnimInstance.h"
#include "Leaper.h"


void ULeaperAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<ALeaper>(Pawn);
		}
	}
}

void ULeaperAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = Speed.Size();

		Enemy = Cast<ALeaper>(Pawn);
	}

}