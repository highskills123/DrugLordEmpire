// Copyright (c) Empire of Ash. All Rights Reserved.

#include "Core/EmpireGameState.h"
#include "Net/UnrealNetwork.h"
#include "EmpireOfAsh.h"

AEmpireGameState::AEmpireGameState()
{
    bReplicates = true;
}

void AEmpireGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AEmpireGameState, DistrictStates);
    DOREPLIFETIME(AEmpireGameState, ActiveAlerts);
    DOREPLIFETIME(AEmpireGameState, TeamObjectives);
}

void AEmpireGameState::SetDistrictState(FName DistrictID, EDistrictControlState NewState)
{
    DistrictStates.Add(DistrictID, NewState);
    OnDistrictStateChanged.Broadcast(DistrictID);
    UE_LOG(LogEmpireOfAsh, Log, TEXT("District %s state changed to %d"), *DistrictID.ToString(), (uint8)NewState);
}

EDistrictControlState AEmpireGameState::GetDistrictState(FName DistrictID) const
{
    if (const EDistrictControlState* State = DistrictStates.Find(DistrictID))
    {
        return *State;
    }
    return EDistrictControlState::Neutral;
}

void AEmpireGameState::PushAlert(const FText& AlertMessage)
{
    ActiveAlerts.Add(AlertMessage);
    OnRep_ActiveAlerts();
}

void AEmpireGameState::ClearAlerts()
{
    ActiveAlerts.Empty();
}

void AEmpireGameState::OnRep_ActiveAlerts()
{
    if (ActiveAlerts.Num() > 0)
    {
        OnAlertTriggered.Broadcast(ActiveAlerts.Last());
    }
}

void AEmpireGameState::SetTeamObjectives(const TArray<FText>& NewObjectives)
{
    TeamObjectives = NewObjectives;
}
