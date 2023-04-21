// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FishBase.generated.h"

UCLASS(Blueprintable)
class FISH_FISHER_API AFishBase : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fish")
    float baseSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fish")
    float randomWalkError;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fish")
    float kP;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fish")
    float dartTimerMin;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fish")
    float dartTimerMax;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fish")
    int pointValue;

private:
    FVector velocity;
    FVector targetVelocity;

    float speed;

    float dartTimer;

    float kPMultiplier;

public:
    // Sets default values for this actor's properties
    AFishBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    FVector getVelocity() const;
    FVector getTargetVelocity() const;
    
    void setTargetVelocity(FVector newTargetVelocity);

    float getSpeed() const;

private:
    void setDartTimer();
};
