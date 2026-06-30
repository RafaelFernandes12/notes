# Stateless vs Stateful

______________________________________________________________________

**Date:** 2026-04-27
**Tags:**[Udemy.md](tags/Udemy.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md), [Backend_communication_design_patterns.md](tags/Backend_communication_design_patterns.md)
**URL:**

______________________________________________________________________

## Stateful vs Stateless backend

- Stateful
  - Stores state about clients in its memory
  - Depends on the information being there
- Stateless
  - client is responsible to "transfer" the state to the with every request
  - May store but can safely lose it

## Stateles backends

- Stateles backends can still store data somewhere else
- Can you restart the backend during idle time while the client workflow continues to work?

## What makes a backend stateless

- Stateless backends can store state somewhere else (database)
- The backend remain stateless but the system is stateful
- Can you restart the backend during idle time while the client workflow continues to work?

![stateful backend](../assets/stateful%20backend.png)

![stateful backend where it breaks](../assets/stateful%20backend%20where%20it%20breaks.png)

![stateless backend](../assets/stateless%20backend.png)

## Stateless vs Stateful protocols

- The protocols can be designed to store state
- TCP is stateful
  - Sequences, Connection file descriptor
- UDP is stateless
  - DNS send queryID in UDP to identify queries
  - QUIC sends connectionID to identify connection

## Stateful vs Stateless protocols

- You can build a stateless protocol on top of a stateful one and vice versa
- HTTP on top of TCP
- if TCP breaks, HTTP blindly create another one
- QUIC is build on top of UDP

## Complete Stateless system

- Stateles system are rare
- State is carried with every request
- A backend service that relies completely on the input
  - check if input param is a prime number
- JWT (JSON web token)
