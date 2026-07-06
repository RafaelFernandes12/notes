# Protocol properties

______________________________________________________________________

**Date:** 2026-07-06
**Tags:**[Udemy.md](tags/Udemy.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md), [protocol.md](tags/protocol.md)
**URL:**

______________________________________________________________________


## Whats a protocol?

- A system that allows two parties to communicate
- A protocol is designed with a set of properties
- Depending on the purpose of the protocol
- TCP, UDP, HTTP, gRPC, FTP


## Protocol properties

What to take into account when designing a protocol.

### Data format

- **Text based** (plain text, JSON, XML)
- **Binary** (protobuf, RESP, h2, h3)

### Transfer mode

- **Message based** (UDP, HTTP)
- **Stream** (TCP, WebRTC)

### Addressing system

- DNS name, IP, MAC

### Directionality

- **Bidirectional** (TCP)
- **Unidirectional** (HTTP)
- **Full/Half duplex**

### State

- **Stateful** (TCP, gRPC, Apache Thrift)
- **Stateless** (UDP, HTTP)

### Routing

- Proxies, Gateways

### Flow & Congestion control

- **TCP** (Flow & Congestion)
- **UDP** (No control)

### Error management

- Error code
- Retries and timeouts
