# OSI model

______________________________________________________________________

**Date:** 2026-07-06
**Tags:** [Udemy.md](tags/Udemy.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md), [OSI.md](tags/OSI.md), [protocol.md](tags/protocol.md)
**URL:**

______________________________________________________________________


Open Systems Interconnection model.

## Why do we need a communication model?

- **Agnostic applications**
    - App doesn't need to know the network medium
    - Otherwise we'd need a separate app for WIFI, ethernet vs LTE vs fiber
- **Network Equipment Management**
    - Without a standard model, upgrading network equipment becomes difficult
- **Decoupled Innovation**
    - Innovations can be done in each layer separately without affecting the rest of the model

## What is the OSI Model?

7 layers, each describing a specific networking component.

| Layer | Name         | Responsibility                    |
| ----- | ------------ | --------------------------------- |
| 7     | Application  | HTTP / FTP / gRPC                 |
| 6     | Presentation | Encoding, Serialization           |
| 5     | Session      | Connection establishment, TLS     |
| 4     | Transport    | UDP / TCP                         |
| 3     | Network      | IP                                |
| 2     | Data link    | Frames, MAC address, Ethernet     |
| 1     | Physical     | Electric signals, fiber or radio waves |

## Example — client sends an HTTPS POST request

Data travels **down** the layers on the client and **up** the layers on the server.
Each layer wraps the data with its own info (data units):

- **Transport** → Segment (adds SPORT, DPORT)
- **Network** → Packet (adds SIP, DIP)
- **Data Link** → Frame (adds SMAC, DMAC)
- **Physical** → electric signals / radio waves

## Across networks

Not every device maps the entire 7 layers:

- **Switch** — operates at Layers 1-2 (Physical, Data Link)
- **Router** — operates at Layers 1-3 (adds Network)
- **Layer 4 Proxy / Firewall** — operates up to Layer 4 (Transport)
- **Layer 7 Load Balancer / CDN** — operates through all 7 layers (Application)

## Shortcomings of the OSI Model

- Too many layers, which can be hard to comprehend
- Hard to argue about which layer does what
- Simpler to deal with Layers 5-6-7 as just one layer: application
- The TCP/IP model does just that

## TCP/IP Model

Much simpler than OSI, just 4 layers:

- **Application** (Layers 5, 6 and 7)
- **Transport** (Layer 4)
- **Internet** (Layer 3)
- **Data link** (Layer 2)
- Physical layer is not officially covered in the model

## Summary

- Why do we need a communication model?
- What is the OSI Model?
- Example
- Each device in the network doesn't have to map the entire 7 layers
- TCP/IP is a simpler model
