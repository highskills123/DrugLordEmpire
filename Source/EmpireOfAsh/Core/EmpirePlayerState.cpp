// Copyright (c) Empire of Ash. All Rights Reserved.

#include "Core/EmpirePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "EmpireOfAsh.h"

AEmpirePlayerState::AEmpirePlayerState()
{
    bReplicates = true;
}

void AEmpirePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AEmpirePlayerState, PlayerRole);
    DOREPLIFETIME(AEmpirePlayerState, CashCarried);
    DOREPLIFETIME(AEmpirePlayerState, HeatContribution);
    DOREPLIFETIME(AEmpirePlayerState, bIsDowned);
}

void AEmpirePlayerState::SetPlayerRole(EPlayerRole NewRole)
{
    if (HasAuthority())
    {
        PlayerRole = NewRole;
        OnRep_PlayerRole();
    }
}

void AEmpirePlayerState::OnRep_PlayerRole()
{
    UE_LOG(LogEmpireOfAsh, Log, TEXT("Player %s role changed to %d"), *GetPlayerName(), (uint8)PlayerRole);
}

bool AEmpirePlayerState::TrySpendCarriedCash(int32 Amount)
{
    if (Amount <= 0 || CashCarried < Amount)
    {
        return false;
    }
    CashCarried -= Amount;
    return true;
}

void AEmpirePlayerState::AddCarriedCash(int32 Amount)
{
    if (Amount > 0)
    {
        CashCarried += Amount;
    }
}

void AEmpirePlayerState::AddHeat(float Amount)
{
    HeatContribution = FMath::Clamp(HeatContribution + Amount, 0.f, 100.f);
}

void AEmpirePlayerState::ReduceHeat(float Amount)
{
    HeatContribution = FMath::Clamp(HeatContribution - Amount, 0.f, 100.f);
}

void AEmpirePlayerState::SetDowned(bool bDowned)
{
    if (HasAuthority())
    {
        bIsDowned = bDowned;
        OnRep_IsDowned();
    }
}

void AEmpirePlayerState::OnRep_IsDowned()
{
    UE_LOG(LogEmpireOfAsh, Log, TEXT("Player %s downed state: %s"), *GetPlayerName(), bIsDowned ? TEXT("true") : TEXT("false"));
}
