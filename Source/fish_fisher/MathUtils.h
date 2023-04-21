// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FISH_FISHER_API MathUtils
{
public:
    static FVector lerpConserveLength(const FVector& vec1, const FVector& vec2, float alpha);
};
