# UDP

______________________________________________________________________

**Date:** 2026-07-16
**Tags:** [Udemy.md](tags/Udemy.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md), [UDP.md](tags/UDP.md), [protocol.md](tags/protocol.md)
**URL:**

______________________________________________________________________


User Datagram Protocol.

## UDP

- Message Based Layer 4 protocol
- Ability to address processes in a host using ports
- Simple protocol to send and receive messages
- Prior communication not required (double edge sword)
- Stateless — no knowledge is stored on the host
- 8 byte header Datagram

## UDP Use cases

- Video streaming
- VPN
- DNS
- WebRTC

## Multiplexing and demultiplexing

- IP target hosts only
- Hosts run many apps each with different requirements
- Ports now identify the "app" or "process"
- Sender multiplexes all its apps into UDP
- Receiver demultiplex UDP datagrams to each app

## Source and Destination Port

- App1 on 10.0.0.1 sends data to AppX on 10.0.0.2
- Destination Port = 53
- AppX responds back to App1
- We need Source Port so we know how to send back data
- Source Port = 5555

## UDP Pros

- Simple protocol
- Header size is small so datagrams are small
- Uses less bandwidth
- Stateless
- Consumes less memory (no state stored in the server/client)
- Low latency - no handshake, order, retransmission or guaranteed delivery

## UDP Cons

- No acknowledgement
- No guarantee delivery
- Connection-less - anyone can send data without prior knowledge
- No flow control
- No congestion control
- No ordered packets
- Security - can be easily spoofed

## Summary

- UDP is a simple message based layer 4 protocol
- Uses ports to address processes
- Stateless
- Pros & Cons
