// Copyright (c) Empire of Ash. All Rights Reserved.

#include "Managers/FactionManager.h"
#include "EmpireOfAsh.h"

UFactionManager::UFactionManager()
{
}

void UFactionManager::RegisterFaction(const FFactionRecord& Record)
{
    Factions.Add(Record.FactionID, Record);
    UE_LOG(LogEmpireOfAsh, Log, TEXT("Faction registered: %s (type %d)"),
           *Record.FactionID.ToString(), (uint8)Record.Type);
}

FFactionRecord UFactionManager::GetFactionRecord(FName FactionID) const
{
    if (const FFactionRecord* Record = Factions.Find(FactionID))
    {
        return *Record;
    }
    return FFactionRecord{};
}

void UFactionManager::SetFactionStance(FName FactionID, EFactionStance NewStance)
{
    if (FFactionRecord* Record = Factions.Find(FactionID))
    {
        Record->Stance = NewStance;
        OnFactionStanceChanged.Broadcast(FactionID, NewStance);
        UE_LOG(LogEmpireOfAsh, Log, TEXT("Faction %s stance -> %d"),
               *FactionID.ToString(), (uint8)NewStance);
    }
}

void UFactionManager::ModifyAggression(FName FactionID, float Delta)
{
    if (FFactionRecord* Record = Factions.Find(FactionID))
    {
        Record->Aggression = FMath::Clamp(Record->Aggression + Delta, 0.f, 100.f);
    }
}

void UFactionManager::EvaluateFactionActions()
{
    for (auto& Pair : Factions)
    {
        FFactionRecord& Faction = Pair.Value;
        if (Faction.Stance != EFactionStance::Hostile && Faction.Stance != EFactionStance::War)
        {
            continue;
        }

        // Independent probability rolls weighted by aggression (0-100)
        const float AggressionNorm = Faction.Aggression / 100.f;

        if (FMath::FRand() < AggressionNorm * 0.30f)
        {
            TriggerSabotage(Faction);
        }
        if (FMath::FRand() < AggressionNorm * 0.45f)
        {
            TriggerAmbush(Faction);
        }
        if (FMath::FRand() < AggressionNorm * 0.15f)
        {
            TriggerDealerBribe(Faction);
        }
    }
}

void UFactionManager::TriggerSabotage(const FFactionRecord& Faction)
{
    UE_LOG(LogEmpireOfAsh, Warning, TEXT("Faction %s is sabotaging a lab!"),
           *Faction.FactionID.ToString());
    // TODO: Notify EmpireManager to damage a random lab
}

void UFactionManager::TriggerAmbush(const FFactionRecord& Faction)
{
    UE_LOG(LogEmpireOfAsh, Warning, TEXT("Faction %s is ambushing a convoy!"),
           *Faction.FactionID.ToString());
    // TODO: Spawn ambush event on active smuggling route
}

void UFactionManager::TriggerDealerBribe(const FFactionRecord& Faction)
{
    UE_LOG(LogEmpireOfAsh, Warning, TEXT("Faction %s is bribing a dealer!"),
           *Faction.FactionID.ToString());
    // TODO: Notify EmpireManager to flip a distribution node
}
