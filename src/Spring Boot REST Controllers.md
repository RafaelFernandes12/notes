# Spring Boot REST Controllers

______________________________________________________________________

**Date:** 2026-03-02
**Tags:** [Spring_Boot](../tags/Spring_Boot.md), [Java](../tags/Java.md), [Rest](../tags/Rest.md)
**URL:** https://spring.io/guides/gs/rest-service

______________________________________________________________________

## Concept (what is it?)

In Spring Boot, a **REST controller** is a class annotated with `@RestController` that:

1. **Maps HTTP requests** (GET/POST/PUT/DELETE) to methods (`@GetMapping`, `@PostMapping`, etc.)
1. **Returns a “resource representation”** (usually JSON)
1. Lets Spring handle **serialization** (commonly via Jackson) and HTTP response creation

This is different from a traditional MVC `@Controller` that usually returns a *view* (HTML). `@RestController` is basically `@Controller + @ResponseBody`.

______________________________________________________________________

## Example 1 — GET endpoint returning JSON (+ query param)

Resource representation (DTO) using a Java `record`:

```java
package com.example.restservice;

public record Greeting(long id, String content) {}
```

Controller mapping `GET /greeting` and binding `?name=` from the query string:

```java
package com.example.restservice;

import java.util.concurrent.atomic.AtomicLong;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class GreetingController {

  private static final String TEMPLATE = "Hello, %s!";
  private final AtomicLong counter = new AtomicLong();

  @GetMapping("/greeting")
  public Greeting greeting(@RequestParam(defaultValue = "World") String name) {
    return new Greeting(counter.incrementAndGet(), TEMPLATE.formatted(name));
  }
}
```

How it behaves:

```bash
curl 'http://localhost:8080/greeting'
# {"id":1,"content":"Hello, World!"}

curl 'http://localhost:8080/greeting?name=User'
# {"id":2,"content":"Hello, User!"}
```

Notes:

- `@RequestParam` binds query params (`?name=...`). `defaultValue` is used when the param is missing.
- Controllers are **singletons by default** in Spring, so any field state is shared across requests. If you keep state (like `counter`), it must be thread-safe (here `AtomicLong` is used just for the demo).

______________________________________________________________________

## Example 2 — Path variable + explicit status codes (ResponseEntity)

Using `@PathVariable` for `GET /customers/{id}` and returning `404` when not found:

```java
package com.example.api;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class CustomerController {

  public record Customer(String id, String name) {}

  // Demo-only “repository”
  private final Map<String, Customer> customers = new ConcurrentHashMap<>(Map.of(
      "1", new Customer("1", "Matheus"),
      "2", new Customer("2", "Rafael")
  ));

  @GetMapping("/customers/{id}")
  public ResponseEntity<Customer> getCustomer(@PathVariable String id) {
    var customer = customers.get(id);
    if (customer == null) {
      return ResponseEntity.notFound().build();
    }
    return ResponseEntity.ok(customer);
  }
}
```

Why `ResponseEntity` is useful:

- You can control **status code**, **headers**, and **body**.
- For “not found” cases, returning `404` is usually better than returning `null`.

______________________________________________________________________

## Example 3 — POST with JSON body (+ validation) and 201 Created

Creating resources typically uses `@RequestBody` and returns `201 Created`.

```java
package com.example.api;

import java.net.URI;
import java.util.UUID;

import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.util.UriComponentsBuilder;

@RestController
public class AppointmentController {

  public record CreateAppointmentRequest(
      @NotBlank String customerName,
      @NotBlank String serviceName
  ) {}

  public record Appointment(String id, String customerName, String serviceName) {}

  @PostMapping("/appointments")
  public ResponseEntity<Appointment> create(
      @Valid @RequestBody CreateAppointmentRequest body,
      UriComponentsBuilder uri
  ) {
    var id = UUID.randomUUID().toString();
    var created = new Appointment(id, body.customerName(), body.serviceName());

    URI location = uri.path("/appointments/{id}").buildAndExpand(id).toUri();
    return ResponseEntity.created(location).body(created);
  }
}
```

Request example:

```bash
curl -i -X POST 'http://localhost:8080/appointments' \
  -H 'content-type: application/json' \
  -d '{"customerName":"Rafael","serviceName":"Corte"}'
```

Validation note:

- `@Valid` + `jakarta.validation` annotations usually require a validation starter/dependency (commonly `spring-boot-starter-validation`).

______________________________________________________________________

## Running & packaging (Maven Wrapper)

```bash
./mvnw spring-boot:run

./mvnw clean package
java -jar target/*.jar
```

______________________________________________________________________

## Links

- Guide: https://spring.io/guides/gs/rest-service
- `@RestController`: https://docs.spring.io/spring-framework/docs/current/javadoc-api/org/springframework/web/bind/annotation/RestController.html
- `@GetMapping`: https://docs.spring.io/spring-framework/docs/current/javadoc-api/org/springframework/web/bind/annotation/GetMapping.html
- `@RequestParam`: https://docs.spring.io/spring-framework/docs/current/javadoc-api/org/springframework/web/bind/annotation/RequestParam.html
- `ResponseEntity`: https://docs.spring.io/spring-framework/docs/current/javadoc-api/org/springframework/http/ResponseEntity.html
- Jackson (JSON): https://github.com/FasterXML/jackson
