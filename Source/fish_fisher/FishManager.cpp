// Fill out your copyright notice in the Description page of Project Settings.


#include "FishManager.h"

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

    LOAD("/Script/Engine.Blueprint'/Game/Fishies/DebugFish.DebugFish_C'", "debug");
}

// Called when the game starts or when spawned
void AFishManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFishManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    for (const auto& fish : this->individualFish)
    {
        FVector targetVelocity = fish->getTargetVelocity();
        float targetSpeed = targetVelocity.Size();
        FVector noiseVec = FMath::VRand() * fish->randomness;
        FVector newTargetVelocity = (targetVelocity + noiseVec).GetSafeNormal() * targetSpeed;
        fish->setTargetVelocity(newTargetVelocity);
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
    fish->setTargetVelocity(lookDir * fish->speed);

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
    for (int i = 0; i < numFish; ++i)
    {
        schoolVector.push_back(spawnFish(pos, fishName));
    }
}
