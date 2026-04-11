# Common Request-Response Protocols

______________________________________________________________________

**Date:** 2026-04-11
**Tags:** [Udemy](../tags/Udemy.md), [Backend_communication_design_patterns](../tags/Backend_communication_design_patterns.md), [Fundamentals_of_backend_engineering](../tags/Fundamentals_of_backend_engineering.md), [HTTP](../tags/HTTP.md), [DNS](../tags/DNS.md), [SSH](../tags/SSH.md), [RPC](../tags/RPC.md)
**URL:** https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview

______________________________________________________________________

## Concept (what is it?)

HTTP, DNS, SSH, and RPC all appear when people talk about **request-response systems**, but they are not the same kind of thing.

- **HTTP** is a protocol for transferring web requests and responses.
- **DNS** is a protocol for asking “what address or record belongs to this name?”.
- **SSH** is a secure protocol for remote login, command execution, and tunneling.
- **RPC** is a communication style where code calls a function on another machine and gets a result back.

Important nuance:

- **HTTP, DNS, and SSH are protocols**
- **RPC is a pattern/abstraction**, not one single wire protocol

So RPC can be implemented in different ways, such as **JSON-RPC over HTTP** or **gRPC over HTTP/2**.

______________________________________________________________________

## Quick comparison

| Topic | Main request | Main response | Common use |
| --- | --- | --- | --- |
| HTTP | “give me this resource / process this action” | status code, headers, body | browsers, REST APIs, web backends |
| DNS | “what record belongs to this domain?” | resource records (`A`, `AAAA`, `MX`, `CNAME`, `TXT`) | name resolution |
| SSH | “open a secure session / run this command” | encrypted session data, command output, exit status | remote administration, deploys, file copy |
| RPC | “execute this remote method with these params” | return value or error | service-to-service communication |

______________________________________________________________________

## HTTP

### What is it?

HTTP (**Hypertext Transfer Protocol**) is the protocol most web apps and APIs use.

A client sends a request with:

- a **method** (`GET`, `POST`, `PUT`, `DELETE`, ...)
- a **URL/path**
- **headers**
- sometimes a **body**

The server returns:

- a **status code** (`200`, `404`, `500`, ...)
- **headers**
- a **body** (HTML, JSON, file, etc.)

### Example 1 — HTTP request with `curl`

```bash
curl -i https://jsonplaceholder.typicode.com/users/1
```

What is happening:

- the client sends a `GET` request
- the server processes it
- the server returns a response like `200 OK`
- the body contains JSON with the user data

This is the classic request-response model.

### Example 2 — Java `HttpClient`

```java
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

HttpClient client = HttpClient.newHttpClient();

HttpRequest request = HttpRequest.newBuilder()
    .uri(URI.create("https://jsonplaceholder.typicode.com/users/1"))
    .GET()
    .build();

HttpResponse<String> response =
    client.send(request, HttpResponse.BodyHandlers.ofString());

System.out.println(response.statusCode());
System.out.println(response.body());
```

This example matches the mental model directly:

1. build a request
2. send it
3. receive a response
4. inspect status code and body

Good mental model: **HTTP is usually resource-oriented**.

Example thought: `GET /users/1`

______________________________________________________________________

## DNS

### What is it?

DNS (**Domain Name System**) translates human-friendly names such as `google.com` or `api.example.com` into records, usually IP addresses.

Before a browser can send an HTTP request to `api.example.com`, it often needs DNS first.

Common DNS record types:

- **`A`** → IPv4 address
- **`AAAA`** → IPv6 address
- **`MX`** → mail server
- **`CNAME`** → alias to another name
- **`TXT`** → text records, often used in verification/configuration

### Example 1 — DNS lookup with `dig`

```bash
dig example.com A
```

Conceptually, the query is asking:

- domain: `example.com`
- record type: `A`

And the response may contain something like:

```text
example.com.  300  IN  A  93.184.216.34
```

Meaning:

- `example.com` has IPv4 address `93.184.216.34`
- `300` is the TTL (time to live)

### Example 2 — using the resolver from code

```python
import socket

addresses = socket.getaddrinfo("example.com", 80)
print(addresses[0])
```

Your code often does not talk to the DNS server directly.
It usually asks the OS resolver, and the OS performs the DNS request.

Good mental model: **DNS is name-to-record lookup**.

Example thought: “what address belongs to this host name?”

______________________________________________________________________

## SSH

### What is it?

SSH (**Secure Shell**) is a protocol for securely connecting to another machine.

It is used for:

- opening an interactive shell
- running commands remotely
- copying files with `scp` or `sftp`
- port forwarding/tunneling

Unlike plain text protocols, SSH first creates an **encrypted authenticated channel**, and then client and server exchange messages inside that channel.

### Example 1 — open a remote shell

```bash
ssh rafael@server.example.com
```

What happens conceptually:

1. connect to the remote server
2. negotiate encryption
3. authenticate the user
4. open a shell session

### Example 2 — run one remote command

```bash
ssh rafael@server.example.com "uptime"
```

This is still request-response:

- request: run `uptime`
- response: command output + exit status

### Example 3 — key-based auth and file copy

These commands already appear in your `commands.md` note:

```bash
ssh-keygen -t ed25519 -C "interas@gateway"
ssh-add ~/.ssh/id_ed25519
scp -r -P 77 ./instantclient_23_26 interas@131.100.25.67:~/
```

What they show:

- `ssh-keygen` creates a key pair
- `ssh-add` loads the private key into the SSH agent
- `scp` copies files over SSH

Good mental model: **SSH is secure remote access and remote execution**.

______________________________________________________________________

## RPC

### What is it?

RPC (**Remote Procedure Call**) is the idea of calling a function on another machine almost as if it were a local function.

Instead of thinking in terms of URLs/resources, RPC usually thinks in terms of **operations/methods**, such as:

- `GetUser(1)`
- `CreateInvoice(...)`
- `SendEmail(...)`

Important detail:

- RPC is **not one single protocol**
- it is a style that can be implemented by systems like **gRPC**, **JSON-RPC**, and **XML-RPC**

### Example 1 — JSON-RPC over HTTP

```http
POST /rpc HTTP/1.1
Content-Type: application/json

{
  "jsonrpc": "2.0",
  "method": "sum",
  "params": [4, 5],
  "id": 1
}
```

Response:

```json
{
  "jsonrpc": "2.0",
  "result": 9,
  "id": 1
}
```

The client is asking the server to execute an operation (`sum`), not fetch a resource like `/users/1`.

### Example 2 — gRPC service definition

```proto
service UserService {
  rpc GetUser(GetUserRequest) returns (User);
}

message GetUserRequest {
  int64 id = 1;
}

message User {
  int64 id = 1;
  string name = 2;
}
```

Client-side, the usage often looks like this:

```ts
const user = await client.getUser({ id: 1 });
```

It looks like a normal method call, but under the hood the client is serializing data, sending it across the network, and waiting for a response.

Good mental model: **RPC is operation-oriented**.

Example thought: `GetUser({ id: 1 })`

______________________________________________________________________

## How they connect in a real system

A common real flow is:

1. **DNS** resolves `api.example.com` to an IP address
2. the client opens a connection to that server
3. the client sends an **HTTP** request or an **RPC** call
4. an engineer later uses **SSH** to log into the machine and debug/deploy

So they are related, but each one solves a different problem:

- **DNS** finds where to go
- **HTTP** defines how web data is requested and returned
- **SSH** defines how to securely access and control a remote machine
- **RPC** defines how one program asks another program to execute an operation

______________________________________________________________________

## HTTP vs RPC

This is a common source of confusion:

- **HTTP** is a protocol
- **RPC** is a communication model

You can combine them:

- **JSON-RPC over HTTP**
- **gRPC over HTTP/2**

Rule of thumb:

- if you think in **URLs/resources**, you are usually closer to HTTP/REST style
- if you think in **methods/operations**, you are usually closer to RPC style

______________________________________________________________________

## Documentation

- HTTP: https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview
- DNS: https://developer.mozilla.org/en-US/docs/Glossary/DNS
- SSH: https://www.ssh.com/academy/ssh/protocol
- gRPC / RPC: https://grpc.io/docs/what-is-grpc/introduction/

______________________________________________________________________

## Related note

- [Request - Response](./Request - Response.md)
