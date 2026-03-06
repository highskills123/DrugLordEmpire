// Copyright (c) Empire of Ash. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/EmpireTypes.h"
#include "EmpirePlayerController.generated.h"

/**
 * Player controller for Empire of Ash.
 * Handles input routing, HUD management, and role-selection RPC calls.
 */
UCLASS(BlueprintType, Blueprintable)
class EMPIREOFASH_API AEmpirePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AEmpirePlayerController();

    // ── Role selection ───────────────────────────────

    /**
     * Request a role change from the client. The server validates availability.
     * @param RequestedRole  The role this player wants to take.
     */
    UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Empire|Role")
    void Server_RequestRole(EPlayerRole RequestedRole);

    /** Broadcasts to owning client when role is confirmed. */
    UFUNCTION(Client, Reliable, Category = "Empire|Role")
    void Client_RoleConfirmed(EPlayerRole ConfirmedRole);

    // ── Revive system ────────────────────────────────

    /**
     * Attempt to revive a downed teammate.
     * @param DownedController  The controller of the downed player.
     */
    UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Empire|Revive")
    void Server_ReviveTeammate(AEmpirePlayerController* DownedController);

    // ── Mission voting ───────────────────────────────

    /**
     * Cast this player's vote for the next mission.
     * @param MissionType  The mission type this player votes for.
     */
    UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Empire|Mission")
    void Server_VoteForMission(EMissionType MissionType);

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    void Server_RequestRole_Implementation(EPlayerRole RequestedRole);
    void Client_RoleConfirmed_Implementation(EPlayerRole ConfirmedRole);
    void Server_ReviveTeammate_Implementation(AEmpirePlayerController* DownedController);
    void Server_VoteForMission_Implementation(EMissionType MissionType);
};
