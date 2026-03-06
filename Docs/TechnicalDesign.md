# Technical Design Document — Empire of Ash

**Engine:** Unreal Engine 5.7+  
**Coding standard:** Blueprint-first; C++ for backend / server-authoritative systems  
**Target platform:** Android (Snapdragon 7xx+)  
**Networking:** 4-player online co-op, dedicated server model

---

## 1. Module Structure

```
Source/
  EmpireOfAsh/
    EmpireOfAsh.h / .cpp       # Module entry point
    EmpireOfAsh.Build.cs       # Build rules

    Core/
      EmpireGameInstance       # Singleton — managers, save, economy
      EmpirePlayerController   # Input, HUD, RPC entry points
      EmpirePlayerState        # Role, carried cash, heat contribution, downed state
      EmpireGameState          # Replicated district map, team objectives, alerts

    Managers/
      EmpireManager            # Labs, fronts, routes, economy calculations
      HeatManager              # Enforcement heat 0-100, raid triggers
      FactionManager           # Rival AI stance and action selection
      MissionManager           # Co-op objectives, voting, rewards

    Components/
      InventoryComponent       # Replicated item storage (players + world actors)
      ProductionStation        # Contraband manufacturing world actor
      DistributionNode         # Sales point world actor

    Data/
      EmpireTypes.h            # All enums and structs (no .cpp needed)
```

---

## 2. Networking Architecture

### 2.1 Authority Model

All economy, inventory, heat, and faction state mutations are **server-authoritative**.

- Clients call `Server_*` RPCs for any state change request
- The server validates and applies the change
- Clients receive replicated updates via `DOREPLIFETIME` properties and `OnRep_*` callbacks

### 2.2 Anti-Cheat Points

| System | Protection |
|---|---|
| `UInventoryComponent` | Quantity and ItemID validated before any add/remove |
| `UEmpireGameInstance::SpendCash` | Negative amount guard; underflow guard |
| `UEmpireGameInstance::AddCash` | Positive amount guard |
| `AEmpirePlayerController::Server_RequestRole` | Role change only applied on server |
| `AProductionStation::StartProduction` | Ingredient and tier validation on server |
| `ADistributionNode::ProcessSale` | Sale processing only runs on server (`HasAuthority()`) |

### 2.3 Replication Summary

| Property | Owner | Replication |
|---|---|---|
| `EmpirePlayerState::PlayerRole` | Server | `ReplicatedUsing = OnRep_PlayerRole` |
| `EmpirePlayerState::CashCarried` | Server | `Replicated` |
| `EmpirePlayerState::HeatContribution` | Server | `Replicated` |
| `EmpirePlayerState::bIsDowned` | Server | `ReplicatedUsing = OnRep_IsDowned` |
| `EmpireGameState::DistrictStates` | Server | `Replicated` |
| `EmpireGameState::ActiveAlerts` | Server | `ReplicatedUsing = OnRep_ActiveAlerts` |
| `EmpireGameState::TeamObjectives` | Server | `Replicated` |
| `ProductionStation::bIsProducing` | Server | `Replicated` |
| `ProductionStation::ProductionProgress` | Server | `ReplicatedUsing = OnRep_Progress` |
| `DistributionNode::bIsActive` | Server | `Replicated` |
| `DistributionNode::bIsCompromised` | Server | `Replicated` |
| `InventoryComponent::Items` | Server | `ReplicatedUsing = OnRep_Items` |

---

## 3. Class Responsibilities

### `UEmpireGameInstance`
- Lives for the entire session (survives map transitions)
- Owns the four manager UObjects as subobjects
- Manages the shared empire cash and reputation
- Entry point for `SaveEmpireState()` / `LoadEmpireState()`

### `AEmpirePlayerController`
- Thin client-side entry point for input and RPCs
- Delegates all logic to server via `Server_*` functions
- Receives role confirmation and UI updates via `Client_*` functions

### `AEmpirePlayerState`
- Replicated per-player data visible to all clients
- Role, carried cash, personal heat contribution, downed state
- Used by UI to display team status

### `AEmpireGameState`
- Replicated team-wide state
- District ownership map (`TMap<FName, EDistrictControlState>`)
- Alert queue and team objectives
- Fires Blueprint-assignable delegates on changes

### `UEmpireManager`
- Tracks labs (ID → tier), fronts (ID → type), routes (ID → destination)
- Validates and applies cash spend for upgrades/purchases via `UEmpireGameInstance`
- Static `CalculateSalePrice()` used by `ADistributionNode`

### `UHeatManager`
- Single float `HeatValue` (0–100) representing team enforcement pressure
- Exposes `AddHeat()`, `ReduceHeat()`, `TickHeatDecay()`
- Fires `OnHeatLevelChanged` delegate when the tier changes
- Fires `OnRaidTriggered` and resets heat by 50 when threshold (default 90) is crossed

### `UFactionManager`
- Registry of `FFactionRecord` structs keyed by FactionID
- `EvaluateFactionActions()` called periodically; rolls against aggression to select an action (sabotage / ambush / dealer bribe)
- Fires `OnFactionStanceChanged` delegate

### `UMissionManager`
- Registry of `FMissionRecord` structs
- `RegisterVote()` accumulates player votes; resolves automatically at ≥2 votes
- `StartMission()` / `CompleteMission()` manage lifecycle
- On success: cash + reputation added via `UEmpireGameInstance`, heat added via `UHeatManager`

### `UInventoryComponent`
- Replicated `TArray<FInventoryItem>` Items
- `AddItem()` / `RemoveItem()` with quantity validation
- Used by both `AProductionStation` (ingredients) and `ADistributionNode` (product)

### `AProductionStation`
- Ticking actor; runs production timer on server
- `StartProduction()` validates recipe tier and consumes ingredients
- `FinishProduction()` rolls quality (affected by tier + contamination chance)
- Fires `OnProductionComplete` or `OnProductionFailed`

### `ADistributionNode`
- Ticking actor; sells product periodically on server
- `ProcessSale()` calculates revenue using `UEmpireManager::CalculateSalePrice()`
- Generates heat proportional to visibility and units sold
- Fires `OnSaleCompleted`; `Compromise()` disables the node

---

## 4. Data Tables

### RecipeData (UDataTable, row type FRecipeData)

Columns: RecipeID, DisplayName, OutputType, OutputQuantity, RequiredTier, Ingredients[], ProductionTimeSec, BaseContaminationChance, HeatPerBatch

### DistrictData (UDataTable, row type FDistrictData)

Columns: DistrictID, DisplayName, BaseDemand, WealthLevel, PolicePresence, GangPresence, ControlState

---

## 5. Save System (Planned)

Use `USaveGame` subclass `UEmpireSaveGame`:

```
UEmpireSaveGame
  EmpireCash: int64
  Reputation: int32
  Labs: TMap<FName, int32>
  Fronts: TMap<FName, EDistributionNodeType>
  Routes: TMap<FName, FName>
  DistrictStates: TMap<FName, EDistrictControlState>
  Factions: TArray<FFactionRecord>
```

Saved on: mission completion, empire mode screen exit, app background.
Loaded on: `UEmpireGameInstance::Init()`.

---

## 6. Mobile Performance Guidelines

| Area | Target |
|---|---|
| Draw calls (action mission) | < 200 |
| Polygon budget (scene) | < 500k tris |
| Texture streaming pool | 256 MB |
| Target frame rate | 60fps (Snapdragon 778G+) |
| Network tick rate | 30Hz server, 15Hz min client |

- Use Level of Detail (LOD) meshes for all character and building assets
- Pool all frequently spawned actors (projectiles, particles)
- Use Nanite only for static environment geometry if GPU supports it; otherwise use traditional LODs
- Avoid dynamic global illumination on mobile; use baked lighting + dynamic accent lights

---

## 7. Blueprint Integration Points

All C++ classes are `BlueprintType` / `Blueprintable`. Key Blueprint hooks:

| C++ | Blueprint Subclass | Purpose |
|---|---|---|
| `UEmpireGameInstance` | `BP_EmpireGameInstance` | Save/load UI, session handling |
| `AEmpirePlayerController` | `BP_PlayerController` | Enhanced Input, HUD widget management |
| `AEmpirePlayerState` | `BP_PlayerState` | Role icon display, downed overlay |
| `AEmpireGameState` | `BP_GameState` | District overlay HUD, alert popups |
| `UEmpireManager` | — | Called directly from BP_GameMode |
| `UHeatManager` | — | Heat bar widget binding |
| `UFactionManager` | — | Faction stance UI |
| `UMissionManager` | — | Mission vote widget |
| `UInventoryComponent` | — | Inventory widget binding |
| `AProductionStation` | `BP_ProductionStation` | Minigame interaction widget |
| `ADistributionNode` | `BP_DistributionNode` | Node status widget, placement |
