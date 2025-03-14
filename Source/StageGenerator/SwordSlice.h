// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "SwordSlice.generated.h"

/**
 * 
 */
UCLASS()
class STAGEGENERATOR_API ASwordSlice : public ABullet
{
	GENERATED_BODY()
public:

    ASwordSlice();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
        class UBoxComponent* SquareHitBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
        TArray<class ACreature*> HitEnemies;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
        float StunTime{ 5.f };

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "SpawnStunFx"))
        void SpawnStunFxForDurationOnTarget(float duration, FVector targetlocation);


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

    UFUNCTION()
        virtual void SquareBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
        virtual void SquareBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
