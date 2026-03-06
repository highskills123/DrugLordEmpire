// Copyright (c) Empire of Ash. All Rights Reserved.

#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "EmpireOfAsh.h"

UInventoryComponent::UInventoryComponent()
{
    SetIsReplicatedByDefault(true);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UInventoryComponent, Items);
}

int32 UInventoryComponent::GetItemQuantity(FName ItemID) const
{
    const int32 Idx = FindItemIndex(ItemID);
    return Idx != INDEX_NONE ? Items[Idx].Quantity : 0;
}

bool UInventoryComponent::HasItem(FName ItemID, int32 RequiredQuantity) const
{
    return GetItemQuantity(ItemID) >= RequiredQuantity;
}

bool UInventoryComponent::TryFindItem(FName ItemID, FInventoryItem& OutItem) const
{
    const int32 Idx = FindItemIndex(ItemID);
    if (Idx != INDEX_NONE)
    {
        OutItem = Items[Idx];
        return true;
    }
    return false;
}

bool UInventoryComponent::AddItem(const FInventoryItem& Item)
{
    if (Item.ItemID.IsNone() || Item.Quantity <= 0)
    {
        return false;
    }

    const int32 Idx = FindItemIndex(Item.ItemID);
    if (Idx != INDEX_NONE)
    {
        Items[Idx].Quantity += Item.Quantity;
        OnInventoryChanged.Broadcast(Item.ItemID, Items[Idx].Quantity);
    }
    else
    {
        Items.Add(Item);
        OnInventoryChanged.Broadcast(Item.ItemID, Item.Quantity);
    }
    return true;
}

bool UInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
    if (ItemID.IsNone() || Quantity <= 0)
    {
        return false;
    }

    const int32 Idx = FindItemIndex(ItemID);
    if (Idx == INDEX_NONE || Items[Idx].Quantity < Quantity)
    {
        return false;
    }

    Items[Idx].Quantity -= Quantity;
    const int32 Remaining = Items[Idx].Quantity;

    if (Remaining <= 0)
    {
        Items.RemoveAt(Idx);
    }

    OnInventoryChanged.Broadcast(ItemID, Remaining);
    return true;
}

void UInventoryComponent::ClearInventory()
{
    Items.Empty();
}

void UInventoryComponent::OnRep_Items()
{
    // Notify UI on client
}

int32 UInventoryComponent::FindItemIndex(FName ItemID) const
{
    for (int32 i = 0; i < Items.Num(); ++i)
    {
        if (Items[i].ItemID == ItemID)
        {
            return i;
        }
    }
    return INDEX_NONE;
}
