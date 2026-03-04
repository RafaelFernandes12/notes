# Java Date and Time (java.time)

______________________________________________________________________

**Date:** 2026-03-02
**Tags:** [Java](../tags/Java.md), [Java_Time](../tags/Java_Time.md), [Types](../tags/Types.md), [Rest](../tags/Rest.md), [BancoDeDados](../tags/BancoDeDados.md)
**URL:** https://docs.oracle.com/javase/8/docs/api/java/time/package-summary.html

______________________________________________________________________

## Concept (what is it?)

`java.time` is the modern Java date/time API (JSR-310). It replaces the legacy `java.util.Date` / `Calendar` APIs with types that model *exactly* what you mean.

Pick the type based on meaning:

- **Date only (no time):** `LocalDate` (e.g. `2026-03-02`)
- **Time only (no date):** `LocalTime` (e.g. `14:30`)
- **Date + time, but no timezone:** `LocalDateTime` (wall-clock; use carefully, e.g. `2026-03-02T14:30`)
- **A moment in time (UTC timestamp):** `Instant` (great for storage/comparisons, e.g. `2026-03-02T17:30:00Z`)
- **Date + time + offset:** `OffsetDateTime` (e.g. `2026-03-02T14:30-03:00`, great for APIs)
- **Date + time + named timezone:** `ZonedDateTime` (good for business rules and DST, e.g. `2026-03-02T14:30-03:00[America/Sao_Paulo]`)

Rule of thumb for scheduling/agendamento systems:

- **Store/compare:** `Instant` (UTC)
- **Accept/return in API:** `OffsetDateTime` (or `Instant`)
- Keep the **business timezone** separately (e.g. `ZoneId` string like `America/Sao_Paulo`) for display and slot rules.

---

## How to use (examples)

### Example 1 — date-only vs time-only (forms and filters)

```java
import java.time.LocalDate;
import java.time.LocalTime;

LocalDate day = LocalDate.parse("2026-03-02");
LocalTime start = LocalTime.parse("14:30");
```

Typical use cases:

- `LocalDate`: “show available slots for **a day**”
- `LocalTime`: “working hours start at **09:00**”

---

### Example 2 — “moment in time” for storage + comparisons (`Instant`)

```java
import java.time.Instant;

Instant createdAt = Instant.now();
Instant appointmentStart = Instant.parse("2026-03-02T17:30:00Z");

if (appointmentStart.isBefore(createdAt)) {
  throw new IllegalStateException("appointment cannot start in the past");
}
```

Why `Instant` helps:

- unambiguous (UTC)
- easy ordering/comparison
- avoids DST/timezone surprises when persisted

---

### Example 3 — API-friendly timestamps (`OffsetDateTime`)

If your API wants to preserve the client offset (`-03:00` etc.), use `OffsetDateTime`.

```java
import java.time.OffsetDateTime;

OffsetDateTime start = OffsetDateTime.parse("2026-03-02T14:30:00-03:00");
```

Convert to storage (`Instant`):

```java
import java.time.Instant;
import java.time.OffsetDateTime;

Instant startUtc = start.toInstant();
```

---

### Example 4 — business rules in a named timezone (`ZonedDateTime` + `ZoneId`)

Use a named zone (e.g. `America/Sao_Paulo`) for rules like “business opens at 09:00 *local time*” and for DST correctness.

```java
import java.time.*;

ZoneId businessZone = ZoneId.of("America/Sao_Paulo");

// A stored Instant shown in local business time
Instant startUtc = Instant.parse("2026-03-02T17:30:00Z");
ZonedDateTime local = startUtc.atZone(businessZone);

LocalDate day = local.toLocalDate();
LocalTime time = local.toLocalTime();
```

When you *build* slots for a day:

```java
import java.time.*;

ZoneId zone = ZoneId.of("America/Sao_Paulo");
LocalDate day = LocalDate.of(2026, 3, 2);

ZonedDateTime firstSlot = day.atTime(9, 0).atZone(zone);
ZonedDateTime lastSlot  = day.atTime(17, 0).atZone(zone);

// for persistence
Instant firstSlotUtc = firstSlot.toInstant();
Instant lastSlotUtc  = lastSlot.toInstant();
```

---

### Example 5 — avoid `LocalDateTime` for persistence (ambiguity)

`LocalDateTime` has **no offset/zone**. That means it can be ambiguous around DST transitions.

Use it for “wall-clock” values that only make sense *within a known zone* (and keep the zone alongside), or convert immediately to `Instant`.

```java
import java.time.*;

LocalDateTime wallClock = LocalDateTime.parse("2026-03-02T14:30:00");
ZoneId zone = ZoneId.of("America/Sao_Paulo");

Instant utc = wallClock.atZone(zone).toInstant();
```

---

### Example 6 — durations vs periods (time vs calendar)

- `Duration`: time-based (seconds/nanos)
- `Period`: date-based (years/months/days)

```java
import java.time.*;

Duration serviceDuration = Duration.ofMinutes(45);
Instant end = Instant.parse("2026-03-02T17:30:00Z").plus(serviceDuration);

Period reminderLead = Period.ofDays(1);
LocalDate tomorrow = LocalDate.of(2026, 3, 2).plus(reminderLead);
```

---

### Example 7 — formatting/parsing with `DateTimeFormatter` (stable output)

```java
import java.time.*;
import java.time.format.DateTimeFormatter;

OffsetDateTime start = OffsetDateTime.parse("2026-03-02T14:30:00-03:00");

String iso = start.format(DateTimeFormatter.ISO_OFFSET_DATE_TIME);
// 2026-03-02T14:30:00-03:00
```

---

## Practical guidance for scheduling APIs

1. **Persist UTC Instants** in DB (`TIMESTAMP WITH TIME ZONE` in Postgres maps nicely, but even if your column is naive, store UTC consistently).
2. Keep a **ZoneId** for the business (or per-location/professional): `America/Sao_Paulo`, not `GMT-3`.
3. Generate available slots using `ZonedDateTime` in that zone, then convert to `Instant` for conflict checks.
4. Prefer returning `OffsetDateTime` in APIs if clients care about offset; otherwise return `Instant` and let clients render.

---

## Links

- `java.time` package summary: https://docs.oracle.com/javase/8/docs/api/java/time/package-summary.html
- `LocalDate`: https://docs.oracle.com/javase/8/docs/api/java/time/LocalDate.html
- `Instant`: https://docs.oracle.com/javase/8/docs/api/java/time/Instant.html
- `OffsetDateTime`: https://docs.oracle.com/javase/8/docs/api/java/time/OffsetDateTime.html
- `ZonedDateTime`: https://docs.oracle.com/javase/8/docs/api/java/time/ZonedDateTime.html
- `ZoneId`: https://docs.oracle.com/javase/8/docs/api/java/time/ZoneId.html
