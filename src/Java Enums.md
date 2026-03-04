# Java Enums

______________________________________________________________________

**Date:** 2026-03-02
**Tags:** [Java](../tags/Java.md), [Enums](../tags/Enums.md), [Types](../tags/Types.md), [Spring_Boot](../tags/Spring_Boot.md), [Rest](../tags/Rest.md), [BancoDeDados](../tags/BancoDeDados.md)
**URL:** https://docs.oracle.com/javase/specs/jls/se21/html/jls-8.html#jls-8.9

______________________________________________________________________

## Concept (what is it?)

An **enum** is a special Java type for representing a **fixed set of constants**.

Key properties:

- each constant is a **singleton instance** of the enum type
- enums can have **fields, methods, and constructors**
- enums are great for **domain states** (status, role, type) and reduce “stringly-typed” code

---

## How to use (examples)

### Example 1 — enum with fields + method (domain-friendly)

```java
public enum AppointmentStatus {
  PENDING(false),
  CONFIRMED(true),
  CANCELLED(false);

  private final boolean blocksSlot;

  AppointmentStatus(boolean blocksSlot) {
    this.blocksSlot = blocksSlot;
  }

  public boolean blocksSlot() {
    return blocksSlot;
  }
}
```

---

### Example 2 — per-constant behavior (strategy)

```java
import java.math.BigDecimal;

public enum DiscountPolicy {
  NONE {
    @Override public BigDecimal apply(BigDecimal total) {
      return total;
    }
  },
  TEN_PERCENT {
    @Override public BigDecimal apply(BigDecimal total) {
      return total.multiply(new BigDecimal("0.90"));
    }
  };

  public abstract BigDecimal apply(BigDecimal total);
}
```

This replaces `if/else` or `switch` chains with polymorphism.

---

### Example 3 — enums in Spring Boot REST (binding + validation)

Spring can bind enums from query params/path variables when names match.

```java
import jakarta.validation.constraints.NotNull;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

enum Channel { EMAIL, SMS }

@Validated
@RestController
class NotificationController {
  @GetMapping("/notifications")
  String list(@RequestParam @NotNull Channel channel) {
    return "channel=" + channel;
  }
}
```

---

### Example 4 — enums with JPA: `EnumType.STRING` vs `ORDINAL`

```java
import jakarta.persistence.*;

@Entity
class Appointment {
  @Id @GeneratedValue
  Long id;

  @Enumerated(EnumType.STRING)
  AppointmentStatus status;
}
```

Prefer `EnumType.STRING` so reordering enum constants won’t change stored meaning.

---

## Practical tips

- Use `EnumSet` / `EnumMap` when you need collections keyed by enums.

```java
import java.util.EnumSet;

EnumSet<AppointmentStatus> terminal = EnumSet.of(AppointmentStatus.CANCELLED);
```

- If your API needs stable string values (`"confirmed"` instead of `CONFIRMED`), store an explicit code:

```java
public enum AppointmentStatus {
  CONFIRMED("confirmed"),
  CANCELLED("cancelled");

  private final String code;
  AppointmentStatus(String code) { this.code = code; }
  public String code() { return code; }
}
```

---

## Links

- JLS — Enum Types: https://docs.oracle.com/javase/specs/jls/se21/html/jls-8.html#jls-8.9
- Java tutorial — Enums: https://docs.oracle.com/javase/tutorial/java/javaOO/enum.html
- Hibernate — `@Enumerated`: https://docs.jboss.org/hibernate/orm/current/userguide/html_single/Hibernate_User_Guide.html#basic-enums
