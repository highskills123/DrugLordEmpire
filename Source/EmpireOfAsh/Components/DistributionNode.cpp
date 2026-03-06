// Copyright (c) Empire of Ash. All Rights Reserved.

#include "Components/DistributionNode.h"
#include "Core/EmpireGameInstance.h"
#include "Managers/EmpireManager.h"
#include "Managers/HeatManager.h"
#include "Net/UnrealNetwork.h"
#include "EmpireOfAsh.h"

ADistributionNode::ADistributionNode()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    ProductInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("ProductInventory"));
}

void ADistributionNode::BeginPlay()
{
    Super::BeginPlay();
}

void ADistributionNode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsActive || bIsCompromised || !HasAuthority())
    {
        return;
    }

    TimeSinceLastSale += DeltaTime;
    if (TimeSinceLastSale >= SaleIntervalSec)
    {
        TimeSinceLastSale = 0.f;
        ProcessSale();
    }
}

void ADistributionNode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ADistributionNode, bIsActive);
    DOREPLIFETIME(ADistributionNode, bIsCompromised);
}

void ADistributionNode::Activate()
{
    bIsActive = true;
    TimeSinceLastSale = 0.f;
    UE_LOG(LogEmpireOfAsh, Log, TEXT("DistributionNode %s activated."), *NodeID.ToString());
}

void ADistributionNode::Deactivate()
{
    bIsActive = false;
    UE_LOG(LogEmpireOfAsh, Log, TEXT("DistributionNode %s deactivated."), *NodeID.ToString());
}

void ADistributionNode::Compromise()
{
    bIsCompromised = true;
    bIsActive = false;
    OnNodeCompromised.Broadcast();
    UE_LOG(LogEmpireOfAsh, Warning, TEXT("DistributionNode %s compromised!"), *NodeID.ToString());
}

void ADistributionNode::ProcessSale()
{
    // Sell the first available product stack
    if (ProductInventory->Items.Num() == 0)
    {
        return;
    }

    FInventoryItem& Item = ProductInventory->Items[0];
    const int32 SellQuantity = FMath::Min(Item.Quantity, MaxUnitsPerSale);

    UEmpireGameInstance* GI = Cast<UEmpireGameInstance>(GetGameInstance());
    if (!GI)
    {
        return;
    }

    // Default district stats; in a full implementation fetch from DistrictData
    const float DistrictDemand = 60.f;
    const float DistrictWealth = 50.f;

    const int32 PricePerUnit = UEmpireManager::CalculateSalePrice(Item.Quality, DistrictDemand, DistrictWealth);
    const int32 Revenue = PricePerUnit * SellQuantity;

    // Apply visibility modifier to heat
    const float HeatPerUnit = 1.5f * Visibility;
    const float HeatGenerated = HeatPerUnit * SellQuantity;

    GI->AddCash(Revenue);

    if (GI->HeatManager)
    {
        GI->HeatManager->AddHeat(HeatGenerated, FString::Printf(TEXT("Sale at %s"), *NodeID.ToString()));
    }

    ProductInventory->RemoveItem(Item.ItemID, SellQuantity);

    OnSaleCompleted.Broadcast(Revenue, HeatGenerated);

    UE_LOG(LogEmpireOfAsh, Log, TEXT("Node %s sold %dx %s for $%d (+%.1f heat)"),
           *NodeID.ToString(), SellQuantity, *Item.ItemID.ToString(), Revenue, HeatGenerated);
}
