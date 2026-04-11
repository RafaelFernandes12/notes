# Java Interfaces

______________________________________________________________________

**Date:** 2026-03-15
**Tags:** [Java](../tags/Java.md), [OOP](../tags/OOP.md), [Types](../tags/Types.md)
**URL:** https://docs.oracle.com/javase/tutorial/java/IandI/createinterface.html

______________________________________________________________________

## Concept (what is it?)

An **interface** is a **type** that describes a **contract**: *“any class/record that implements this interface guarantees it provides these operations”*.

In Java, interfaces are mainly used to:

- model **capabilities** (e.g., `Comparable`, `Closeable`)
- enable **polymorphism** without sharing implementation
- decouple code via **Dependency Inversion** (depend on abstractions)
- support **multiple inheritance of type** (a class can implement many interfaces)

An interface can contain:

- **abstract methods** (implicitly `public abstract`)
- **default methods** (concrete methods with `default`)
- **static methods**
- **private methods** (helpers for default/static methods, Java 9+)
- **constants** (fields are implicitly `public static final`)
- nested types (`interface`, `class`, `enum`, `record`)

An interface **cannot**:

- have instance fields (only constants)
- have constructors

______________________________________________________________________

## How to declare (the syntax)

### Basic interface

```java
public interface Greeter {
  String greet(String name);
}
```

### Implementing an interface (class or record)

```java
public class EnglishGreeter implements Greeter {
  @Override
  public String greet(String name) {
    return "Hello, " + name;
  }
}

public record PortugueseGreeter() implements Greeter {
  @Override
  public String greet(String name) {
    return "Olá, " + name;
  }
}
```

### Multiple interfaces

```java
import java.io.Closeable;

public class FileLogger implements Closeable, Greeter {
  @Override
  public String greet(String name) { return "logged: " + name; }

  @Override
  public void close() { /* close file */ }
}
```

### Extending interfaces (interface-to-interface inheritance)

```java
public interface Named {
  String name();
}

public interface Employee extends Named {
  String employeeId();
}
```

### Default + static + private methods (Java 8+ / 9+)

```java
public interface IdFormatter {
  String rawId();

  default String formattedId() {
    return "ID-" + normalize(rawId());
  }

  static boolean looksValid(String id) {
    return id != null && !id.isBlank();
  }

  // Java 9+: helper to avoid duplicating code in defaults/statics
  private String normalize(String s) {
    return s == null ? "" : s.trim().toUpperCase();
  }
}
```

______________________________________________________________________

## How to use (examples)

### Example 1 — decoupling with an interface (Strategy / Dependency Inversion)

You can program against the interface and swap implementations easily.

```java
public interface PaymentGateway {
  String charge(String customerId, int cents);
}

public class StripeGateway implements PaymentGateway {
  @Override
  public String charge(String customerId, int cents) {
    return "stripe:ch_123";
  }
}

public class FakeGateway implements PaymentGateway {
  @Override
  public String charge(String customerId, int cents) {
    return "fake:ok"; // useful for tests
  }
}

public class CheckoutService {
  private final PaymentGateway gateway;

  public CheckoutService(PaymentGateway gateway) {
    this.gateway = gateway;
  }

  public String checkout(String customerId) {
    return gateway.charge(customerId, 4990);
  }
}
```

Why it’s good:

- `CheckoutService` doesn’t depend on Stripe directly
- tests can inject `FakeGateway`

______________________________________________________________________

### Example 2 — default methods for evolving APIs

Default methods are often used to add behavior without breaking all implementers.

```java
public interface JsonSerializable {
  String toJson();

  // added later, doesn’t break old implementations
  default byte[] toJsonBytes() {
    return toJson().getBytes(java.nio.charset.StandardCharsets.UTF_8);
  }
}

public record UserDto(String id, String email) implements JsonSerializable {
  @Override
  public String toJson() {
    return "{\"id\":\"" + id + "\",\"email\":\"" + email + "\"}";
  }
}
```

Notes:

- default methods are **real implementation**; use them carefully (they can hide complexity)
- if two interfaces provide the same default method, the implementing class must resolve the conflict

______________________________________________________________________

### Example 3 — functional interfaces (lambdas)

A **functional interface** has exactly one abstract method, so it can be implemented with a lambda.

```java
@FunctionalInterface
public interface Validator<T> {
  boolean isValid(T value);

  default Validator<T> and(Validator<T> other) {
    return v -> this.isValid(v) && other.isValid(v);
  }
}

class Demo {
  void run() {
    Validator<String> notBlank = s -> s != null && !s.isBlank();
    Validator<String> hasAt = s -> s != null && s.contains("@");

    var emailValidator = notBlank.and(hasAt);
    System.out.println(emailValidator.isValid("rafael@example.com"));
  }
}
```

This style is common in Java APIs like `Predicate<T>`, `Function<T, R>`, `Comparator<T>`.

______________________________________________________________________

## Pros and cons

### Pros

- **Decoupling**: consumers depend on a stable contract, not concrete classes
- **Testability**: easy to substitute fakes/mocks
- **Multiple inheritance of type**: one class can implement many interfaces
- **API design**: small capability interfaces are composable (`Runnable`, `Closeable`, etc.)
- **Evolution**: `default` methods can add behavior without forcing immediate changes everywhere

### Cons / trade-offs

- **Over-abstraction**: too many interfaces can make code harder to follow
- **Versioning pain**: adding a new abstract method breaks implementers (default methods mitigate, but can introduce surprises)
- **Default method complexity**: ambiguity/conflicts when multiple interfaces define the same default signature
- **No shared state**: if you need shared fields/invariants, you’ll need composition or a base class

______________________________________________________________________

## Practical guidelines

- Prefer **small, capability-based** interfaces over “god interfaces”.
- Introduce an interface when you have:
  - multiple implementations, or
  - you want to decouple a high-level module from details (e.g., external providers), or
  - you want to model a capability (e.g., `Auditable`, `Cacheable`).
- If you only ever expect **one implementation**, a simple class may be clearer.
- Be careful with default methods in public APIs: treat them like shipped code.

______________________________________________________________________

## Related docs

- Oracle Java Tutorial — Interfaces: https://docs.oracle.com/javase/tutorial/java/IandI/createinterface.html
- JLS §9 (Interfaces): https://docs.oracle.com/javase/specs/jls/se21/html/jls-9.html
- `@FunctionalInterface`: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/FunctionalInterface.html
