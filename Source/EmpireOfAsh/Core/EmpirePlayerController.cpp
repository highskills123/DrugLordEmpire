// Copyright (c) Empire of Ash. All Rights Reserved.

#include "Core/EmpirePlayerController.h"
#include "Core/EmpirePlayerState.h"
#include "Managers/MissionManager.h"
#include "Core/EmpireGameInstance.h"
#include "EmpireOfAsh.h"

AEmpirePlayerController::AEmpirePlayerController()
{
}

void AEmpirePlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AEmpirePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    // Input bindings are handled through Enhanced Input in Blueprint subclasses.
}

// ── Role selection ────────────────────────────────────────────────────────────

void AEmpirePlayerController::Server_RequestRole_Implementation(EPlayerRole RequestedRole)
{
    if (AEmpirePlayerState* EmpirePS = GetPlayerState<AEmpirePlayerState>())
    {
        EmpirePS->SetPlayerRole(RequestedRole);
        Client_RoleConfirmed(RequestedRole);
    }
}

void AEmpirePlayerController::Client_RoleConfirmed_Implementation(EPlayerRole ConfirmedRole)
{
    UE_LOG(LogEmpireOfAsh, Log, TEXT("Role confirmed: %d"), (uint8)ConfirmedRole);
}

// ── Revive system ─────────────────────────────────────────────────────────────

void AEmpirePlayerController::Server_ReviveTeammate_Implementation(AEmpirePlayerController* DownedController)
{
    if (!DownedController)
    {
        return;
    }

    if (AEmpirePlayerState* DownedPS = DownedController->GetPlayerState<AEmpirePlayerState>())
    {
        if (DownedPS->bIsDowned)
        {
            DownedPS->SetDowned(false);
            UE_LOG(LogEmpireOfAsh, Log, TEXT("Player %s revived."), *DownedPS->GetPlayerName());
        }
    }
}

// ── Mission voting ────────────────────────────────────────────────────────────

void AEmpirePlayerController::Server_VoteForMission_Implementation(EMissionType MissionType)
{
    if (UEmpireGameInstance* GI = Cast<UEmpireGameInstance>(GetGameInstance()))
    {
        if (GI->MissionManager)
        {
            GI->MissionManager->RegisterVote(this, MissionType);
        }
    }
}
