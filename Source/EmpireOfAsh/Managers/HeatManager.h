// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/EmpireTypes.h"
#include "HeatManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeatLevelChanged, EHeatLevel, NewHeatLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRaidTriggered);

/**
 * Server-authoritative manager for the enforcement heat system.
 *
 * Heat accumulates from player actions (production, distribution, combat).
 * At critical levels it triggers raids, checkpoint sweeps, and asset seizures.
 * Heat decays passively over time and is reduced by laying low.
 */
UCLASS(BlueprintType, Blueprintable)
class EMPIREOFASH_API UHeatManager : public UObject
{
    GENERATED_BODY()

public:
    UHeatManager();

    // ── Heat value ───────────────────────────────────

    /** Global team heat: 0-100 */
    UPROPERTY(BlueprintReadOnly, Category = "Empire|Heat")
    float HeatValue = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Empire|Heat")
    EHeatLevel CurrentHeatLevel = EHeatLevel::None;

    UPROPERTY(BlueprintAssignable, Category = "Empire|Heat")
    FOnHeatLevelChanged OnHeatLevelChanged;

    UPROPERTY(BlueprintAssignable, Category = "Empire|Heat")
    FOnRaidTriggered OnRaidTriggered;

    // ── Modifiers ────────────────────────────────────

    /** Add heat from a player action. Recalculates level and may trigger a raid. */
    UFUNCTION(BlueprintCallable, Category = "Empire|Heat")
    void AddHeat(float Amount, const FString& Reason);

    /** Reduce heat (laying low, bribery, cleaning up evidence). */
    UFUNCTION(BlueprintCallable, Category = "Empire|Heat")
    void ReduceHeat(float Amount);

    /** Called on a timer to passively decay heat over time. */
    UFUNCTION(BlueprintCallable, Category = "Empire|Heat")
    void TickHeatDecay(float DeltaTime);

    /** Rate of passive heat decay per second when below Medium. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empire|Heat")
    float PassiveDecayRate = 0.5f;

    // ── Thresholds ───────────────────────────────────

    /** Heat threshold at which a raid is triggered (default: 90). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empire|Heat",
              meta = (ClampMin = 50.f, ClampMax = 100.f))
    float RaidThreshold = 90.f;

private:
    EHeatLevel ComputeHeatLevel() const;
    void CheckAndTriggerRaid();
};
