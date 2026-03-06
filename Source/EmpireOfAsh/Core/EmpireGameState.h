// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/EmpireTypes.h"
#include "EmpireGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDistrictStateChanged, FName, DistrictID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlertTriggered, FText, AlertMessage);

/**
 * Replicated game state: district ownership, team objectives, active alerts.
 */
UCLASS(BlueprintType, Blueprintable)
class EMPIREOFASH_API AEmpireGameState : public AGameState
{
    GENERATED_BODY()

public:
    AEmpireGameState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // ── District state ───────────────────────────────

    /** Current district ownership map: DistrictID -> ControlState */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Empire|Districts")
    TMap<FName, EDistrictControlState> DistrictStates;

    UFUNCTION(BlueprintCallable, Authority, Category = "Empire|Districts")
    void SetDistrictState(FName DistrictID, EDistrictControlState NewState);

    UFUNCTION(BlueprintPure, Category = "Empire|Districts")
    EDistrictControlState GetDistrictState(FName DistrictID) const;

    UPROPERTY(BlueprintAssignable, Category = "Empire|Districts")
    FOnDistrictStateChanged OnDistrictStateChanged;

    // ── Active alerts ────────────────────────────────

    UPROPERTY(ReplicatedUsing = OnRep_ActiveAlerts, BlueprintReadOnly, Category = "Empire|Alerts")
    TArray<FText> ActiveAlerts;

    UFUNCTION(BlueprintCallable, Authority, Category = "Empire|Alerts")
    void PushAlert(const FText& AlertMessage);

    UFUNCTION(BlueprintCallable, Authority, Category = "Empire|Alerts")
    void ClearAlerts();

    UPROPERTY(BlueprintAssignable, Category = "Empire|Alerts")
    FOnAlertTriggered OnAlertTriggered;

    UFUNCTION()
    void OnRep_ActiveAlerts();

    // ── Team objectives ──────────────────────────────

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Empire|Mission")
    TArray<FText> TeamObjectives;

    UFUNCTION(BlueprintCallable, Authority, Category = "Empire|Mission")
    void SetTeamObjectives(const TArray<FText>& NewObjectives);
};
