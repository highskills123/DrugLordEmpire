// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/EmpireTypes.h"
#include "Components/InventoryComponent.h"
#include "DistributionNode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaleCompleted, int32, CashEarned, float, HeatGenerated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNodeCompromised);

/**
 * World actor representing a distribution point (dealer, nightclub front, dead drop, etc.).
 *
 * Players load product into this node; it sells automatically over time.
 * Heat is generated proportionally to sales volume and visibility.
 */
UCLASS(BlueprintType, Blueprintable)
class EMPIREOFASH_API ADistributionNode : public AActor
{
    GENERATED_BODY()

public:
    ADistributionNode();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // ── Configuration ────────────────────────────────

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empire|Distribution")
    FName NodeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empire|Distribution")
    EDistributionNodeType NodeType = EDistributionNodeType::StreetDealer;

    /** District this node is located in */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empire|Distribution")
    FName DistrictID;

    /** How visible this node is to enforcement (0-1). Higher = more heat per sale. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empire|Distribution",
              meta = (ClampMin = 0.f, ClampMax = 1.f))
    float Visibility = 0.5f;

    /** Maximum product units sold per sale interval */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empire|Distribution",
              meta = (ClampMin = 1, ClampMax = 50))
    int32 MaxUnitsPerSale = 5;

    /** Interval between automatic sales (seconds) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empire|Distribution")
    float SaleIntervalSec = 30.f;

    // ── Product storage ──────────────────────────────

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Empire|Distribution")
    TObjectPtr<UInventoryComponent> ProductInventory;

    // ── Runtime state ────────────────────────────────

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Empire|Distribution")
    bool bIsActive = false;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Empire|Distribution")
    bool bIsCompromised = false;

    // ── Actions ──────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Empire|Distribution")
    void Activate();

    UFUNCTION(BlueprintCallable, Category = "Empire|Distribution")
    void Deactivate();

    UFUNCTION(BlueprintCallable, Category = "Empire|Distribution")
    void Compromise();

    UPROPERTY(BlueprintAssignable, Category = "Empire|Distribution")
    FOnSaleCompleted OnSaleCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Empire|Distribution")
    FOnNodeCompromised OnNodeCompromised;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    float TimeSinceLastSale = 0.f;

    void ProcessSale();
};
