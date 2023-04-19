// Fill out your copyright notice in the Description page of Project Settings.


#include "FishBase.h"

#include "Kismet/KismetMathLibrary.h"

const float xyPlaneFactor = 0.5;

const float groundZ = 0;
const float surfaceZ = 1000;
const float zErrorZone = 50;
const float zCorrectionAdditionalXYPlaneFactor = 3.0;
const float zCorrectionAdditionalZVelocity = 1.0;

const float kPMultiplierMax = 2.5;
const float kPMultiplierDecay = 0.6;

const float dartSpeedMultiplier = 1.3;

AFishBase::AFishBase()
    : baseSpeed(7.0), randomWalkError(0.3), kP(1.8), dartTimerMin(8.0), dartTimerMax(16.0), velocity(), targetVelocity(),
    speed(baseSpeed* FMath::RandRange(0.75, 1.25)), dartTimer(0.0), kPMultiplier(1.0)
{
    PrimaryActorTick.bCanEverTick = true;

    this->setDartTimer();
}
void AFishBase::BeginPlay()
{
    Super::BeginPlay();
}

FVector reorientVector(const FVector& source, const FVector& target)
{
    return target.GetSafeNormal() * source.Size();
}

void AFishBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const float kPFinal = this->kP * this->kPMultiplier * DeltaTime;

    this->kPMultiplier -= (this->kPMultiplier - 1.0) * kPMultiplierDecay;

    {
        float currentSpeed = this->getVelocity().Size();
        float targetSpeed = this->getTargetVelocity().Size();
        float newSpeed = currentSpeed + kPFinal * (targetSpeed - currentSpeed);

        FVector currentVelocityNormalized = this->getVelocity().GetSafeNormal();
        FVector targetVelocityNormalized = this->getTargetVelocity().GetSafeNormal();
        FVector newDirection = FMath::Lerp(currentVelocityNormalized, targetVelocityNormalized, kPFinal);

        this->velocity = newDirection * newSpeed;
    }

    FVector newTargetVelocityDir = getTargetVelocity().GetSafeNormal();

    // dart
    this->dartTimer -= DeltaTime;
    if (dartTimer <= 0)
    {
        this->setDartTimer();

        this->kPMultiplier = kPMultiplierMax;
        this->velocity *= dartSpeedMultiplier;

        FVector oldTargetVelocityDir = newTargetVelocityDir.GetSafeNormal();
        do
        {
            newTargetVelocityDir = UKismetMathLibrary::RandomUnitVectorInConeInRadians(oldTargetVelocityDir, FMath::DegreesToRadians(90));
        } while (FMath::Acos(oldTargetVelocityDir.Dot(newTargetVelocityDir)) < FMath::DegreesToRadians(45));
    }

    // bias target velocity upwards if near ground and downwards if near ocean surface
    const FVector currentPos = this->GetActorLocation();
    bool needsZCorrection = false;
    float normalizedDistanceToZBound = 0;
    float totalXYPlaneFactor = xyPlaneFactor;
    if (currentPos.Z < groundZ + zErrorZone)
    {
        needsZCorrection = true;
        normalizedDistanceToZBound = (currentPos.Z - groundZ) / zErrorZone;
    }
    else if (currentPos.Z > surfaceZ - zErrorZone)
    {
        needsZCorrection = true;
        normalizedDistanceToZBound = (surfaceZ - currentPos.Z) / zErrorZone;
    }

    if (needsZCorrection)
    {
        totalXYPlaneFactor += normalizedDistanceToZBound * zCorrectionAdditionalXYPlaneFactor;
    }

    // bias towards XY plane
    newTargetVelocityDir = FMath::Lerp(newTargetVelocityDir.GetSafeNormal(),
        newTargetVelocityDir.GetSafeNormal2D(), xyPlaneFactor * DeltaTime);

    // add additional vertical velocity for Z correction
    if (needsZCorrection)
    {
        newTargetVelocityDir = newTargetVelocityDir.GetSafeNormal()
            + FVector(0, 0, normalizedDistanceToZBound * zCorrectionAdditionalZVelocity * DeltaTime);
    }

    // TODO keep the fish from straying too far from center pos ((0, 0) to start but change to player position later)

    this->setTargetVelocity(reorientVector(this->getTargetVelocity(), newTargetVelocityDir));

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

float AFishBase::getSpeed()
{
    return speed;
}

void AFishBase::setDartTimer()
{
    this->dartTimer = FMath::FRandRange(this->dartTimerMin, this->dartTimerMax);
}
