// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Data/EmpireTypes.h"
#include "EmpirePlayerState.generated.h"

/**
 * Per-player replicated state: role, cash on person, heat contribution.
 */
UCLASS(BlueprintType, Blueprintable)
class EMPIREOFASH_API AEmpirePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AEmpirePlayerState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // ── Role ─────────────────────────────────────────

    UPROPERTY(ReplicatedUsing = OnRep_PlayerRole, BlueprintReadOnly, Category = "Empire|Role")
    EPlayerRole PlayerRole = EPlayerRole::None;

    UFUNCTION(BlueprintCallable, Category = "Empire|Role")
    void SetPlayerRole(EPlayerRole NewRole);

    UFUNCTION()
    void OnRep_PlayerRole();

    // ── Money on person (not team vault) ─────────────

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Empire|Economy")
    int32 CashCarried = 0;

    UFUNCTION(BlueprintCallable, Category = "Empire|Economy")
    bool TrySpendCarriedCash(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Empire|Economy")
    void AddCarriedCash(int32 Amount);

    // ── Heat contribution (this player's share) ───────

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Empire|Heat")
    float HeatContribution = 0.f;

    UFUNCTION(BlueprintCallable, Category = "Empire|Heat")
    void AddHeat(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Empire|Heat")
    void ReduceHeat(float Amount);

    // ── Downed / revive state ────────────────────────

    UPROPERTY(ReplicatedUsing = OnRep_IsDowned, BlueprintReadOnly, Category = "Empire|Status")
    bool bIsDowned = false;

    UFUNCTION(BlueprintCallable, Category = "Empire|Status")
    void SetDowned(bool bDowned);

    UFUNCTION()
    void OnRep_IsDowned();
};
