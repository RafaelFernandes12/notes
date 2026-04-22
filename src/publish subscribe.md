# publish subscribe

______________________________________________________________________

**Date:** 2026-04-12
**Tags:**[Udemy.md](tags/Udemy.md), [Fundamentals_of_backend_engineering.md](tags/Fundamentals_of_backend_engineering.md), [Backend_communication_design_patterns.md](tags/Backend_communication_design_patterns.md)
**URL:**

______________________________________________________________________

## Where it breaks with request and response

![pub sub breaks](../assets/pub%20sub%20breaks.png)

- pros: elegant, simple, scalable
- cons: bad for multiple receivers, high coupling, client/server have to be running, chaining, circuit breaking

## pub sub cool

![pub sub true](../assets/pub%20sub%20true.png)

- pros: scales with multiple receivers, great for microservices, loose coupling, works while clients not running
- cons: message delivery issues, complexity, network saturation

## demo js with rabbitmq
