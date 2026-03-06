// Copyright (c) Empire of Ash. All Rights Reserved.

#include "Core/EmpireGameInstance.h"
#include "Managers/EmpireManager.h"
#include "Managers/HeatManager.h"
#include "Managers/FactionManager.h"
#include "Managers/MissionManager.h"
#include "EmpireOfAsh.h"

UEmpireGameInstance::UEmpireGameInstance()
{
}

void UEmpireGameInstance::Init()
{
    Super::Init();
    CreateManagers();
    LoadEmpireState();
    UE_LOG(LogEmpireOfAsh, Log, TEXT("EmpireGameInstance initialised."));
}

void UEmpireGameInstance::Shutdown()
{
    SaveEmpireState();
    Super::Shutdown();
}

void UEmpireGameInstance::CreateManagers()
{
    EmpireManager  = NewObject<UEmpireManager>(this);
    HeatManager    = NewObject<UHeatManager>(this);
    FactionManager = NewObject<UFactionManager>(this);
    MissionManager = NewObject<UMissionManager>(this);
}

bool UEmpireGameInstance::SpendCash(int64 Amount)
{
    if (Amount <= 0 || EmpireCash < Amount)
    {
        return false;
    }
    EmpireCash -= Amount;
    return true;
}

void UEmpireGameInstance::AddCash(int64 Amount)
{
    if (Amount > 0)
    {
        EmpireCash += Amount;
    }
}

void UEmpireGameInstance::SaveEmpireState()
{
    // TODO: Serialise to USaveGame and write to disk / cloud
    UE_LOG(LogEmpireOfAsh, Log, TEXT("Empire state saved. Cash=%lld Rep=%d"), EmpireCash, Reputation);
}

void UEmpireGameInstance::LoadEmpireState()
{
    // TODO: Deserialise from USaveGame
    UE_LOG(LogEmpireOfAsh, Log, TEXT("Empire state loaded."));
}
