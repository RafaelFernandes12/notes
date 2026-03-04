# Jakarta Validation (Bean Validation)

______________________________________________________________________

**Date:** 2026-03-02
**Tags:** [Java](../tags/Java.md), [Spring_Boot](../tags/Spring_Boot.md), [Validation](../tags/Validation.md), [Maven](../tags/Maven.md)
**URL:** https://jakarta.ee/specifications/bean-validation/

______________________________________________________________________

## Concept (what is it?)

**Jakarta Validation** (a.k.a. *Bean Validation*) is the Java standard for validating objects using annotations like `@NotNull` and `@Size`.

It defines the **API/spec** (`jakarta.validation.*`). To actually *run* validations, you also need an implementation (most commonly **Hibernate Validator**).

In Spring Boot apps, you typically use the Spring Boot starter that brings everything wired correctly.

______________________________________________________________________

## How to install (Maven)

### Option A (recommended) — Spring Boot Starter

Add this to `pom.xml`:

```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-validation</artifactId>
</dependency>
```

Why this is the usual choice:

- brings `jakarta.validation-api` (annotations/API)
- brings an implementation (usually `hibernate-validator`)
- Spring Boot auto-configures validation so `@Valid` works in controllers

### Option B — API only (rarely enough by itself)

If you add only the API, you get the annotations, but you might *not* have a provider to execute validation:

```xml
<dependency>
  <groupId>jakarta.validation</groupId>
  <artifactId>jakarta.validation-api</artifactId>
</dependency>
```

If you go this route, you’ll also need an implementation (example):

```xml
<dependency>
  <groupId>org.hibernate.validator</groupId>
  <artifactId>hibernate-validator</artifactId>
</dependency>
```

______________________________________________________________________

## How to use in Spring Boot

### Example 1 — validate `@RequestBody` with `@Valid`

DTO using common constraints:

```java
import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;

public record CreateCustomerRequest(
  @NotBlank @Size(max = 80) String name,
  @Email String email
) {}
```

Controller triggers validation:

```java
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

@RestController
class CustomerController {
  @PostMapping("/customers")
  public void create(@Valid @RequestBody CreateCustomerRequest body) {
    // if invalid, Spring returns 400 with details
  }
}
```

### Example 2 — validate query params / path vars with `@Validated`

```java
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

@Validated
@RestController
class AppointmentQueryController {

  @GetMapping("/appointments")
  public String list(
      @RequestParam @NotBlank String providerId,
      @RequestParam(defaultValue = "0") @Min(0) int page
  ) {
    return "ok";
  }
}
```

______________________________________________________________________

## Common constraints (cheat sheet)

### Null / empty

- `@NotNull` (cannot be null)
- `@NotBlank` (String: not null, not empty, not whitespace)
- `@NotEmpty` (String/Collection/Map/Array: not null and size > 0)

### Size / length

- `@Size(min=, max=)` (String/Collection/Map/Array)

### Numbers

- `@Min`, `@Max`
- `@Positive`, `@PositiveOrZero`
- `@Negative`, `@NegativeOrZero`
- `@DecimalMin`, `@DecimalMax`
- `@Digits(integer=, fraction=)`

### Strings

- `@Email`
- `@Pattern(regexp="...")`

### Date/time

- `@Past`, `@PastOrPresent`
- `@Future`, `@FutureOrPresent`

### Booleans

- `@AssertTrue`, `@AssertFalse`

### Nested objects

- `@Valid` (cascades validation)

______________________________________________________________________

## Tips

- Use Jakarta constraints for portability. Only use `org.hibernate.validator.constraints.*` if you need extra constraints.
- For cross-field rules (example: `endAt` must be after `startAt`), create a **custom constraint** (annotation + validator) or validate in the service layer.

______________________________________________________________________

## Links

- Jakarta Bean Validation spec: https://jakarta.ee/specifications/bean-validation/
- Spring Boot validation docs: https://docs.spring.io/spring-boot/reference/io/validation.html
- Hibernate Validator (implementation): https://hibernate.org/validator/
