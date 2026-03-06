// Copyright (c) Empire of Ash. All Rights Reserved.

#include "Managers/HeatManager.h"
#include "EmpireOfAsh.h"

UHeatManager::UHeatManager()
{
}

void UHeatManager::AddHeat(float Amount, const FString& Reason)
{
    HeatValue = FMath::Clamp(HeatValue + Amount, 0.f, 100.f);
    UE_LOG(LogEmpireOfAsh, Log, TEXT("Heat +%.1f (%s) -> %.1f"), Amount, *Reason, HeatValue);

    const EHeatLevel NewLevel = ComputeHeatLevel();
    if (NewLevel != CurrentHeatLevel)
    {
        CurrentHeatLevel = NewLevel;
        OnHeatLevelChanged.Broadcast(CurrentHeatLevel);
    }

    CheckAndTriggerRaid();
}

void UHeatManager::ReduceHeat(float Amount)
{
    HeatValue = FMath::Clamp(HeatValue - Amount, 0.f, 100.f);

    const EHeatLevel NewLevel = ComputeHeatLevel();
    if (NewLevel != CurrentHeatLevel)
    {
        CurrentHeatLevel = NewLevel;
        OnHeatLevelChanged.Broadcast(CurrentHeatLevel);
    }
}

void UHeatManager::TickHeatDecay(float DeltaTime)
{
    if (HeatValue > 0.f && CurrentHeatLevel < EHeatLevel::Medium)
    {
        ReduceHeat(PassiveDecayRate * DeltaTime);
    }
}

EHeatLevel UHeatManager::ComputeHeatLevel() const
{
    if (HeatValue >= 85.f) return EHeatLevel::Critical;
    if (HeatValue >= 65.f) return EHeatLevel::High;
    if (HeatValue >= 40.f) return EHeatLevel::Medium;
    if (HeatValue >= 10.f) return EHeatLevel::Low;
    return EHeatLevel::None;
}

void UHeatManager::CheckAndTriggerRaid()
{
    if (HeatValue >= RaidThreshold)
    {
        UE_LOG(LogEmpireOfAsh, Warning, TEXT("RAID TRIGGERED at heat %.1f!"), HeatValue);
        OnRaidTriggered.Broadcast();
        // After a raid the heat partially resets (enforcement satisfied for now)
        HeatValue = FMath::Max(0.f, HeatValue - 50.f);
        CurrentHeatLevel = ComputeHeatLevel();
        OnHeatLevelChanged.Broadcast(CurrentHeatLevel);
    }
}
