// Copyright (c) Empire of Ash. All Rights Reserved.

#include "Components/ProductionStation.h"
#include "Data/EmpireTypes.h"
#include "Net/UnrealNetwork.h"
#include "EmpireOfAsh.h"

AProductionStation::AProductionStation()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    IngredientInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("IngredientInventory"));
}

void AProductionStation::BeginPlay()
{
    Super::BeginPlay();
}

void AProductionStation::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsProducing || !HasAuthority())
    {
        return;
    }

    ElapsedTime += DeltaTime;
    ProductionProgress = FMath::Clamp(ElapsedTime / ProductionDuration, 0.f, 1.f);

    if (ElapsedTime >= ProductionDuration)
    {
        FinishProduction();
    }
}

void AProductionStation::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AProductionStation, WorkstationTier);
    DOREPLIFETIME(AProductionStation, bIsProducing);
    DOREPLIFETIME(AProductionStation, ActiveRecipeID);
    DOREPLIFETIME(AProductionStation, ProductionProgress);
}

bool AProductionStation::StartProduction(FName RecipeID, UDataTable* RecipeTable)
{
    if (bIsProducing || !RecipeTable)
    {
        return false;
    }

    const FRecipeData* Recipe = RecipeTable->FindRow<FRecipeData>(RecipeID, TEXT("StartProduction"));
    if (!Recipe)
    {
        UE_LOG(LogEmpireOfAsh, Warning, TEXT("Recipe %s not found in DataTable."), *RecipeID.ToString());
        return false;
    }

    if (Recipe->RequiredTier > WorkstationTier)
    {
        UE_LOG(LogEmpireOfAsh, Warning, TEXT("Recipe %s requires tier %d but station is tier %d."),
               *RecipeID.ToString(), Recipe->RequiredTier, WorkstationTier);
        return false;
    }

    // Validate ingredients
    for (const FIngredientCost& Cost : Recipe->Ingredients)
    {
        if (!IngredientInventory->HasItem(Cost.IngredientID, Cost.Quantity))
        {
            UE_LOG(LogEmpireOfAsh, Warning, TEXT("Missing ingredient: %s x%d"),
                   *Cost.IngredientID.ToString(), Cost.Quantity);
            return false;
        }
    }

    // Consume ingredients
    for (const FIngredientCost& Cost : Recipe->Ingredients)
    {
        IngredientInventory->RemoveItem(Cost.IngredientID, Cost.Quantity);
    }

    ActiveRecipeID = RecipeID;
    ProductionDuration = Recipe->ProductionTimeSec;
    ContaminationChance = FMath::Max(0.f, Recipe->BaseContaminationChance - (WorkstationTier - 1) * 0.05f);
    ElapsedTime = 0.f;
    ProductionProgress = 0.f;
    bIsProducing = true;

    UE_LOG(LogEmpireOfAsh, Log, TEXT("Production started: %s (%.0fs)"), *RecipeID.ToString(), ProductionDuration);
    return true;
}

void AProductionStation::CancelProduction()
{
    bIsProducing = false;
    ElapsedTime = 0.f;
    ProductionProgress = 0.f;
    ActiveRecipeID = NAME_None;
}

void AProductionStation::FinishProduction()
{
    bIsProducing = false;
    ElapsedTime = 0.f;

    const EProductQuality Quality = RollOutputQuality();

    if (Quality == EProductQuality::Contaminated)
    {
        OnProductionFailed.Broadcast(ActiveRecipeID);
        UE_LOG(LogEmpireOfAsh, Warning, TEXT("Production failed (contamination): %s"), *ActiveRecipeID.ToString());
    }
    else
    {
        OnProductionComplete.Broadcast(ActiveRecipeID, Quality);
        UE_LOG(LogEmpireOfAsh, Log, TEXT("Production complete: %s, Quality=%d"), *ActiveRecipeID.ToString(), (uint8)Quality);
    }

    ActiveRecipeID = NAME_None;
    ProductionProgress = 0.f;
}

EProductQuality AProductionStation::RollOutputQuality() const
{
    const float Roll = FMath::FRand();

    if (Roll < ContaminationChance)
    {
        return EProductQuality::Contaminated;
    }

    // Higher tier stations produce better quality
    const float QualityRoll = FMath::FRand();
    const float TierBonus = (WorkstationTier - 1) * 0.15f;

    if (QualityRoll > 0.9f - TierBonus) return EProductQuality::Premium;
    if (QualityRoll > 0.7f - TierBonus) return EProductQuality::High;
    if (QualityRoll > 0.4f - TierBonus) return EProductQuality::Standard;
    return EProductQuality::Low;
}

void AProductionStation::OnRep_Progress()
{
    // Notify UI on client for progress bar update
}
