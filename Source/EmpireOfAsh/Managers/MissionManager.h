// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/EmpireTypes.h"
#include "Core/EmpirePlayerController.h"
#include "MissionManager.generated.h"

USTRUCT(BlueprintType)
struct FMissionRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName MissionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMissionType Type = EMissionType::IngredientTheft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMissionStatus Status = EMissionStatus::Pending;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    /** Votes cast by player controllers (PlayerController -> MissionType they voted for) */
    UPROPERTY()
    TMap<TObjectPtr<AEmpirePlayerController>, EMissionType> Votes;

    /** Minimum players required to start (1-4) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1, ClampMax = 4))
    int32 MinPlayers = 1;

    /** Cash reward on success */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CashReward = 1000;

    /** Reputation reward on success */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ReputationReward = 50;

    /** Heat added on completion */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HeatOnComplete = 10.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionStarted, FName, MissionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionCompleted, FName, MissionID, bool, bSucceeded);

/**
 * Server-authoritative manager for co-op mission objectives, voting, and rewards.
 */
UCLASS(BlueprintType, Blueprintable)
class EMPIREOFASH_API UMissionManager : public UObject
{
    GENERATED_BODY()

public:
    UMissionManager();

    // ── Mission registry ─────────────────────────────

    UPROPERTY(BlueprintReadOnly, Category = "Empire|Missions")
    TMap<FName, FMissionRecord> Missions;

    UFUNCTION(BlueprintCallable, Category = "Empire|Missions")
    void RegisterMission(const FMissionRecord& Record);

    // ── Voting ───────────────────────────────────────

    /**
     * Register a player's mission vote. Once all connected players have voted,
     * the most-voted mission is launched automatically.
     */
    void RegisterVote(AEmpirePlayerController* Voter, EMissionType MissionType);

    // ── Mission lifecycle ────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Empire|Missions")
    bool StartMission(FName MissionID);

    UFUNCTION(BlueprintCallable, Category = "Empire|Missions")
    void CompleteMission(FName MissionID, bool bSucceeded);

    UFUNCTION(BlueprintPure, Category = "Empire|Missions")
    FMissionRecord GetActiveMission() const;

    UPROPERTY(BlueprintAssignable, Category = "Empire|Missions")
    FOnMissionStarted OnMissionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Empire|Missions")
    FOnMissionCompleted OnMissionCompleted;

private:
    FName ActiveMissionID = NAME_None;

    /** Current voting round: MissionType -> vote count */
    TMap<EMissionType, int32> VoteTally;

    /** Registered voters this round */
    TArray<TObjectPtr<AEmpirePlayerController>> Voters;

    void ResolveMissionVote();
    FName FindMissionByType(EMissionType Type) const;
};
