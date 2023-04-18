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
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fish")
	float randomness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fish")
	float kP;

private:
	FVector velocity;
	FVector targetVelocity;

public:
	// Sets default values for this actor's properties
	AFishBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector getVelocity();
	FVector getTargetVelocity();

	void setTargetVelocity(FVector newTargetVelocity);
};
