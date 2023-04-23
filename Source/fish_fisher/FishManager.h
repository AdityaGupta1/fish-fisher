// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FishBase.h"

#include <map>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FishManager.generated.h"

UCLASS(Blueprintable)
class FISH_FISHER_API AFishManager : public AActor
{
    GENERATED_BODY()

private:
    std::map<FString, TSubclassOf<class AFishBase>> fishies;

    std::vector<AFishBase*> individualFish;
    std::vector<std::vector<AFishBase*>> schoolsOfFish;

public:
    // Sets default values for this actor's properties
    AFishManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    void setIndividualFishRandomWalkVelocity(AFishBase& fish);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    AFishBase* spawnFish(const FVector& pos, const FString& fishName);

public:
    UFUNCTION(BlueprintCallable)
    void spawnIndividualFish(FVector pos, const FString fishName);
    
    UFUNCTION(BlueprintCallable)
    void spawnMultipleIndividualFish(FVector pos, const FString fishName, int numFish);

    UFUNCTION(BlueprintCallable)
    void spawnSchoolOfFish(FVector pos, const FString fishName, int numFish);
};
