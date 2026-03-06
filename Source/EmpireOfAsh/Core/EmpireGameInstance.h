// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/EmpireTypes.h"
#include "EmpireGameInstance.generated.h"

class UEmpireManager;
class UHeatManager;
class UFactionManager;
class UMissionManager;

/**
 * Persistent game instance that survives map transitions.
 * Owns the empire-wide managers and player save profile.
 */
UCLASS(BlueprintType, Blueprintable)
class EMPIREOFASH_API UEmpireGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UEmpireGameInstance();

    // ── UGameInstance interface ─────────────────────
    virtual void Init() override;
    virtual void Shutdown() override;

    // ── Empire managers (server-authoritative) ──────

    UPROPERTY(BlueprintReadOnly, Category = "Empire|Managers")
    TObjectPtr<UEmpireManager> EmpireManager;

    UPROPERTY(BlueprintReadOnly, Category = "Empire|Managers")
    TObjectPtr<UHeatManager> HeatManager;

    UPROPERTY(BlueprintReadOnly, Category = "Empire|Managers")
    TObjectPtr<UFactionManager> FactionManager;

    UPROPERTY(BlueprintReadOnly, Category = "Empire|Managers")
    TObjectPtr<UMissionManager> MissionManager;

    // ── Save / Profile ───────────────────────────────

    /** Player display name */
    UPROPERTY(BlueprintReadWrite, Category = "Empire|Profile")
    FString PlayerDisplayName;

    /** Total empire cash (shared across team) */
    UPROPERTY(BlueprintReadWrite, Category = "Empire|Economy")
    int64 EmpireCash = 0;

    /** Total reputation points */
    UPROPERTY(BlueprintReadWrite, Category = "Empire|Economy")
    int32 Reputation = 0;

    UFUNCTION(BlueprintCallable, Category = "Empire|Economy")
    bool SpendCash(int64 Amount);

    UFUNCTION(BlueprintCallable, Category = "Empire|Economy")
    void AddCash(int64 Amount);

    // ── Session helpers ──────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Empire|Session")
    void SaveEmpireState();

    UFUNCTION(BlueprintCallable, Category = "Empire|Session")
    void LoadEmpireState();

private:
    void CreateManagers();
};
