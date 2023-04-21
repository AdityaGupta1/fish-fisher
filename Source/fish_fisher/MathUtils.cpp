// Fill out your copyright notice in the Description page of Project Settings.


#include "MathUtils.h"

FVector MathUtils::lerpConserveLength(const FVector& vec1, const FVector& vec2, float alpha)
{
    alpha = FMath::Clamp(alpha, 0, 1);

    float length1 = vec1.Size();
    float length2 = vec2.Size();
    float newLength = FMath::Lerp(length1, length2, alpha);

    FVector vec1Normalized = vec1.GetSafeNormal();
    FVector vec2Normalized = vec2.GetSafeNormal();
    FVector newDirection = FMath::Lerp(vec1Normalized, vec2Normalized, alpha);

    return newLength * newDirection;
}
