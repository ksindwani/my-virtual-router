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