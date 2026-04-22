# short polling

______________________________________________________________________

**Date:** 2026-04-12
**Tags:**[Udemy.md](tags/Udemy.md), [Backend_communication_design_patterns.md](tags/Backend_communication_design_patterns.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md), [Polling.md](tags/Polling.md)
**URL:**

______________________________________________________________________

## Whats short polling?

- CLient sends a request
- Server responds immediately with a handle
- Server continues to process the request
- Client uses that handle to check for status
- Multiple "short" request response as polls

![short polling](../assets/short%20polling.png)

## Pros and cons of short polling

- Pros:
  - Simple
  - Good for long running requests
  - Client can disconnect
- Cons:
  - Too chatty: not really escalable, especially if there are many clients
  - Network bandwidth: many requests and responses, even if there is no new data
  - Wasted backend resources: server has to handle many requests, even if there is no new data

## Demo js

[short_polling](./code/js/polling/short_polling.js)
