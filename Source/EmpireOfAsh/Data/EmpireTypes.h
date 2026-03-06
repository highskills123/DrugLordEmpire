// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EmpireTypes.generated.h"

// ─────────────────────────────────────────────
//  Player Roles
// ─────────────────────────────────────────────

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
    None       UMETA(DisplayName = "None"),
    Boss       UMETA(DisplayName = "Boss"),
    Chemist    UMETA(DisplayName = "Chemist / Crafter"),
    Smuggler   UMETA(DisplayName = "Smuggler"),
    Enforcer   UMETA(DisplayName = "Enforcer")
};

// ─────────────────────────────────────────────
//  Contraband / Product Types
// ─────────────────────────────────────────────

UENUM(BlueprintType)
enum class EContrabandType : uint8
{
    None                UMETA(DisplayName = "None"),
    SynthCompound       UMETA(DisplayName = "Synthetic Compound"),
    StimulantTech       UMETA(DisplayName = "Illegal Stimulant Tech"),
    BioSerum            UMETA(DisplayName = "Banned Bio-Serum"),
    StreetChemical      UMETA(DisplayName = "Contraband Street Chemical"),
    NightlifeProduct    UMETA(DisplayName = "Cartel Nightlife Product")
};

UENUM(BlueprintType)
enum class EProductQuality : uint8
{
    Contaminated UMETA(DisplayName = "Contaminated"),
    Low          UMETA(DisplayName = "Low"),
    Standard     UMETA(DisplayName = "Standard"),
    High         UMETA(DisplayName = "High"),
    Premium      UMETA(DisplayName = "Premium")
};

// ─────────────────────────────────────────────
//  District
// ─────────────────────────────────────────────

UENUM(BlueprintType)
enum class EDistrictControlState : uint8
{
    Neutral     UMETA(DisplayName = "Neutral"),
    PlayerOwned UMETA(DisplayName = "Player Owned"),
    RivalOwned  UMETA(DisplayName = "Rival Owned"),
    Contested   UMETA(DisplayName = "Contested")
};

USTRUCT(BlueprintType)
struct FDistrictData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName DistrictID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    /** Base demand for contraband products (0-100) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 100))
    float BaseDemand = 50.f;

    /** Wealth level affects price multipliers (0-100) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 100))
    float WealthLevel = 50.f;

    /** Base police presence (0-100) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 100))
    float PolicePresence = 30.f;

    /** Base rival gang presence (0-100) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 100))
    float GangPresence = 20.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDistrictControlState ControlState = EDistrictControlState::Neutral;
};

// ─────────────────────────────────────────────
//  Production / Recipe
// ─────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FIngredientCost
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName IngredientID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 1;
};

USTRUCT(BlueprintType)
struct FRecipeData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName RecipeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EContrabandType OutputType = EContrabandType::SynthCompound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OutputQuantity = 1;

    /** Required workstation tier (1-3) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1, ClampMax = 3))
    int32 RequiredTier = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FIngredientCost> Ingredients;

    /** Production time in seconds */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProductionTimeSec = 60.f;

    /** Base contamination chance (0-1) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 1.f))
    float BaseContaminationChance = 0.1f;

    /** Heat generated per batch produced */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HeatPerBatch = 5.f;
};

// ─────────────────────────────────────────────
//  Inventory Item
// ─────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EContrabandType ContrabandType = EContrabandType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EProductQuality Quality = EProductQuality::Standard;
};

// ─────────────────────────────────────────────
//  Heat / Enforcement
// ─────────────────────────────────────────────

UENUM(BlueprintType)
enum class EHeatLevel : uint8
{
    None      UMETA(DisplayName = "No Heat"),
    Low       UMETA(DisplayName = "Low"),
    Medium    UMETA(DisplayName = "Medium"),
    High      UMETA(DisplayName = "High"),
    Critical  UMETA(DisplayName = "Critical - Raid Imminent")
};

// ─────────────────────────────────────────────
//  Faction
// ─────────────────────────────────────────────

UENUM(BlueprintType)
enum class EFactionType : uint8
{
    None             UMETA(DisplayName = "None"),
    LocalGang        UMETA(DisplayName = "Local Gang"),
    CartelOperator   UMETA(DisplayName = "Cartel-Backed Operator"),
    CorruptOfficial  UMETA(DisplayName = "Corrupt Official"),
    IndependentThief UMETA(DisplayName = "Independent Thief")
};

UENUM(BlueprintType)
enum class EFactionStance : uint8
{
    Neutral   UMETA(DisplayName = "Neutral"),
    Allied    UMETA(DisplayName = "Allied"),
    Hostile   UMETA(DisplayName = "Hostile"),
    War       UMETA(DisplayName = "At War")
};

// ─────────────────────────────────────────────
//  Mission
// ─────────────────────────────────────────────

UENUM(BlueprintType)
enum class EMissionType : uint8
{
    IngredientTheft   UMETA(DisplayName = "Ingredient Theft"),
    ConvoyEscort      UMETA(DisplayName = "Convoy Escort"),
    StashRecovery     UMETA(DisplayName = "Stash Recovery"),
    DealerRescue      UMETA(DisplayName = "Dealer Rescue"),
    SabotageRivals    UMETA(DisplayName = "Sabotage Rivals"),
    AvoidRaid         UMETA(DisplayName = "Avoid Task Force Raid"),
    TurfTakeover      UMETA(DisplayName = "Turf Takeover"),
    DefendLab         UMETA(DisplayName = "Defend Lab"),
    CartelRetaliation UMETA(DisplayName = "Cartel Retaliation")
};

UENUM(BlueprintType)
enum class EMissionStatus : uint8
{
    Pending    UMETA(DisplayName = "Pending Vote"),
    InProgress UMETA(DisplayName = "In Progress"),
    Succeeded  UMETA(DisplayName = "Succeeded"),
    Failed     UMETA(DisplayName = "Failed"),
    Abandoned  UMETA(DisplayName = "Abandoned")
};

// ─────────────────────────────────────────────
//  Distribution
// ─────────────────────────────────────────────

UENUM(BlueprintType)
enum class EDistributionNodeType : uint8
{
    StreetDealer    UMETA(DisplayName = "Street Dealer"),
    NightclubFront  UMETA(DisplayName = "Nightclub Front"),
    DeadDrop        UMETA(DisplayName = "Dead Drop"),
    CourierRoute    UMETA(DisplayName = "Courier Route"),
    StashHouse      UMETA(DisplayName = "Hidden Stash House"),
    PremiumClient   UMETA(DisplayName = "Premium Client")
};
