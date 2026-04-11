# Fetching Data in Java (HTTP Clients)

______________________________________________________________________

**Date:** 2026-03-25
**Tags:** [Java](../tags/Java.md), [Rest](../tags/Rest.md), [Spring_Boot](../tags/Spring_Boot.md), [HTTP](../tags/HTTP.md)
**URL:** https://docs.oracle.com/en/java/javase/21/docs/api/java.net.http/java/net/http/HttpClient.html

______________________________________________________________________

## Concept (what is it?)

When people say “fetch data” in Java, they usually mean **making an HTTP request to another service** and reading the response.

In Java, the most common options are:

- **`HttpClient`** (`java.net.http`) → built into Java 11+, closest to JS `fetch`
- **`RestClient`** (Spring) → simple synchronous client, often feels closer to Axios
- **`WebClient`** (Spring WebFlux) → asynchronous/non-blocking client
- **`RestTemplate`** → older Spring client; still found in older codebases, but not the preferred choice for new code

So the basic flow is:

1. build a request
2. send it
3. inspect the status code
4. read the body
5. map JSON into Java objects when needed

______________________________________________________________________

## Quick mental map

If you come from JavaScript:

- `fetch(...)` in JS → `HttpClient` in plain Java
- `axios.get(...)` in JS → `RestClient` in Spring
- if you need reactive/non-blocking flows → `WebClient`

Rule of thumb:

- **plain Java app** → use `HttpClient`
- **Spring Boot app, normal sync flow** → use `RestClient`
- **Spring WebFlux/reactive app** → use `WebClient`

______________________________________________________________________

## Example 1 — Plain Java with `HttpClient`

This is the closest built-in option to JS `fetch`.

```java
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

public class FetchUserExample {

  public static void main(String[] args) throws Exception {
    HttpClient client = HttpClient.newHttpClient();

    HttpRequest request = HttpRequest.newBuilder()
        .uri(URI.create("https://jsonplaceholder.typicode.com/users/1"))
        .GET()
        .build();

    HttpResponse<String> response = client.send(
        request,
        HttpResponse.BodyHandlers.ofString()
    );

    System.out.println(response.statusCode());
    System.out.println(response.body());
  }
}
```

What this shows:

- `HttpClient` is the HTTP client itself
- `HttpRequest` describes the request
- `client.send(...)` performs the request synchronously
- `BodyHandlers.ofString()` says “give me the response body as text”

This is good when:

- you are not in Spring
- you want something built into the JDK
- you are okay handling serialization/errors more manually

### Mapping JSON into a Java object

Often you do not want the raw JSON string. You want a Java type.

```java
import com.fasterxml.jackson.databind.ObjectMapper;

record UserResponse(int id, String name, String email) {}

ObjectMapper objectMapper = new ObjectMapper();

HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());

if (response.statusCode() == 200) {
  UserResponse user = objectMapper.readValue(response.body(), UserResponse.class);
  System.out.println(user.name());
}
```

This is a common Java pattern:

- fetch as string
- deserialize with Jackson

______________________________________________________________________

## Example 2 — Spring `RestClient`

In a Spring Boot application, `RestClient` is usually the nicest synchronous choice for external APIs.

```java
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestClient;

record CustomerResponse(Long id, String name, String contact) {}

@Service
public class CustomerGateway {

  private final RestClient restClient;

  public CustomerGateway(RestClient.Builder builder) {
    this.restClient = builder
        .baseUrl("https://api.example.com")
        .build();
  }

  public CustomerResponse fetchCustomer(Long id) {
    return restClient.get()
        .uri("/customers/{id}", id)
        .retrieve()
        .body(CustomerResponse.class);
  }
}
```

Why this is nicer than raw `HttpClient` in Spring:

- less boilerplate
- easier JSON → object mapping
- integrates naturally with Spring configuration
- better for “service calls another service” style code

### POST example with JSON body

```java
import org.springframework.http.MediaType;

record SendMessageRequest(String phone, String message) {}
record SendMessageResponse(String status, String providerId) {}

public SendMessageResponse sendMessage(String phone, String message) {
  return restClient.post()
      .uri("/messages")
      .contentType(MediaType.APPLICATION_JSON)
      .body(new SendMessageRequest(phone, message))
      .retrieve()
      .body(SendMessageResponse.class);
}
```

This feels very similar to Axios:

- choose method
- provide URL
- provide body
- get mapped response object back

______________________________________________________________________

## Example 3 — Spring `WebClient`

`WebClient` is useful when you want **non-blocking** calls or are already in a reactive application.

```java
import org.springframework.stereotype.Service;
import org.springframework.web.reactive.function.client.WebClient;
import reactor.core.publisher.Mono;

record OrderResponse(Long id, String status) {}

@Service
public class OrderGateway {

  private final WebClient webClient;

  public OrderGateway(WebClient.Builder builder) {
    this.webClient = builder
        .baseUrl("https://api.example.com")
        .build();
  }

  public Mono<OrderResponse> fetchOrder(Long id) {
    return webClient.get()
        .uri("/orders/{id}", id)
        .retrieve()
        .bodyToMono(OrderResponse.class);
  }
}
```

Main difference here:

- this returns `Mono<OrderResponse>`, not `OrderResponse`
- the HTTP call is part of a reactive pipeline
- you do not usually call this style unless you are already using reactive programming intentionally

For many normal Spring Boot apps, `RestClient` is simpler.

______________________________________________________________________

## Error handling patterns

### With `HttpClient`

You usually check the status code yourself:

```java
HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());

if (response.statusCode() >= 400) {
  throw new RuntimeException("Request failed: " + response.statusCode());
}
```

### With `RestClient`

You can keep the call small, but you still need to think about non-2xx responses and timeouts.

```java
CustomerResponse customer = restClient.get()
    .uri("/customers/{id}", id)
    .retrieve()
    .body(CustomerResponse.class);
```

In real projects, you often add:

- timeout configuration
- auth headers (`Authorization: Bearer ...`)
- custom exception mapping
- logging around external requests

______________________________________________________________________

## Which one should I pick?

### Pick `HttpClient` when:

- you want no Spring dependency
- you are writing a small utility or CLI
- you want JDK-only code

### Pick `RestClient` when:

- you are in Spring Boot
- the flow is synchronous
- you want readable service-layer code

### Pick `WebClient` when:

- you are already using WebFlux/reactive programming
- you need non-blocking HTTP calls
- you are composing async pipelines

______________________________________________________________________

## Dependencies / setup notes

- `HttpClient` → built into Java 11+
- `RestClient` → available through Spring Web / `spring-boot-starter-web`
- `WebClient` → available through WebFlux / `spring-boot-starter-webflux`

So in a Spring Boot app, the HTTP client you choose is often related to whether you have:

- `starter-web` → `RestClient`
- `starter-webflux` → `WebClient`

______________________________________________________________________

## Practical summary

- Java absolutely can “fetch data” from APIs
- the core idea is the same as in JS: request → response → parse JSON
- the main difference is which client style you want:
  - built-in JDK (`HttpClient`)
  - Spring sync (`RestClient`)
  - Spring reactive (`WebClient`)

For most Spring Boot CRUD/business apps, **`RestClient` is the most practical default**.

______________________________________________________________________

## Links

- Java `HttpClient`: https://docs.oracle.com/en/java/javase/21/docs/api/java.net.http/java/net/http/HttpClient.html
- Java `HttpRequest`: https://docs.oracle.com/en/java/javase/21/docs/api/java.net.http/java/net/http/HttpRequest.html
- Spring `RestClient`: https://docs.spring.io/spring-framework/reference/integration/rest-clients.html
- Spring `WebClient`: https://docs.spring.io/spring-framework/reference/web/webflux-webclient.html
- Jackson: https://github.com/FasterXML/jackson
