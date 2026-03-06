# Empire of Ash

> A 4-player co-op crime empire game for mobile where players build a contraband network, manufacture fictional illicit products, run risky distribution routes, evade elite task forces, and fight cartel rivals for control of the city.

---

## Overview

**Empire of Ash** is a UE5 mobile game (Android-first) combining top-down/third-person management and action gameplay for up to 4 players in co-op. Players arrive in a hostile city with nothing and must build a black-market empire dealing in **synthetic fictional compounds**, banned bio-serums, illegal stimulant tech, and cartel-controlled nightlife products.

---

## Genre & Platform

| Property | Value |
|---|---|
| Engine | Unreal Engine 5.7+ |
| Platform | Android (mobile-first) |
| Players | 1-4 co-op (online) |
| Camera | Top-down / isometric |
| Genre | Management-action hybrid |

---

## Core Gameplay Loop

1. **Start** with a tiny safehouse in a hostile district
2. **Gather** ingredients/components from suppliers and risky field runs
3. **Manufacture** contraband at mini-production facilities
4. **Distribute** through dealers, fronts, drones, runners, and hidden routes
5. **Evade** raids, surveillance, informants, and rival attacks
6. **Upgrade** territory, staff, vehicles, and protection
7. **Control** districts and grow into a citywide empire

---

## 4-Player Co-op Roles

| Role | Focus |
|---|---|
| **Boss** | Territory expansion, investments, pricing & risk strategy |
| **Chemist / Crafter** | Production minigames, purity/quality, advanced recipes |
| **Smuggler** | Deliveries, stealth/driving, stash movement between zones |
| **Enforcer** | Shipment protection, rival encounters, turf defense |

---

## Main Systems

### 1. Production System
- Raw material sourcing
- Workstation upgrades and recipe tiers
- Contamination/failure chance
- Output quality affects price, heat, and reputation

### 2. Distribution System
- Street dealers, nightclub fronts, dead drops, courier routes
- Hidden stash houses
- Premium vs. mass-market client segments

### 3. Heat System
- Enforcement pressure rises with visibility (cameras, witnesses, loud fights)
- High heat triggers raids, checkpoint sweeps, arrests, asset seizures

### 4. Rival Factions
- Local gangs, cartel-backed operators, corrupt officials, independent thieves
- Can sabotage labs, steal product, ambush convoys, buy your dealers

### 5. Territory Control
- Districts with demand, wealth, police presence, gang presence
- Take over corners, fronts, warehouses
- Each district modifies profitability and danger

### 6. Co-op Progression
- Shared empire account
- Personal skill trees per role
- Revive/downed teammate system
- Role synergy bonuses
- Mission voting before runs

---

## Game Mode Split

### A. Empire Mode (Persistent Metagame)
Manage labs, assign workers, buy fronts, upgrade routes, choose expansions.

### B. Action Missions (5-12 min co-op instances)
Ingredient theft, convoy escort, stash recovery, dealer rescue, sabotage rivals, avoid task force raids.

### C. Turf Events
Timed district takeovers, defend labs, sudden inspections, cartel retaliation.

---

## Project Structure

```
EmpireOfAsh.uproject
Source/
  EmpireOfAsh/
    Core/           # GameInstance, PlayerController, PlayerState, GameState
    Managers/       # EmpireManager, HeatManager, FactionManager, MissionManager
    Components/     # InventoryComponent, ProductionStation, DistributionNode
    Data/           # Structs, enums, DataTables for contraband, districts, recipes
Config/
  DefaultGame.ini
  DefaultEngine.ini
Docs/
  GDD.md
  TechnicalDesign.md
```

---

## Monetization

- Premium game purchase (no pay-to-win)
- Cosmetic safehouse skins
- Character outfits
- Vehicle skins

---

## Art Style

Stylized gritty neon city - low-poly / semi-realistic with strong silhouettes, night-heavy lighting, and readable UI with district heat overlays.

---

## Technical Architecture

Blueprint-first with C++ for backend systems. See [Docs/TechnicalDesign.md](Docs/TechnicalDesign.md) for the full class hierarchy.

Key C++ classes:

| Class | Responsibility |
|---|---|
| `UEmpireGameInstance` | Profile, save data, backend session |
| `AEmpirePlayerController` | Input, UI, role selection |
| `AEmpirePlayerState` | Role, money carried, heat contribution |
| `AEmpireGameState` | District state, team objectives, active alerts |
| `UEmpireManager` | Economy, labs, fronts, routes |
| `UHeatManager` | Wanted level / enforcement escalation |
| `UFactionManager` | Rival AI behavior |
| `UMissionManager` | Co-op objectives |
| `UInventoryComponent` | Item/product inventory |
| `AProductionStation` | Manufacturing minigame actor |
| `ADistributionNode` | Distribution point actor |
