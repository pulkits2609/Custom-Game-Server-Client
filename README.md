# Custom Game Server Client

A modular, reusable Unreal Engine 5 client plugin written in modern C++ that communicates with the **Custom Game Server** backend through both **HTTP** and **persistent realtime TCP networking**.

The plugin provides a complete client-side networking framework for multiplayer games, including session authentication, lobby management, realtime event synchronization, automatic reconnection, presence updates, and a modular UI framework.

Unlike traditional Unreal networking based on built-in replication, this project communicates with a completely custom multiplayer backend, allowing the client to remain backend-agnostic while exposing clean C++ APIs and Blueprint integration.

---

# Features

## Session System

A complete authentication layer built on top of the backend HTTP APIs.

### Supported Operations

- Login
- Logout
- Fetch Session

### Features

- SessionID & SessionToken management
- Automatic session caching
- Login state tracking
- Session information retrieval
- Automatic realtime authentication after login
- Delegate-driven architecture

---

# Lobby System

Complete support for the backend lobby lifecycle.

### Lobby Operations

- Create Lobby
- Fetch Current Lobby
- Fetch Lobby by ID
- Fetch Lobby List
- Join Lobby
- Leave Lobby
- Destroy Lobby
- Kick Player

### Lobby Features

- Cached lobby information
- Cached lobby browser list
- Automatic lobby refresh
- Delegate-driven responses
- HTTP abstraction layer
- Strongly typed request/response parsing

---

# HTTP Networking

A reusable HTTP layer built around Unreal Engine's HTTP module.

### Features

- REST API communication
- JSON serialization
- JSON deserialization
- Authorization header helpers
- Centralized HTTP helpers
- Request builders
- Modular request parsing
- Backend URL configuration through Project Settings

---

# Realtime Networking

A dedicated persistent TCP networking layer completely independent from HTTP.

The realtime layer maintains a long-lived socket connection to the backend and receives server events in real time.

## Features

- Persistent TCP socket
- Non-blocking communication
- Event-driven architecture
- Automatic JSON message parsing
- Connection monitoring
- Automatic reconnect support
- Socket authentication
- Heartbeat (Ping/Pong)
- Presence synchronization

---

# Automatic Reconnect

The client automatically reconnects whenever the socket connection is lost.

Reconnect workflow:

```
Authenticated

↓

Gameplay

↓

Connection Lost

↓

Detect Disconnect

↓

Reconnect Requested

↓

Retry Connection

↓

Authenticate

↓

Resume Events
```

### Features

- Automatic reconnect attempts
- Configurable retry delay
- Maximum retry count
- Authentication after reconnect
- Reconnect state reset after successful authentication

---

# Heartbeat System

The plugin implements the client side of the heartbeat protocol.

Workflow:

```
Server

↓

Ping

↓

Client

↓

Pong

↓

Server Updates Last Seen
```

Heartbeat support prevents stale socket connections while allowing temporary network interruptions.

---

# Presence Events

The client supports realtime player presence updates.

Supported events include:

- PlayerConnected
- PlayerDisconnected
- PlayerReconnected
- PlayerTimedOut

These events are exposed through Blueprint delegates for gameplay and UI synchronization.

---

# Realtime Event Router

Incoming socket messages are routed through a dedicated dispatcher.

Workflow:

```
Socket

↓

JSON

↓

Event Router

↓

Specific Handler

↓

Subsystem Delegate

↓

UI / Gameplay
```

Currently supported events include:

- Authenticated
- AuthenticationFailed
- LobbyCreated
- LobbyUpdated
- LobbyDestroyed
- LobbyListUpdated
- PlayerConnected
- PlayerDisconnected
- PlayerReconnected
- PlayerTimedOut
- Ping

The router remains lightweight and only dispatches events without containing gameplay logic.

---

# UI Framework

The plugin includes a modular C++ UI architecture designed around reusable widgets.

## Root Navigation

A dedicated root widget manages application flow using a `WidgetSwitcher`.

Current navigation flow:

```
Login

↓

Loading

↓

Lobby Browser

↓

Current Lobby
```

Navigation is fully event-driven and reacts automatically to subsystem delegates.

---

# UI Components

### CGSTestRootWidget

Acts as the application's root controller.

Responsibilities:

- Widget navigation
- Application flow
- Delegate binding
- Login flow
- Lobby flow
- Realtime event handling

---

### Login Widget

Responsibilities:

- Login requests
- Login UI
- Authentication interaction

---

### Lobby Browser Widget

Responsibilities:

- Refresh lobby list
- Display available lobbies
- Join lobby
- Lobby browser interactions

---

### Lobby Entry Widget

Represents a single lobby entry.

Responsibilities:

- Display lobby information
- Join lobby
- Kick player (host)

---

### Create Lobby Widget

Dedicated widget responsible only for lobby creation.

Responsibilities:

- Create lobby
- Create lobby validation

---

### Current Lobby Widget *(In Progress)*

Will become the primary in-lobby interface.

Planned responsibilities:

- Player list
- Host controls
- Leave lobby
- Ready state
- Presence
- Match start
- Chat
- Realtime updates

---

# Unreal Engine Integration

The client is implemented entirely as a reusable Unreal Engine plugin.

## GameInstance Subsystems

The plugin exposes three primary subsystems:

### Session Subsystem

Responsible for:

- Login
- Logout
- Session information
- Session state

---

### Lobby Subsystem

Responsible for:

- Lobby APIs
- Cached lobby information
- Lobby browser cache
- Lobby delegates

---

### Realtime Subsystem

Responsible for:

- Socket connection
- Authentication
- Event routing
- Heartbeat
- Automatic reconnect
- Presence events

---

# Plugin Architecture

```
                UI Layer
                     │
     ┌───────────────┼───────────────┐
     ▼               ▼               ▼
 Login         LobbyBrowser     CurrentLobby
                     │
                     ▼
              Root UI Controller
                     │
                     ▼
          GameInstance Subsystems
       ┌─────────────┼──────────────┐
       ▼             ▼              ▼
    Session       Lobby        Realtime
       │             │              │
       ▼             ▼              ▼
     HTTP        HTTP        Socket Client
                                  │
                                  ▼
                            Event Router
                                  │
                                  ▼
                              Delegates
```

The architecture follows strict separation of responsibilities, allowing each layer to remain independent and easily extensible.

---

# Current Realtime Workflow

```
Login

↓

HTTP Authentication

↓

Session Token

↓

Realtime Socket

↓

Authenticate

↓

Realtime Events

↓

UI Synchronization
```

---

# Backend Communication

The client communicates with the backend using two separate networking layers.

## HTTP

Responsible for:

- Authentication
- Lobby state changes
- Authoritative operations

---

## Realtime TCP

Responsible for:

- Notifications
- Presence
- Heartbeat
- Lobby updates
- Connection events

The client always refreshes authoritative state through HTTP after receiving realtime notifications.

---

# Technologies Used

- Unreal Engine 5
- Modern C++
- Unreal HTTP Module
- Unreal JSON
- Unreal Sockets API
- TCP Networking
- HTTP REST APIs
- JSON
- Blueprint Integration
- GameInstance Subsystems
- Widget Framework

---

# Project Structure

```
Custom_Game_Server_Client/
│
├── Public/
│   ├── Http/
│   ├── Lobby/
│   ├── Realtime/
│   ├── Sessions/
│   ├── Settings/
│   └── UI/
│       ├── Base/
│       ├── Login/
│       ├── Lobby/
│       └── Root/
│
├── Private/
│   ├── Http/
│   ├── Lobby/
│   ├── Realtime/
│   ├── Sessions/
│   ├── Settings/
│   └── UI/
│       ├── Base/
│       ├── Login/
│       ├── Lobby/
│       └── Root/
│
├── Custom_Game_Server_Client.uplugin
├── Build.cs
└── README.md
```

---

# Current Development Status

## Completed

- HTTP networking layer
- Session authentication
- Session caching
- Lobby lifecycle support
- Lobby browser
- Lobby caching
- Kick player support
- Persistent realtime TCP client
- Socket authentication
- Event routing
- Presence events
- Heartbeat (Ping/Pong)
- Automatic reconnect framework
- Delegate-driven architecture
- Root UI navigation
- WidgetSwitcher-based UI flow
- Modular Unreal Engine plugin architecture

---

# In Progress

- Current Lobby Widget
- Automatic UI synchronization
- Lobby presence indicators
- Dynamic lobby entries
- Cached UI refresh system

---

# Planned Features

- Lobby ready system
- Match start workflow
- Gameplay packet protocol
- Player registration
- Spawn system
- Gameplay state synchronization
- Movement replication
- Animation synchronization
- Gameplay event replication
- Chat system
- Matchmaking
- Dedicated gameplay session management

---

# Design Philosophy

The project is built around a strict separation of concerns.

- **HTTP** remains the authoritative layer responsible for changing server state.
- **Realtime TCP** is responsible only for notifying clients about state changes.
- **Subsystems** own networking logic and application state.
- **Widgets** focus purely on presentation and user interaction.
- **The Root Widget** manages application navigation.
- **Realtime events** drive UI synchronization while HTTP remains the single source of truth.

This layered architecture keeps the plugin modular, reusable, scalable, and easy to extend for future gameplay systems while remaining independent of Unreal Engine's built-in networking model.