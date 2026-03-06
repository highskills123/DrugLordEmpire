// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/EmpireTypes.h"
#include "FactionManager.generated.h"

USTRUCT(BlueprintType)
struct FFactionRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName FactionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionType Type = EFactionType::LocalGang;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionStance Stance = EFactionStance::Neutral;

    /** 0-100 aggression level. High aggression increases sabotage/ambush chance. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 100.f))
    float Aggression = 20.f;

    /** Districts this faction controls (IDs) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> ControlledDistricts;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFactionStanceChanged, FName, FactionID, EFactionStance, NewStance);

/**
 * Server-authoritative manager for rival faction AI behaviour.
 *
 * Factions can sabotage labs, steal product, ambush convoys, or bribe dealers
 * based on their stance and aggression level.
 */
UCLASS(BlueprintType, Blueprintable)
class EMPIREOFASH_API UFactionManager : public UObject
{
    GENERATED_BODY()

public:
    UFactionManager();

    // ── Faction registry ─────────────────────────────

    UPROPERTY(BlueprintReadOnly, Category = "Empire|Factions")
    TMap<FName, FFactionRecord> Factions;

    UFUNCTION(BlueprintCallable, Category = "Empire|Factions")
    void RegisterFaction(const FFactionRecord& Record);

    UFUNCTION(BlueprintPure, Category = "Empire|Factions")
    FFactionRecord GetFactionRecord(FName FactionID) const;

    // ── Stance management ────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Empire|Factions")
    void SetFactionStance(FName FactionID, EFactionStance NewStance);

    UPROPERTY(BlueprintAssignable, Category = "Empire|Factions")
    FOnFactionStanceChanged OnFactionStanceChanged;

    // ── Aggression ───────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Empire|Factions")
    void ModifyAggression(FName FactionID, float Delta);

    // ── AI actions ───────────────────────────────────

    /**
     * Evaluate each hostile faction and potentially trigger an attack.
     * Should be called periodically on the server.
     */
    UFUNCTION(BlueprintCallable, Category = "Empire|Factions")
    void EvaluateFactionActions();

private:
    void TriggerSabotage(const FFactionRecord& Faction);
    void TriggerAmbush(const FFactionRecord& Faction);
    void TriggerDealerBribe(const FFactionRecord& Faction);
};
