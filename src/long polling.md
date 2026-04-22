# short polling

______________________________________________________________________

**Date:** 2026-04-12
**Tags:**[Udemy.md](tags/Udemy.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md),[Backend_communication_design_patterns.md](tags/Backend_communication_design_patterns.md), [Polling.md](tags/Polling.md)
**URL:**

______________________________________________________________________

## Where request/response and polling inst ideal

- A request takes long time to process
  - Upload a youtube video
- The backend want to send notification
  - A user just logged in
- Short polling is a good but chatty solution for this problem
- Meet long polling (kafka uses it)

## What is long polling?

- Client sends a request
- Server responds immediately with a handle
- Server continues to process the request
- Client uses that handle to check for status
- Server doesnt respond until it has the response
- So we got a handle, we can disconnect and reconnect later to check for status, less chatty than short polling

![long polling](../assets/long%20polling.png)
