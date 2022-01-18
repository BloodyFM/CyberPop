// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GruntAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STAGEGENERATOR_API UGruntAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
		void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		FVector2D MovementSpeed{ 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* Pawn{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class AGrunt* Enemy{ nullptr };
	
};
