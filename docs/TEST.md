### Test Cases


**Valid Scenarios**
#### TC-1 (Display interfaces)
Run `show-interfaces` - Displayed configured interfaces - PASS

#### TC-2 (Display static routes)
Run `show-routes` - Dislpays configured static routes - PASS

#### TC-3 (explain-lookup "destip")
**LPM Precedence** - Add route 172.16.10.0/24 | Lookup for `172.16.10.5` must select the /24 route over the /16 route.

Run `explain-lookup 172.16.10.5` - Displays detailed lookup route for dest ip - PASS


#### TC-4 (replay-events with input file)
`replay-events ./events.json` <br>
**Interface state toggle** | `show-interfaces` should display interface state as down. <br>
**Route Removal** | `route_remove` (172.16.0.0/16) | `show-routes` must no longer list the removed prefix.

Run `replay-events ./input/events.json` - Execute the events configured in input file - PASS

**After replay-event processing testing**

### TC-5 (Explain-lookup)
(explain-lookup destip after setting interface to down and removing a route)
Run `explain-lookup 172.16.10.5` - This should fail with error `No valid route found` because eth2 is down and another route is removed as per events configured in events.json - PASS

### TC-6 (Validate `show-routes` after replay-events)
Run `show-routes` - Validate one of the route will be deleted `172.16.0.0/16` in this case - PASS

### TC-7 (Validate `show-interfaces` after replay-events)
9. Run `show-interfaces` - Validate eth2 is down - PASS

### TC-8 (Validate lookup for destip)
Run `lookup 192.168.1.1` - Displays eth name of valid route

**Negative Scenarios**

#### TC-9 (replay-events with no file argument)
Run `replay-events` - This should fail with missing file argument - PASS

### TC-10 (Invalid ip address format)
Run `lookup "invalidip"` - This should return invalid ip address format error


```
 ./router-basic ./input/interfaces.json ./input/static_routes.json
[DEBUG] Loaded eth0 (Admin: "up", Oper: "up") -> is_up: TRUE
[DEBUG] Loaded eth1 (Admin: "up", Oper: "up") -> is_up: TRUE
[DEBUG] Loaded eth2 (Admin: "up", Oper: "up") -> is_up: TRUE
Adding routeAdding routeAdding route> show-routes    
Prefix              Next Hop       Type      
---------------------------------------------
172.16.0.0/16       eth0           Static    
192.168.1.0/24      eth1           Static    
172.16.10.0/24      eth2           Static    
> show-interfaces
Name           IP/Prefix           Status    RX        TX        
-----------------------------------------------------------------
eth0                               UP        1200      980       
eth1                               UP        0         0         
eth2                               UP        500       500       
> explain-lookup 172.16.10.5
--- Routing Lookup Explanation ---
[LOOKUP] Destination IP: 172.16.10.5
Match Found: 2886729728/16
-> New Best Match!
Match Found: 2886732288/24
-> New Best Match!
Result: Match Found!
Selected Prefix: 172.16.10.0/24
Route Type:      Static
Resolved To:     eth2
Criteria:        Best match (longest prefix length).
> replay-events 
Error: Please provide a filename. Usage: replay-events <file>
> replay-events ./input/events.json
[EVENT] Interface eth2 state set to DOWN
[EVENT] Interface eth2 state updated to "down"
[SUCCESS] Removed route: 172.16.0.0/16
[EVENT] Route 172.16.0.0/16 removed.
Event replay complete.
> explain-lookup 172.16.10.5
--- Routing Lookup Explanation ---
[LOOKUP] Destination IP: 172.16.10.5
Skipping route 2886732288/24 (Interface eth2 is DOWN)
Result: No valid route found.
> show-interfaces
Name           IP/Prefix           Status    RX        TX        
-----------------------------------------------------------------
eth0                               UP        1200      980       
eth1                               UP        0         0         
eth2                               DOWN      500       500       
> show-routes
Prefix              Next Hop       Type      
---------------------------------------------
192.168.1.0/24      eth1           Static    
172.16.10.0/24      eth2           Static    
> lookup 192.168.1.1
Match found: eth1
> lookup 10.0.01
Error: Invalid IP address format: 10.0.01
```
