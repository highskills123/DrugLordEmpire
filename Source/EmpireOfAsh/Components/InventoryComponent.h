// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/EmpireTypes.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, FName, ItemID, int32, NewQuantity);

/**
 * Replicated inventory component attached to players and stash actors.
 *
 * Server-authoritative: all modifications go through server functions.
 * Anti-cheat: quantity and item ID are validated before any change.
 */
UCLASS(ClassGroup = (Empire), BlueprintType, Blueprintable,
       meta = (BlueprintSpawnableComponent))
class EMPIREOFASH_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // ── Inventory data ───────────────────────────────

    UPROPERTY(ReplicatedUsing = OnRep_Items, BlueprintReadOnly, Category = "Empire|Inventory")
    TArray<FInventoryItem> Items;

    UPROPERTY(BlueprintAssignable, Category = "Empire|Inventory")
    FOnInventoryChanged OnInventoryChanged;

    // ── Queries ──────────────────────────────────────

    UFUNCTION(BlueprintPure, Category = "Empire|Inventory")
    int32 GetItemQuantity(FName ItemID) const;

    UFUNCTION(BlueprintPure, Category = "Empire|Inventory")
    bool HasItem(FName ItemID, int32 RequiredQuantity = 1) const;

    UFUNCTION(BlueprintPure, Category = "Empire|Inventory")
    bool TryFindItem(FName ItemID, FInventoryItem& OutItem) const;

    // ── Modifications (server-authoritative) ─────────

    UFUNCTION(BlueprintCallable, Category = "Empire|Inventory")
    bool AddItem(const FInventoryItem& Item);

    UFUNCTION(BlueprintCallable, Category = "Empire|Inventory")
    bool RemoveItem(FName ItemID, int32 Quantity);

    UFUNCTION(BlueprintCallable, Category = "Empire|Inventory")
    void ClearInventory();

    UFUNCTION()
    void OnRep_Items();

private:
    int32 FindItemIndex(FName ItemID) const;
};
