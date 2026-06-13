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

### Validation scenraios for explain-lookup

| Test ID | Dest IP | Expected Path | Logic Explanation |
| :--- | :--- | :--- | :--- |
| **VAL-01** | 172.16.10.5 | eth2 | Matches both /16 and /24. The /24 route is more specific (Best Match). |
| **VAL-02** | 172.16.5.5 | 172.16.0.1 | Matches the /16 route. The route is valid because the associated interface is UP. |
| **VAL-03** | 192.168.1.5 | eth1 | Matches the /16 route. The route is valid because the associated interface is UP. |
| **VAL-04** | 192.168.1.5 | Drop | If eth1 oper_state is "down", the route will be excluded by the Gatekeeper, resulting in a drop. |
| **VAL-05** | 172.16.10.5 | Drop | If eth2 oper_state is "down", the route will be excluded by the Gatekeeper, resulting in a drop. |
