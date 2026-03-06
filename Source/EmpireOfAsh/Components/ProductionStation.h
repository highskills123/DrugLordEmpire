// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/EmpireTypes.h"
#include "Components/InventoryComponent.h"
#include "ProductionStation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionComplete, FName, RecipeID, EProductQuality, Quality);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProductionFailed, FName, RecipeID);

/**
 * World actor representing a contraband production workstation.
 *
 * The Chemist player interacts with this to start production minigames.
 * Output quality is influenced by the Chemist's skill, workstation tier,
 * and random contamination rolls.
 */
UCLASS(BlueprintType, Blueprintable)
class EMPIREOFASH_API AProductionStation : public AActor
{
    GENERATED_BODY()

public:
    AProductionStation();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // ── Configuration ────────────────────────────────

    /** Unique ID for this station (links to EmpireManager Labs map) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empire|Production")
    FName StationID;

    /** Current workstation tier (1-3), synced with EmpireManager */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Empire|Production")
    int32 WorkstationTier = 1;

    // ── Production state ─────────────────────────────

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Empire|Production")
    bool bIsProducing = false;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Empire|Production")
    FName ActiveRecipeID;

    /** Progress 0-1 */
    UPROPERTY(ReplicatedUsing = OnRep_Progress, BlueprintReadOnly, Category = "Empire|Production")
    float ProductionProgress = 0.f;

    // ── Ingredient storage ───────────────────────────

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Empire|Production")
    TObjectPtr<UInventoryComponent> IngredientInventory;

    // ── Actions ──────────────────────────────────────

    /**
     * Start production of the given recipe.
     * Validates that required ingredients are present and workstation tier is sufficient.
     * @param RecipeID  DataTable row name.
     * @param RecipeTable  Reference to the recipe DataTable asset.
     */
    UFUNCTION(BlueprintCallable, Category = "Empire|Production")
    bool StartProduction(FName RecipeID, UDataTable* RecipeTable);

    UFUNCTION(BlueprintCallable, Category = "Empire|Production")
    void CancelProduction();

    UPROPERTY(BlueprintAssignable, Category = "Empire|Production")
    FOnProductionComplete OnProductionComplete;

    UPROPERTY(BlueprintAssignable, Category = "Empire|Production")
    FOnProductionFailed OnProductionFailed;

    UFUNCTION()
    void OnRep_Progress();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    float ProductionDuration = 60.f;
    float ElapsedTime = 0.f;
    float ContaminationChance = 0.1f;

    void FinishProduction();
    EProductQuality RollOutputQuality() const;
};
