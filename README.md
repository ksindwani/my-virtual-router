# my-virtual-router

## 1. Cloning the Repository
Code is pushed to github, clone using below command:

```bash
git clone git@github.com:ksindwani/my-virtual-router.git
cd <repository-name>
```

## 1. Overview
This project implements network routing engine in C++. The router maintains an in-memory routing table and interface state, allowing for deterministic lookup operations using Longest Prefix Match (LPM) logic and an Interface Gatekeeper mechanism to validate routes based on operational status.

## 2. Key Features
* **Deterministic Lookups**: Validates paths using both LPM and real-time interface status checks.
* **Event-Driven Updates**: Dynamically adjust the router state (interface toggles, route removal) using JSON-based event logs.
* **CLI Interface**: Interactive shell for querying routes, interface status, and path resolution.
* **Data-Driven**: Startup configuration and runtime events are parsed from standard JSON formats.

## 3. Project Structure
```text
.
├── src/                # C++ source code (.cpp)
├── include/            # C++ header code (.hpp)
├── input/              # Configuration files (interfaces.json, static_routes.json, events.json)
├── json/               # nlohmann json.hpp file
├── Makefile            # Makefile
├── docs/
  ├── RUN.md              # Compilation and execution instructions with test logs
  ├── TEST.md             # Test cases and validation suite with sample run logs
  ├── DESIGN.md           # Test cases and validation suite with sample run logs
  ├── AI_USAGE.md         # AI prompts and response history in choronological order (I have added  prompts given by me to LLM for starting the implementation at the top with share link followed by complete chat)
└── README.md           # Project documentation