# Request - Response

______________________________________________________________________

**Date:** 2026-04-11
**Tags:**[Udemy.md](tags/Udemy.md), [Backend_communication_design_patterns.md](tags/Backend_communication_design_patterns.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md)
**URL:**

______________________________________________________________________

## Request - Response

- Client sends a request
- Server parses the request
- Server processes the request
- Server sends a response
- Client parses the response and consume

## Where it is used?

- RPC (remote procedure call)
- SQL and database protocols
- APIs (REST/SOAP/GraphQL)
- Web, http, dns, ssh
- [./Common Request-Response Protocols.md]

## Anatomy of a request/response

- A request structure is defined by both client and server
- Request has a boundary
- Defined by a protocol and message format
- Same for the response
- E.g Http Request

```js
GET / HTTP/1.1
Headers
<CRLF>
BODY
```

## Building an upload image service with request response

- Send a large request with the image (simple)
- Chunk image and send a request per chunk (resumable)

![chunk sending](../assets/chunk%20sending.png)

## Doesnt work everywhere

- Notification service
- Chatting application
- Very long running request: Takes too much time to process and client may timeout

## Demo

this is a simple demo to show the request and response in action. We will use curl to send a request to google.com and see the response.

```bash
curl -v --trace out.txt http://google.com
cat out.txt
```
