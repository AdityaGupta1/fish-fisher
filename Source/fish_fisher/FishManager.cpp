// Fill out your copyright notice in the Description page of Project Settings.


#include "FishManager.h"

#include "MathUtils.h"

#include "Kismet/KismetMathLibrary.h"

#define LOAD(path, fishName) {\
    ConstructorHelpers::FObjectFinder<UClass> fishClass(TEXT(path));\
    if (fishClass.Object) {\
        fishies[FString(TEXT(fishName))] = fishClass.Object;\
    }\
}

// Sets default values
AFishManager::AFishManager()
    : fishies(), individualFish(), schoolsOfFish()
{
    PrimaryActorTick.bCanEverTick = true;

    fishies["invisible"] = AFishBase::StaticClass();

    LOAD("/Script/Engine.Blueprint'/Game/Fishies/DebugFish.DebugFish_C'", "debug")
    LOAD("/Script/Engine.Blueprint'/Game/Fishies/InvisibleFish.InvisibleFish_C'", "invisible")
}

// Called when the game starts or when spawned
void AFishManager::BeginPlay()
{
    Super::BeginPlay();
}

void AFishManager::setIndividualFishRandomWalkVelocity(AFishBase& fish)
{
    FVector targetVelocity = fish.getTargetVelocity();
    float targetSpeed = targetVelocity.Size();
    FVector noiseVec = FMath::VRand() * fish.randomWalkError;
    FVector newTargetVelocity = (targetVelocity + noiseVec).GetSafeNormal() * targetSpeed;
    fish.setTargetVelocity(newTargetVelocity);
}

const float cAlignment = 0.2;
const float cSeparation = 50.0;
const float cCohesion = 0.2;
const float cArrival = 1.0;

// Called every frame
void AFishManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    for (const auto& fish : this->individualFish)
    {
        setIndividualFishRandomWalkVelocity(*fish);
    }

    for (const auto& school : this->schoolsOfFish) 
    {
        AFishBase* leader = school[0];
        setIndividualFishRandomWalkVelocity(*leader);

        FVector centerOfMass(0);
        FVector avgVelocity(0);
        for (const auto& fish : school) 
        {
            centerOfMass += fish->GetActorLocation();
            avgVelocity += fish->getTargetVelocity();
        }
        centerOfMass /= school.size();
        avgVelocity /= school.size();

        FVector vAlignment = cAlignment * avgVelocity;

        for (int i = 1; i < school.size(); ++i) 
        {
            AFishBase* fish = school[i];

            FVector repulsion(0);
            for (const auto& otherFish : school) 
            {
                FVector d = fish->GetActorLocation() - otherFish->GetActorLocation();
                repulsion += d / FMath::Max(d.SizeSquared(), 0.1);
            }
            FVector vSeparation = cSeparation * repulsion;

            FVector vCohesion = cCohesion * (centerOfMass - fish->GetActorLocation());

            FVector vArrival = cArrival * (leader->GetActorLocation() - fish->GetActorLocation());

            FVector newTargetVelocity = vAlignment + vSeparation + vCohesion + vArrival;
            fish->setTargetVelocity(MathUtils::lerpConserveLength(fish->getTargetVelocity(), newTargetVelocity, 30.0 * DeltaTime));
            setIndividualFishRandomWalkVelocity(*fish);
        }
    }
}

AFishBase* AFishManager::spawnFish(const FVector& pos, const FString& fishName)
{
    UWorld* world = this->GetWorld();
    if (!world)
    {
        return nullptr;
    }

    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;
    spawnParams.Instigator = this->GetInstigator();

    const FRotator rot = UKismetMathLibrary::RandomRotator(false);
    AFishBase* fish = world->SpawnActor<AFishBase>(fishies[fishName], pos, rot, spawnParams);

    const FVector lookDir = rot.RotateVector(FVector(1, 0, 0));
    fish->setTargetVelocity(lookDir * fish->getSpeed());

    return fish;
}

void AFishManager::spawnIndividualFish(FVector pos, const FString fishName)
{
    this->individualFish.push_back(spawnFish(pos, fishName));
}

void AFishManager::spawnSchoolOfFish(FVector pos, const FString fishName, int numFish)
{
    this->schoolsOfFish.push_back(std::vector<AFishBase*>());
    std::vector<AFishBase*>& schoolVector = this->schoolsOfFish.back();
    
    schoolVector.push_back(spawnFish(pos, "invisible"));

    for (int i = 0; i < numFish; ++i)
    {
        AFishBase* fish = spawnFish(pos + FMath::VRand() * 10.0, fishName);
        fish->setTargetVelocity(FVector::ZeroVector);
        schoolVector.push_back(fish);
    }
}
