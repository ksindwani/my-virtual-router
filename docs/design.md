## Architecture diagram

+------------------------------------------------------+
|                  Virtual Router Process              |
+------------------------------------------------------+
|                                                      |
|  +----------------+      +----------------------+    |
|  | Config Loader  |----->| Routing Table Mgr   |    |
|  +----------------+      +----------------------+    |
|         |                          |                |
|         |                          v                |
|         |                +-------------------+      |
|         |                | LPM Lookup Engine |      |
|         |                +-------------------+      |
|         |                          |                |
|         v                          |                |
|  +----------------+                |                |
|  | Interface Mgr  |----------------+                |
|  +----------------+                                 |
|         |                                           |
|         v                                           |
|  +----------------+                                 |
|  | Statistics Mgr |                                 |
|  +----------------+                                 |
|                                                      |
|  +----------------+                                 |
|  | Router CLI     |---------------------------------+
|  +----------------+                                 |
|                                                      |
+------------------------------------------------------+

## Component diagram

                    +------------------+
                    | RouterCLI        |
                    +--------+---------+
                             |
                             v
+----------------+   +---------------+   +------------------+
| InterfaceMgr   |<->| RouterCore    |<->| RoutingTableMgr  |
+----------------+   +---------------+   +------------------+
        |                    |                     |
        |                    |                     |
        v                    v                     v
+----------------+   +---------------+   +------------------+
| StatisticsMgr  |   | ConfigLoader  |   | LPM Engine       |
+----------------+   +---------------+   +------------------+
                             |
                             v
                   +--------------------+
                   | JSON Configuration |
                   +--------------------+

## Data flow

# Startup Phase

interfaces.json
       |
       v
+----------------+
| Config Loader  |
+----------------+
       |
       v
+----------------+
| Interface Mgr  |
+----------------+
       |
       | derive connected routes
       v

static_routes.json
       |
       v
+----------------+
| Config Loader  |
+----------------+
       |
       v

+--------------------------------+
| Routing Table Manager          |
|                                |
| Connected Routes              |
| + Static Routes               |
+--------------------------------+
       |
       v
 Active Routing Table


# Route Lookup Flow

CLI Command

lookup 10.10.20.5
      |
      v
+----------------+
| Router CLI     |
+----------------+
      |
      v
+----------------+
| LPM Engine     |
+----------------+
      |
      | Search routing table
      v
Best Matching Route
      |
      v
CLI Output

# Statistics flow

show interfaces
       |
       v
Router CLI
       |
       v
Statistics Manager
       |
       v
Interface Counters
       |
       v
Display

# Class diagram

+-----------------------+
| RouterCore            |
+-----------------------+
| initialize()          |
| runCLI()              |
+-----------+-----------+
            |
            |
            v

+-----------------------+
| ConfigLoader          |
+-----------------------+
| loadInterfaces()      |
| loadStaticRoutes()    |
+-----------------------+

+-----------------------+
| InterfaceManager      |
+-----------------------+
| getInterfaces()       |
| deriveConnected()     |
+-----------------------+

+-----------------------+
| RoutingTableManager   |
+-----------------------+
| buildTable()          |
| getRoutes()           |
+-----------------------+

+-----------------------+
| LpmEngine             |
+-----------------------+
| lookup(ip)            |
+-----------------------+

+-----------------------+
| StatisticsManager     |
+-----------------------+
| showInterfaces()      |
| incrementRx()         |
| incrementTx()         |
+-----------------------+

+-----------------------+
| RouterCLI             |
+-----------------------+
| processCommand()      |
+-----------------------+