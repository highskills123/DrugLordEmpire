// Copyright (c) Empire of Ash. All Rights Reserved.

#include "Managers/MissionManager.h"
#include "Core/EmpirePlayerController.h"
#include "Core/EmpireGameInstance.h"
#include "Managers/HeatManager.h"
#include "EmpireOfAsh.h"

UMissionManager::UMissionManager()
{
}

void UMissionManager::RegisterMission(const FMissionRecord& Record)
{
    Missions.Add(Record.MissionID, Record);
}

void UMissionManager::RegisterVote(AEmpirePlayerController* Voter, EMissionType MissionType)
{
    if (!Voter || Voters.Contains(Voter))
    {
        return;
    }

    Voters.Add(Voter);
    int32& Tally = VoteTally.FindOrAdd(MissionType, 0);
    Tally++;

    UE_LOG(LogEmpireOfAsh, Log, TEXT("Vote cast for mission type %d. Total votes: %d"),
           (uint8)MissionType, Voters.Num());

    // Resolve when at least 2 players have voted (or all 4 have voted)
    if (Voters.Num() >= 2)
    {
        ResolveMissionVote();
    }
}

void UMissionManager::ResolveMissionVote()
{
    EMissionType WinningType = EMissionType::IngredientTheft;
    int32 HighVotes = 0;

    for (const auto& Pair : VoteTally)
    {
        if (Pair.Value > HighVotes)
        {
            HighVotes = Pair.Value;
            WinningType = Pair.Key;
        }
    }

    const FName MissionID = FindMissionByType(WinningType);
    if (!MissionID.IsNone())
    {
        StartMission(MissionID);
    }

    VoteTally.Empty();
    Voters.Empty();
}

bool UMissionManager::StartMission(FName MissionID)
{
    FMissionRecord* Record = Missions.Find(MissionID);
    if (!Record || Record->Status == EMissionStatus::InProgress)
    {
        return false;
    }

    Record->Status = EMissionStatus::InProgress;
    ActiveMissionID = MissionID;
    OnMissionStarted.Broadcast(MissionID);
    UE_LOG(LogEmpireOfAsh, Log, TEXT("Mission started: %s"), *MissionID.ToString());
    return true;
}

void UMissionManager::CompleteMission(FName MissionID, bool bSucceeded)
{
    FMissionRecord* Record = Missions.Find(MissionID);
    if (!Record)
    {
        return;
    }

    Record->Status = bSucceeded ? EMissionStatus::Succeeded : EMissionStatus::Failed;

    if (bSucceeded)
    {
        if (UEmpireGameInstance* GI = Cast<UEmpireGameInstance>(GetOuter()))
        {
            GI->AddCash(Record->CashReward);
            GI->Reputation += Record->ReputationReward;

            if (GI->HeatManager)
            {
                GI->HeatManager->AddHeat(Record->HeatOnComplete, TEXT("MissionComplete"));
            }
        }
    }

    ActiveMissionID = NAME_None;
    OnMissionCompleted.Broadcast(MissionID, bSucceeded);
    UE_LOG(LogEmpireOfAsh, Log, TEXT("Mission %s completed: %s"),
           *MissionID.ToString(), bSucceeded ? TEXT("SUCCESS") : TEXT("FAILED"));
}

FMissionRecord UMissionManager::GetActiveMission() const
{
    if (!ActiveMissionID.IsNone())
    {
        if (const FMissionRecord* Record = Missions.Find(ActiveMissionID))
        {
            return *Record;
        }
    }
    return FMissionRecord{};
}

FName UMissionManager::FindMissionByType(EMissionType Type) const
{
    for (const auto& Pair : Missions)
    {
        if (Pair.Value.Type == Type && Pair.Value.Status == EMissionStatus::Pending)
        {
            return Pair.Key;
        }
    }
    return NAME_None;
}
