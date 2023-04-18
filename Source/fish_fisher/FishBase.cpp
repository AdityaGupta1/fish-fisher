// Fill out your copyright notice in the Description page of Project Settings.


#include "FishBase.h"

// Sets default values
AFishBase::AFishBase()
	: speed(7.0), randomness(0.3), kP(0.03), velocity(), targetVelocity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFishBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFishBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		float currentSpeed = this->getVelocity().Size();
		float targetSpeed = this->getTargetVelocity().Size();
		float newSpeed = currentSpeed + this->kP * (targetSpeed - currentSpeed);

		FVector currentVelocityNormalized = this->getVelocity().GetSafeNormal();
		FVector targetVelocityNormalized = this->getTargetVelocity().GetSafeNormal();
		FVector newDirection = FMath::Lerp(currentVelocityNormalized, targetVelocityNormalized, this->kP);

		this->velocity = newDirection * newSpeed;
	}

	float targetSpeed = this->getTargetVelocity().Size();
	FVector newTargetVelocityDir = FMath::Lerp(this->getTargetVelocity().GetSafeNormal(), this->getTargetVelocity().GetSafeNormal2D(), 0.005);
	// TODO bias target velocity upwards if near ground and downwards if near ocean surface
	// TODO randomly change target velocity by a cone of ~45 to ~90 degrees with a low probability (to cause sudden shifts in direction)
	this->setTargetVelocity(newTargetVelocityDir * targetSpeed);

	const FVector currentPos = this->GetActorLocation();
	const FVector newPos = currentPos + this->getVelocity() * this->speed * DeltaTime * FMath::FRandRange(0.85, 1.15);
	this->SetActorLocationAndRotation(newPos, this->getVelocity().Rotation());
}

FVector AFishBase::getVelocity()
{
	return this->velocity;
}

FVector AFishBase::getTargetVelocity()
{
	return this->targetVelocity;
}

void AFishBase::setTargetVelocity(FVector newTargetVelocity)
{
	this->targetVelocity = newTargetVelocity;
}