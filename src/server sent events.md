# server side events

______________________________________________________________________

**Date:** 2026-04-12
**Tags:**[Udemy.md](tags/Udemy.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md)
**URL:**

______________________________________________________________________

## What is server sent events?

- A response has start and end
- Client sends a request
- Server sends logical events as part of response
- Server never writes the end of the response
- Its still a request but and unending response
- Client parses the streams data looking for a event
- It works with http and https

![server side](../assets/server%20side.png)

## pros and cons

- Pros:
  - Real time
  - Works with http and https, and request/response
- Cons:
  - clients must be online
  - clients might not be able to handle
  - polling is preferred for light clients
  - http/1.1 problem (6 connections per host) (http/2 solves this problem)
