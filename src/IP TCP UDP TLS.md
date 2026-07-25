# IP TCP UDP TLS

______________________________________________________________________

**Date:** 2026-07-15
**Tags:** [Udemy.md](tags/Udemy.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md), [IP.md](tags/IP.md), [TCP.md](tags/TCP.md), [UDP.md](tags/UDP.md), [TLS.md](tags/TLS.md), [protocol.md](tags/protocol.md)
**URL:**

______________________________________________________________________


# The IP building blocks

## IP Address

- Layer 3 property
- Can be set automatically or statically
- Has a **Network** and a **Host** portion
- 4 bytes in IPv4 → 32 bits

## Network vs Host

- `a.b.c.d/x` — `a.b.c.d` are integers, `x` is the number of **network** bits, the remaining bits are **host**
- Example `192.168.254.0/24`
    - The first 24 bits (3 bytes) are network, the last 8 bits are host
    - So we can have 2^24 (16,777,216) networks, and each network has 2^8 (256) hosts
- Also called a **subnet**

## Subnet Mask

- `192.168.254.0/24` is also called a subnet
- The subnet has a mask `255.255.255.0`
- The subnet mask is used to determine whether an IP is in the same subnet

## Default Gateway

- Most networks consist of hosts and a Default Gateway
- Host A can talk to B directly if both are in the same subnet
- Otherwise A sends it to someone who might know — the gateway
- The Gateway has an IP address, and each host should know its gateway

### Example — same subnet (no routing)

Host `192.168.1.3` wants to talk to `192.168.1.2`:

- Apply the subnet mask to both:
    - `255.255.255.0 & 192.168.1.3 = 192.168.1.0`
    - `255.255.255.0 & 192.168.1.2 = 192.168.1.0`
- Same subnet → no need to route.

### Example — different subnet (route via gateway)

Host `192.168.1.3` wants to talk to `192.168.2.2`:

- `255.255.255.0 & 192.168.1.3 = 192.168.1.0`
- `255.255.255.0 & 192.168.2.2 = 192.168.2.0`
- Not the same subnet → the packet is sent to the Default Gateway (`192.168.1.100`)


# The IP Packet

- The IP Packet has **headers** and **data** sections
- IP Packet header is **20 bytes** (up to 60 bytes if options are enabled)
- Data section can go up to **65,536** bytes
- Packets get **fragmented** if they don't fit in a frame

### Key header fields

- **Version** — the protocol version (IPv4 / IPv6)
- **IHL** (Internet Header Length) — defines the options length
- **Total Length** — 16-bit, data + header
- **Identification / Flags / Fragment Offset** — used for fragmentation (jumbo packets)
- **Time To Live (TTL)** — how many hops the packet can survive
- **Protocol** — what protocol is inside the data section (TCP, UDP, ICMP…)
- **Source IP Address** / **Destination IP Address**
- **ECN** (Explicit Congestion Notification)
- **Header Checksum**

> To the backend engineer, an IP packet is essentially: `[Source IP][Data][Destination IP]`.

References: [RFC 791](https://datatracker.ietf.org/doc/html/rfc791), [IPv4 (Wikipedia)](https://en.wikipedia.org/wiki/IPv4)


# ICMP — Internet Control Message Protocol

- Designed for **informational messages**
    - Host unreachable, port unreachable, fragmentation needed
    - Packet expired (infinite loop in routers)
- Uses IP directly
- **PING** and **traceroute** use it
- Doesn't require listeners or ports to be opened
- Header: Type, Code, Checksum, Rest of header
- Some firewalls block ICMP for security reasons
    - That's why PING might not work in those cases
    - Disabling ICMP can cause real damage to connection establishment (fragmentation needed)

## Ping

- Sends an ICMP **echo request**; the target replies with an ICMP **echo reply**
- If a router can't reach the destination it replies with **ICMP dest unreachable**

## TraceRoute

- Can you identify the entire path your IP packet takes?
- Clever use of **TTL** — increment TTL slowly and you get the router IP for each hop
- Doesn't always work, as the path changes and ICMP might be blocked

References: [ICMP (Wikipedia)](https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol), [RFC 792](https://datatracker.ietf.org/doc/html/rfc792)


# ARP — Address Resolution Protocol

- We need the **MAC address** to send frames (Layer 2)
- Most of the time we know the IP address but not the MAC
- The **ARP Table** is a cached IP → MAC mapping

### How it works

- Host 2 wants to connect to host 5 (same subnet), needs host 5's MAC, checks its ARP table — not there
- Host 2 broadcasts an **ARP request** to all machines: "Who has IP `10.0.0.5`?"
- Host 5 replies with its MAC; Host 2 updates its ARP table
- If the destination is **not** in the subnet, the host ARPs for the **gateway's** MAC instead, then NAT kicks in

> Attacks can be performed on ARP (**ARP poisoning**).


# Routing Example

- How IP packets are routed between **switches** (Layer 2) and **routers** (Layer 3)
- Hosts on the same switch/subnet talk directly (MAC-to-MAC)
- Cross-subnet or internet traffic goes through the router/gateway


# UDP — User Datagram Protocol

- Layer 4 protocol
- Ability to address processes in a host using **ports**
- Simple protocol to send and receive data
- Prior communication not required (double-edged sword)
- **Stateless** — no knowledge stored on the host
- 8-byte header datagram

## Use cases

- Video streaming, VPN, DNS, WebRTC

## Multiplexing and demultiplexing

- IP targets hosts only; hosts run many apps, each with different requirements
- **Ports** identify the "app" or "process"
- Sender **multiplexes** all its apps into UDP
- Receiver **demultiplexes** UDP datagrams to each app

## Source and Destination Port

- App1 on `10.0.0.1` sends data to AppX on `10.0.0.2`, Destination Port = 53
- We need the **Source Port** so the reply knows where to go back (Source Port = 5555)

## UDP Datagram header (8 bytes)

- Source port (16 bit) / Destination port (16 bit)
- Length / Checksum
- Ports are 16 bit (0 to 65535)

References: [RFC 768](https://www.ietf.org/rfc/rfc768.txt), [UDP (Wikipedia)](https://en.wikipedia.org/wiki/User_Datagram_Protocol)

## UDP Pros

- Simple protocol, small header → small datagrams, uses less bandwidth
- Stateless, consumes less memory (no state stored on server/client)
- Low latency — no handshake, ordering, retransmission or guaranteed delivery

## UDP Cons

- No acknowledgement, no guaranteed delivery
- Connection-less — anyone can send data without prior knowledge
- No flow control, no congestion control, no ordered packets
- Security — can be easily spoofed


# TCP — Transmission Control Protocol

- Layer 4 protocol, addresses processes using **ports**
- "Controls" the transmission, unlike UDP which is a firehose
- **Connection**-oriented, requires a handshake
- 20-byte header segment (can go up to 60)
- **Stateful**

## Use cases

- Reliable communication, remote shell, database connections, web communications, any bidirectional communication

## TCP Connection

- A connection is a Layer 5 (session) concept — an agreement between client and server
- Must create a connection to send data
- Identified by **4 properties**: SourceIP-SourcePort, DestinationIP-DestinationPort
- Can't send data outside of a connection
- Sometimes called a **socket** or **file descriptor**
- Requires a **3-way handshake**
- Segments are sequenced, ordered, acknowledged; lost segments are retransmitted

## Connection Establishment (3-way handshake)

- App1 sends **SYN** to synchronize sequence numbers
- AppX sends **SYN/ACK** to synchronize its sequence number
- App1 **ACK**s AppX's SYN

## Sending data & Acknowledgment

- App1 encapsulates data in a segment and sends it; AppX acknowledges
- Multiple segments can be sent before an ACK arrives (e.g. send seq 1,2,3 → single **ACK 3**)

## Lost data

- If seg 3 is lost, AppX acknowledges up to what it received (**ACK 2**), App1 resends seq 3

## Closing Connection (4-way handshake)

- App1 sends **FIN**, AppX **ACK**s
- AppX sends **FIN**, App1 **ACK**s

## TCP Segment header

- Header is 20 bytes, up to 60 bytes; slides into an IP packet as "data"
- Ports are 16 bit (0 to 65535)
- Key fields: Source/Destination port, **Sequence number**, **Acknowledgment number**, Data offset, Window Size, Checksum, Urgent pointer
- **9-bit flags**: NS, CWR, ECE, URG, ACK, PSH, RST, SYN, FIN
- **Flow Control** uses the **Window Size** field

## Maximum Segment Size (MSS)

- Segment size depends on the **MTU** of the network
- Usually 512 bytes, can go up to 1460
- Default MTU on the internet is 1500 (→ MSS 1460)
- Jumbo frames MTU goes to 9000 or more (MSS can be larger)

References: [TCP (Wikipedia)](https://en.wikipedia.org/wiki/Transmission_Control_Protocol), [RFC 793](https://datatracker.ietf.org/doc/html/rfc793)

## TCP Pros

- Guaranteed delivery
- No one can send data without prior knowledge
- Flow Control and Congestion Control
- Ordered packets, no corruption or app-level work
- Secure, can't be easily spoofed

## TCP Cons

- Large header overhead compared to UDP, more bandwidth
- Stateful — consumes memory on server and client
- Considered high latency for certain workloads (slow start / congestion / acks)
- Does too much at a low level (hence **QUIC**)
    - Single connection to send multiple streams of data (HTTP requests)
    - Stream 1 has nothing to do with Stream 2, but both streams' packets must arrive (head-of-line blocking)
- **TCP Meltdown** — not a good candidate for VPN


# TLS — Transport Layer Security

## Why TLS

- We encrypt with **symmetric key** algorithms
- We need to exchange the symmetric key
- Key exchange uses **asymmetric key** (PKI)
- Authenticate the server
- Extensions (SNI, preshared, 0RTT)

## HTTP vs HTTPS

- **HTTP** (port 80): open → GET / → response (headers + html) → close, all in plain text
- **HTTPS** (port 443): open → **Handshake** → encrypted GET / → encrypted response → close

## TLS 1.2 Handshake (two round trips)

- Client hello
- Server hello (with **cert** / RSA public key)
- Change cipher, fin
- Change cipher, fin (server)
- Then encrypted GET / and response
- The symmetric key is encrypted with the server's RSA public key and only the server can decrypt with its private key

## Diffie Hellman

- Private `x` + Public `g,n` + Private `y` = Symmetric key
- Public / unbreakable / shareable values: `g^x % n` and `g^y % n`
- Both sides compute the same secret:
    - `(g^x % n)^y = g^xy % n`
    - `(g^y % n)^x = g^xy % n`

## TLS 1.3 Improvements (one round trip, can be zero)

- Client hello / key / fin
- Server hello / change cipher / fin
- Then encrypted GET / and response
- Faster: one round trip (can be zero with 0RTT)


# Overview of Popular Networking Protocols

Covered across this deck: IP, ICMP, ARP, UDP, TCP, TLS.
