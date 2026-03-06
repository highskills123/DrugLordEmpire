# Game Design Document — Empire of Ash

**Version:** 0.1  
**Engine:** Unreal Engine 5.4+  
**Platform:** Android (mobile-first)  
**Players:** 1–4 co-op (online)

---

## 1. Vision Statement

Empire of Ash is a 4-player co-op mobile game in which players build a black-market empire from nothing. Arriving in a hostile city as small-time operators, the team manufactures and distributes fictional contraband, fends off rival factions, evades law enforcement task forces, and fights for district control.

The game rewards teamwork, strategic investment, and operational discipline. Every decision carries risk — production generates heat, distribution attracts attention, and rivals constantly probe for weakness.

---

## 2. Safe Reskin — Contraband Categories

All contraband is entirely fictional. No real-world substances are referenced.

| Category | In-Game Name Examples |
|---|---|
| Synthetic Compound | "Vex", "Phase-7", "Nullite" |
| Illegal Stimulant Tech | "Boost Chips", "Neuro-Spark rigs" |
| Banned Bio-Serum | "Surge Serum", "Delta-Bind" |
| Contraband Street Chemical | "Ash", "Glitter-K", "Smoke-9" |
| Cartel Nightlife Product | "Fade", "Echo Tabs", "Crystal Haze" |

---

## 3. Core Gameplay Loop

```
SAFEHOUSE → GATHER → MANUFACTURE → DISTRIBUTE → EVADE → UPGRADE → EXPAND
    ↑______________________________________________________________|
```

### 3.1 Empire Mode (Persistent Metagame)

Between action missions, players manage their empire from the safehouse hub:

- Assign workers to labs
- Review supply chains and restock ingredients
- Buy new distribution fronts
- Upgrade routes and protection
- Choose territory expansion targets
- Review heat level and plan cool-down activities

### 3.2 Action Missions (5–12 min instances)

Short co-op missions that feed the empire:

| Mission | Objective |
|---|---|
| Ingredient Theft | Steal raw materials from a rival or supplier |
| Convoy Escort | Protect a product shipment to its destination |
| Stash Recovery | Retrieve a seized/hidden stash |
| Dealer Rescue | Extract a compromised dealer before capture |
| Sabotage Rivals | Destroy rival production or distribution assets |
| Avoid Task Force Raid | Escape before enforcement sweeps the zone |

### 3.3 Turf Events (Reactive / Scheduled)

| Event | Trigger |
|---|---|
| Timed District Takeover | Scheduled window where contested districts are fought over |
| Defend Lab | Rival faction attacks an active production facility |
| Sudden Inspection | Police presence spikes in a district |
| Cartel Retaliation | A rival cartel launches a large-scale assault |

---

## 4. Player Roles

### Boss
- Opens territory expansion options
- Sets pricing tiers and risk levels
- Manages the empire bank
- Votes carry extra weight in mission selection

### Chemist / Crafter
- Operates production stations via minigames
- Improves product purity and quality over time
- Unlocks advanced recipe tiers
- Reduces contamination chance with skill

### Smuggler
- Handles product transport between zones
- Uses stealth, driving, and route-planning skills
- Manages secure stash movement
- Unlocks hidden route upgrades

### Enforcer
- Guards shipments and teammates
- Engages rival faction encounters
- Secures turf and fortifies defenses
- Revive speed bonus for downed teammates

---

## 5. Systems Design

### 5.1 Production System

**Workstation Tiers**

| Tier | Unlock Cost | Contamination Reduction | Quality Ceiling |
|---|---|---|---|
| 1 (Basic) | Free | — | Standard |
| 2 (Advanced) | $15,000 | -5% | High |
| 3 (Expert) | $50,000 | -10% | Premium |

**Quality → Price Multiplier**

| Quality | Price Multiplier |
|---|---|
| Contaminated | 0x (unsellable) |
| Low | 0.5x |
| Standard | 1.0x |
| High | 1.75x |
| Premium | 3.5x |

### 5.2 Distribution System

| Node Type | Revenue | Heat/Sale | Visibility |
|---|---|---|---|
| Street Dealer | Low | Medium | High |
| Nightclub Front | Medium | Low | Medium |
| Dead Drop | Low | Very Low | Low |
| Courier Route | Medium | Low | Low |
| Stash House | Passive | Very Low | Very Low |
| Premium Client | Very High | Medium | Low |

### 5.3 Heat System

Heat is a 0–100 value. It rises from player actions and decays passively below Medium.

| Level | Heat Range | Enforcement Response |
|---|---|---|
| None | 0–9 | No response |
| Low | 10–39 | Occasional patrols |
| Medium | 40–64 | Increased patrols, tip-offs |
| High | 65–84 | Checkpoints, surveillance |
| Critical | 85–100 | Raid imminent, asset seizure risk |

At 90+ heat, a raid is triggered. Heat resets by 50 points after a raid.

**Heat Sources**

- Producing contraband (+5 per batch base)
- Failed or noisy distribution (+2–10 per event)
- Street fights / gunfire (+15–25)
- Witnesses to criminal activity (+5–15)
- Rival tip-offs to police (+10–20)
- Repeated use of same routes (+2 per repeat)

**Heat Reduction**

- Laying low / time decay (-0.5/sec when below Medium)
- Bribery (-15 per bribe, costs cash)
- Evidence cleanup (-10 per scene)
- Using low-visibility routes

### 5.4 Rival Factions

| Faction | Behavior | Special Action |
|---|---|---|
| Local Gang | Territorial, reactive | Corner takeovers, dealer buyouts |
| Cartel Operator | Aggressive, funded | Lab sabotage, convoy ambush |
| Corrupt Official | Passive unless threatened | Police tip-off, checkpoint setup |
| Independent Thief | Opportunistic | Stash theft, product hijacking |

Faction aggression increases when players expand into their territory or attack their assets.

Stances: Neutral → Allied / Hostile → War

### 5.5 Territory Control

Districts have four dynamic properties:

| Property | Effect |
|---|---|
| Demand (0–100) | Scales sale volume |
| Wealth (0–100) | Scales unit price |
| Police Presence (0–100) | Affects heat gain rate |
| Gang Presence (0–100) | Affects rival event frequency |

Controlling a district grants permanent income bonuses and unlocks new distribution slots.

### 5.6 Co-op Progression

- Shared **Empire Account** for team currency and reputation
- Personal **Skill Trees** per role (unlocked with reputation)
- **Revive system**: downed players can be revived by any teammate within a 30s window
- **Role Synergy Bonuses**: active when all 4 roles are filled (e.g., +10% quality when Chemist + Boss both online)
- **Mission Voting**: all players vote before a mission starts; majority wins

---

## 6. Mobile UX Design

### 6.1 Camera
- Primary: top-down isometric
- Action mode: optional third-person follow camera

### 6.2 Input
- Virtual joystick (left) + action buttons (right)
- Context-sensitive interaction buttons
- Swipe gestures for map navigation in Empire mode

### 6.3 Session Structure

| Mode | Duration | Persistence |
|---|---|---|
| Empire Mode | Async / persistent | Always saved |
| Action Mission | 5–12 minutes | Rewards applied on completion |
| Turf Event | 10–20 minutes | District ownership updated |

---

## 7. Monetization

- **Premium purchase** (no free-to-play gate)
- **Cosmetics only** (never gameplay-affecting):
  - Safehouse skin packs
  - Character outfits per role
  - Vehicle skins for Smuggler vehicles
- **No pay-to-win empire boosts**

---

## 8. Art Direction

- **Style**: Stylized gritty neon city
- **Geometry**: Low-poly / semi-realistic with strong silhouettes
- **Lighting**: Night-heavy, neon accent colours
- **UI**: Minimal in action mode; rich overlay in empire mode with district heat maps
- **Performance target**: 60fps on mid-range Android (Snapdragon 7xx series)
