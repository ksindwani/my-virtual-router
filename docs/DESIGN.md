# High-Level Design: Event-Driven Routing Engine

## 1. Overview
This router is designed as a synchronous, event-driven system that maintains an in-memory routing table. It follows a modular architecture where the **Core Logic** (Routing Engine) is decoupled from the **User Interface** (CLI) and **Data Management** (JSON Parsing).

## 2. Component Architecture

### 2.1 Data Store (`RoutingTable`)
 - **State Management**: Maintains two primary vectors: `routes` (Static routes) and `interfaces` (Link states).
 - **Lookup Engine**: Implements Longest Prefix Match (LPM) logic.
 - Filters lookup candidates by verifying `oper_state` from the `interfaces` vector.

### 2.2 CLI Controller (`RouterCLI`)
Manages the persistent interactive session.
 - **Command Dispatcher**: Parses input and maps commands (`explain-lookup`, `show-routes`, etc.) to specific logic.

### 2.3 Event Processor (`EventProcessor`)
 - **Logic Bridge**: Translates JSON-based event logs into state mutation calls.
 - **Mutation**: Handles `interface_state` toggles and dynamic `route_remove` operations.

## 3. Data Flow

 - **Initialization**: The application loads the initial state from `static_routes.json` and `interfaces.json`.
 - **User Interaction**: The user enters a command via the `runShell`.
 - **Event Trigger**: If `replay-events` is called, `EventProcessor` modifies the internal `RoutingTable` state synchronously.
 - **Validation**: Subsequent lookups check the updated `RoutingTable` state against the current `interfaces` status.

## 4. Design Principles
 - **Single Responsibility**: Each class (`RoutingTable`, `RouterCLI`, `EventProcessor`) handles one specific aspect of the application.
 - **Synchronous Execution**: The system is "Thread-Safe by Architecture," operating on a single thread to eliminate race conditions and avoid the need for complex mutex locking.
 - **Predictability**: State changes are only applied via explicit user commands, ensuring that testing scenarios are deterministic.

## 5. Technology Stack
C++ and JSON (nlohmann/json library) 