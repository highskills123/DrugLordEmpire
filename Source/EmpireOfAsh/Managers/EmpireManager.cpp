// Copyright (c) Empire of Ash. All Rights Reserved.

#include "Managers/EmpireManager.h"
#include "Core/EmpireGameInstance.h"
#include "EmpireOfAsh.h"

UEmpireManager::UEmpireManager()
{
}

bool UEmpireManager::UpgradeLab(FName LabID, int32 UpgradeCost)
{
    UEmpireGameInstance* GI = Cast<UEmpireGameInstance>(GetOuter());
    if (!GI || !GI->SpendCash(UpgradeCost))
    {
        return false;
    }

    int32& Tier = Labs.FindOrAdd(LabID, 0);
    if (Tier >= 3)
    {
        // Already max tier - refund
        GI->AddCash(UpgradeCost);
        return false;
    }

    Tier++;
    OnLabUpgraded.Broadcast(LabID, Tier);
    UE_LOG(LogEmpireOfAsh, Log, TEXT("Lab %s upgraded to tier %d."), *LabID.ToString(), Tier);
    return true;
}

bool UEmpireManager::PurchaseFront(FName FrontID, EDistributionNodeType NodeType, int32 PurchaseCost)
{
    if (Fronts.Contains(FrontID))
    {
        return false;
    }

    UEmpireGameInstance* GI = Cast<UEmpireGameInstance>(GetOuter());
    if (!GI || !GI->SpendCash(PurchaseCost))
    {
        return false;
    }

    Fronts.Add(FrontID, NodeType);
    OnFrontPurchased.Broadcast(FrontID);
    UE_LOG(LogEmpireOfAsh, Log, TEXT("Front %s purchased (type %d)."), *FrontID.ToString(), (uint8)NodeType);
    return true;
}

bool UEmpireManager::AddRoute(FName RouteID, FName DestinationDistrictID)
{
    if (Routes.Contains(RouteID))
    {
        return false;
    }
    Routes.Add(RouteID, DestinationDistrictID);
    return true;
}

bool UEmpireManager::RemoveRoute(FName RouteID)
{
    return Routes.Remove(RouteID) > 0;
}

int32 UEmpireManager::CalculateSalePrice(EProductQuality Quality, float DistrictDemand, float DistrictWealth)
{
    // Base prices per quality tier
    static const int32 BasePrices[] = { 10, 50, 100, 200, 500 };
    const int32 BasePrice = BasePrices[FMath::Clamp((int32)Quality, 0, 4)];

    const float DemandMultiplier = FMath::GetMappedRangeValueClamped(
        FVector2f(0.f, 100.f), FVector2f(0.5f, 2.f), DistrictDemand);
    const float WealthMultiplier = FMath::GetMappedRangeValueClamped(
        FVector2f(0.f, 100.f), FVector2f(0.8f, 1.5f), DistrictWealth);

    return FMath::RoundToInt(BasePrice * DemandMultiplier * WealthMultiplier);
}

int32 UEmpireManager::GetLabTier(FName LabID) const
{
    if (const int32* Tier = Labs.Find(LabID))
    {
        return *Tier;
    }
    return 0;
}
