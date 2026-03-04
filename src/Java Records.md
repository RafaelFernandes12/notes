# Java Records

______________________________________________________________________

**Date:** 2026-03-02
**Tags:** [Java](../tags/Java.md), [Records](../tags/Records.md), [Types](../tags/Types.md), [Validation](../tags/Validation.md), [Spring_Boot](../tags/Spring_Boot.md)
**URL:** https://docs.oracle.com/en/java/javase/14/language/records.html#GUID-6699E26F-4A9B-4393-A08B-1E47D4B2D263

______________________________________________________________________

## Concept (what is it?)

A **record** is a special kind of class meant to model **immutable data** (*“a transparent carrier for immutable data”*).

When you declare a record, the compiler derives (from the **record components** in the header):

- `private final` fields
- a **public accessor** method for each component (named exactly like the component)
- a **canonical constructor** with the same parameter list as the header
- `equals(...)`, `hashCode()`, `toString()`

Records:

- implicitly extend `java.lang.Record`
- cannot extend another class (but **can implement interfaces**)
- are implicitly `final` (can’t be subclassed)
- can declare methods, static fields, and constructors (with some rules)

---

## How to declare (the syntax)

The minimal declaration is:

```java
record Point(int x, int y) {}
```

A more “complete” declaration can include visibility, type parameters, and interfaces:

```java
import java.io.Serializable;
import java.util.List;

public record Page<T>(List<T> items, int page, int size) implements Serializable {}
```

Where:

- `Point` is the record name
- `int x` and `int y` are **record components**
- `{}` is the body (optional if empty)

The compiler behaves *as if* you had written something close to this:

```java
final class Point extends java.lang.Record {
  private final int x;
  private final int y;

  Point(int x, int y) {
    this.x = x;
    this.y = y;
  }

  public int x() { return x; }
  public int y() { return y; }

  // equals / hashCode / toString generated
}
```

---

## How to use (examples)

### Example 1 — simple data carrier (DTO / value object)

```java
record Customer(String id, String name) {}

class Demo {
  void run() {
    var c = new Customer("c-1", "Rafael");

    // accessors are component names, not getX()
    System.out.println(c.id());
    System.out.println(c.name());

    // useful toString/equals out of the box
    System.out.println(c);
    System.out.println(c.equals(new Customer("c-1", "Rafael")));
  }
}
```

---

### Example 2 — validation/invariants with a compact constructor

If a record must always be valid, put the rule in the constructor.

```java
import java.time.Instant;

public record TimeRange(Instant start, Instant end) {
  public TimeRange {
    if (start == null || end == null) {
      throw new IllegalArgumentException("start/end cannot be null");
    }
    if (end.isBefore(start)) {
      throw new IllegalArgumentException("end must be >= start");
    }
  }
}
```

This is a **compact canonical constructor**:

- it has no parameter list
- its parameters are *implicitly* the same as the record header
- after your checks, the compiler assigns the fields from the (possibly adjusted) parameters

Alternative constructors must delegate to the canonical one:

```java
import java.time.Instant;
import java.time.temporal.ChronoUnit;

public record TimeRange(Instant start, Instant end) {
  public TimeRange {
    if (end.isBefore(start)) throw new IllegalArgumentException("end < start");
  }

  public TimeRange(Instant start, long minutes) {
    this(start, start.plus(minutes, ChronoUnit.MINUTES));
  }
}
```

---

### Example 3 — records can have behavior (methods) and implement interfaces

```java
public record Money(long cents, String currency) implements Comparable<Money> {
  public Money {
    if (currency == null || currency.isBlank()) {
      throw new IllegalArgumentException("currency is required");
    }
  }

  public Money plus(Money other) {
    if (!this.currency.equals(other.currency)) {
      throw new IllegalArgumentException("different currencies");
    }
    return new Money(this.cents + other.cents, this.currency);
  }

  @Override
  public int compareTo(Money o) {
    if (!this.currency.equals(o.currency)) {
      throw new IllegalArgumentException("different currencies");
    }
    return Long.compare(this.cents, o.cents);
  }
}
```

---

### Example 4 — keep immutability when components are mutable

Records make the *fields* final, but a component type can still be mutable (e.g. `List`).

If you want deep immutability, normalize/copy in the constructor and/or the accessor.

```java
import java.util.List;

public record Tags(List<String> values) {
  public Tags {
    values = List.copyOf(values); // defensive copy
  }

  @Override
  public List<String> values() {
    return values; // already unmodifiable because of copyOf
  }
}
```

---

### Example 5 — Jakarta Validation annotations on record components

You can annotate **record components** with Jakarta Validation constraints (common in Spring Boot DTOs):

```java
import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;

public record CreateUserRequest(
  @NotBlank @Size(max = 80) String name,
  @Email @NotBlank String email,
  @Min(18) int age
) {}
```

In Spring Boot, validation is typically triggered with `@Valid`:

```java
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
class UserController {
  @PostMapping("/users")
  void create(@Valid @RequestBody CreateUserRequest body) {}
}
```

---

## Rules/constraints that matter in practice

- **No extra instance fields**: the instance state is defined by the record components.
  - You *can* declare **static** fields.
- **Constructors**:
  - the canonical constructor parameter list matches the header
  - additional constructors must delegate to the canonical one via `this(...)`
- **Inheritance**:
  - records can’t `extends` other classes
  - records can `implements` interfaces
- **Accessors** are named exactly like components (`name()`), which can affect conventions (e.g., serialization frameworks).

---

## Links

- Oracle docs — Records (Java 14+): https://docs.oracle.com/en/java/javase/14/language/records.html
- Jakarta Bean Validation: https://jakarta.ee/specifications/bean-validation/
