// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/EmpireTypes.h"
#include "EmpireManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLabUpgraded, FName, LabID, int32, NewTier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFrontPurchased, FName, FrontID);

/**
 * Server-authoritative manager for the empire economy:
 * production labs, distribution fronts, smuggling routes, and territory.
 */
UCLASS(BlueprintType, Blueprintable)
class EMPIREOFASH_API UEmpireManager : public UObject
{
    GENERATED_BODY()

public:
    UEmpireManager();

    // ── Labs ─────────────────────────────────────────

    /** Active labs: LabID -> current workstation tier (1-3) */
    UPROPERTY(BlueprintReadOnly, Category = "Empire|Labs")
    TMap<FName, int32> Labs;

    /**
     * Purchase or upgrade a production lab.
     * @param LabID       Unique identifier for the lab.
     * @param UpgradeCost Cash cost to upgrade.
     * @return true if the upgrade succeeded.
     */
    UFUNCTION(BlueprintCallable, Category = "Empire|Labs")
    bool UpgradeLab(FName LabID, int32 UpgradeCost);

    UPROPERTY(BlueprintAssignable, Category = "Empire|Labs")
    FOnLabUpgraded OnLabUpgraded;

    // ── Fronts ───────────────────────────────────────

    /** Purchased distribution fronts: FrontID -> EDistributionNodeType */
    UPROPERTY(BlueprintReadOnly, Category = "Empire|Fronts")
    TMap<FName, EDistributionNodeType> Fronts;

    /**
     * Purchase a new distribution front.
     * @param FrontID      Unique identifier.
     * @param NodeType     Type of front (dealer, nightclub, etc.).
     * @param PurchaseCost Cash cost.
     * @return true if successfully purchased.
     */
    UFUNCTION(BlueprintCallable, Category = "Empire|Fronts")
    bool PurchaseFront(FName FrontID, EDistributionNodeType NodeType, int32 PurchaseCost);

    UPROPERTY(BlueprintAssignable, Category = "Empire|Fronts")
    FOnFrontPurchased OnFrontPurchased;

    // ── Routes ───────────────────────────────────────

    /** Active smuggling routes: RouteID -> destination DistrictID */
    UPROPERTY(BlueprintReadOnly, Category = "Empire|Routes")
    TMap<FName, FName> Routes;

    UFUNCTION(BlueprintCallable, Category = "Empire|Routes")
    bool AddRoute(FName RouteID, FName DestinationDistrictID);

    UFUNCTION(BlueprintCallable, Category = "Empire|Routes")
    bool RemoveRoute(FName RouteID);

    // ── Revenue calculation ──────────────────────────

    /**
     * Calculate sale price for a product based on quality, district wealth, and demand.
     * @param Quality         Product quality tier.
     * @param DistrictDemand  0-100 demand level.
     * @param DistrictWealth  0-100 wealth level.
     * @return Calculated price per unit.
     */
    UFUNCTION(BlueprintPure, Category = "Empire|Economy")
    static int32 CalculateSalePrice(EProductQuality Quality, float DistrictDemand, float DistrictWealth);

private:
    int32 GetLabTier(FName LabID) const;
};
