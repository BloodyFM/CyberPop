// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LeaperAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STAGEGENERATOR_API ULeaperAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed { 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class ALeaper* Enemy{ nullptr };
};
