### Sample run
### Test cases

### TC-1 (Display interfaces)

Exeute below command.
```
./router-basic show-interfaces
```
Expected output: List all the interfaces from interfaces.json

```
$ ./router-basic show-interfaces
[DEBUG] Loaded eth0 (Admin: "up", Oper: "up") -> is_up: TRUE
[DEBUG] Loaded eth1 (Admin: "up", Oper: "down") -> is_up: FALSE
[DEBUG] Loaded eth2 (Admin: "up", Oper: "up") -> is_up: TRUE
Name           IP/Prefix           Status    RX        TX        
-----------------------------------------------------------------
eth0                               UP        1200      980       
eth1                               DOWN      0         0         
eth2                               UP        500       500       
```

### TC-2 (Display routes)

Exeute below command.
```
./router-basic show-routes
```

Expected output: List all the interfaces from interfaces.json

```
 $ ./router-basic show-routes
[DEBUG] Loaded eth0 (Admin: "up", Oper: "up") -> is_up: TRUE
[DEBUG] Loaded eth1 (Admin: "up", Oper: "down") -> is_up: FALSE
[DEBUG] Loaded eth2 (Admin: "up", Oper: "up") -> is_up: TRUE
Prefix              Next Hop       Type      
---------------------------------------------
172.16.0.0/16       eth0           Static    
172.16.10.0/24      eth2           Static    
```

### TC-3 (Lookup interface)

Execute below command
```
$ ./router-basic lookup 172.16.10.5
```

Expected output: Displays best route(longest prefix match) for dst ip

```
@ksindwani ➜ /workspaces/my-virtual-router (main) $ ./router-basic lookup 172.16.10.5
Match Found: 2886732288/24
-> New Best Match!
Result: Match Found!
Selected Prefix: 2886732288/24
Route Type:      Static
Resolved To:     eth2
Criteria:        Best match (longest prefix length).
```

### TC-4 (Explain lookup interface)

Perform detail path lookup for dst ip

Execute below command
```
$ ./router-basic lookup 172.16.10.5
```

Expected output: Displays detailed version of best route(longest prefix match) for dst ip

```
@ksindwani ➜ /workspaces/my-virtual-router (main) $ ./router-basic explain-lookup 172.16.10.5
[DEBUG] Loaded eth0 (Admin: "up", Oper: "up") -> is_up: TRUE
[DEBUG] Loaded eth1 (Admin: "up", Oper: "down") -> is_up: FALSE
[DEBUG] Loaded eth2 (Admin: "up", Oper: "up") -> is_up: TRUE
--- Routing Lookup Explanation ---
Destination IP (Binary): 2886732293
Match Found: 2886729728/16
-> New Best Match!
Match Found: 2886732288/24
-> New Best Match!
Result: Match Found!
Selected Prefix: 2886732288/24
Route Type:      Static
Resolved To:     eth2
Criteria:        Best match (longest prefix length).
@ksindwani ➜ /workspaces/my-virtual-router (main) $ ./router-basic lookup 172.16.10.5
[DEBUG] Loaded eth0 (Admin: "up", Oper: "up") -> is_up: TRUE
[DEBUG] Loaded eth1 (Admin: "up", Oper: "down") -> is_up: FALSE
[DEBUG] Loaded eth2 (Admin: "up", Oper: "up") -> is_up: TRUE
--- Routing Lookup Explanation ---
Destination IP (Binary): 2886732293
Match Found: 2886729728/16
-> New Best Match!
Match Found: 2886732288/24
-> New Best Match!
Result: Match Found!
Selected Prefix: 2886732288/24
Route Type:      Static
Resolved To:     eth2
Criteria:        Best match (longest prefix length).
```

### Validation scenarios for explain-lookup

| Test ID | Dest IP | Expected Path | Logic Explanation |
| :--- | :--- | :--- | :--- |
| **VAL-01** | 172.16.10.5 | eth2 | Matches both /16 and /24. The /24 route is more specific (Best Match). |
| **VAL-02** | 172.16.5.5 | 172.16.0.1 | Matches the /16 route. The route is valid because the associated interface is UP. |
| **VAL-03** | 192.168.1.5 | eth1 | Matches the /16 route. The route is valid because the associated interface is UP. |
| **VAL-04** | 192.168.1.5 | Drop | If eth1 oper_state is "down", the route will be excluded by the Gatekeeper, resulting in a drop. |
| **VAL-05** | 172.16.10.5 | Drop | If eth2 oper_state is "down", the route will be excluded by the Gatekeeper, resulting in a drop. |


```
@ksindwani ➜ /workspaces/my-virtual-router (main) $ ./router-basic 
[DEBUG] Loaded eth0 (Admin: "up", Oper: "up") -> is_up: TRUE
[DEBUG] Loaded eth1 (Admin: "up", Oper: "up") -> is_up: TRUE
[DEBUG] Loaded eth2 (Admin: "up", Oper: "up") -> is_up: TRUE
Adding routeAdding routeAdding route> 
> show-interfaces
Name           IP/Prefix           Status    RX        TX        
-----------------------------------------------------------------
eth0                               UP        1200      980       
eth1                               UP        0         0         
eth2                               UP        500       500       
> show-routes
Prefix              Next Hop       Type      
---------------------------------------------
172.16.0.0/16       eth0           Static    
192.168.1.0/24      eth1           Static    
172.16.10.0/24      eth2           Static    
> explain-lookup 172.16.10.5    
--- Routing Lookup Explanation ---
Destination IP (Binary): 2886732293
Match Found: 2886729728/16
-> New Best Match!
Match Found: 2886732288/24
-> New Best Match!
Result: Match Found!
Selected Prefix: 2886732288/24
Route Type:      Static
Resolved To:     eth2
Criteria:        Best match (longest prefix length).
> replay-events
[EVENT] Interface eth2 state set to DOWN
[EVENT] Interface eth2 state updated to "down"
[EVENT] Route 172.16.0.0/16 removed.
Event replay complete.
> explain-lookup 172.16.10.5
--- Routing Lookup Explanation ---
Destination IP (Binary): 2886732293
Result: No valid route found.
> 
```


# Routing Engine Validation Suite

## 1. Overview
This suite validates the core functionality of the event-driven routing engine. Tests ensure that the "Gatekeeper" logic (interface status) and the "LPM Engine" (Longest Prefix Match) operate correctly under dynamic state changes.

## 2. Test Environment
* **Initial State:** `interfaces.json` (eth0:UP, eth1:UP, eth2:UP) and `static_routes.json`.
* **Tooling:** `RouterCLI` with `replay-events` capability.

## 3. Test Cases

| ID | Scenario | Input | Expected Behavior |
|:---|:---|:---|:---|
| **VAL-01** | **Interface Gating** | `interface_state` (eth2: DOWN) | `explain-lookup` for a route via `eth2` must result in a "No Match" or "Interface Down" error. |
| **VAL-02** | **Route Removal** | `route_remove` (172.16.0.0/16) | `show-routes` must no longer list the removed prefix. |
| **VAL-03** | **LPM Precedence** | Add route 172.16.10.0/24 | Lookup for `172.16.10.5` must select the /24 route over the /16 route. |
| **VAL-04** | **Negative Validation** | Random IP `200.0.0.1` | Should result in "No route found" (Default drop). |

---

## 4. Input Validation & Error Handling

To ensure robust operation, the following validation checks are required for the `replay-events` and `lookup` commands:

### 4.1 CLI Input Validation
* **Invalid IP Format:** If `explain-lookup` receives an malformed IP (e.g., `10.0.0`), the system should output: `Error: Invalid IP format`.
* **Missing Files:** If `replay-events` points to a non-existent directory or file, the system must handle the exception gracefully: `Error: Could not open [path]`.

### 4.2 JSON Schema Validation (Events)
The `EventProcessor` must validate fields before processing:
* **Required Keys:** Every event must contain `type`, `target`, and `new_state`.
* **State Values:** `new_state` must be restricted to `up` or `down`. If an invalid value is provided, the system should log: `Error: Unknown state [value]`.

### 4.3 Runtime Logic Validation
* **Interface Existence:** If an event tries to change the state of `eth99` (non-existent), the system should issue a warning: `Warning: Interface [name] not found`.
* **Route Existence:** If `route_remove` targets a prefix not in the table, output: `Warning: Prefix [prefix] does not exist`.

---

## 5. Execution Script
Use the following commands in the CLI to verify the test suite:

```bash
# 1. Verify initial lookup
explain-lookup 172.16.10.5 

# 2. Trigger VAL-01 and VAL-02
replay-events samples/validation_events.json

# 3. Verify state change
show-routes
show-interfaces
explain-lookup 172.16.10.5