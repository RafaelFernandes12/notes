# JPA Inheritance (Entity Inheritance Strategies)

______________________________________________________________________

**Date:** 2026-03-15
**Tags:** [Java](../tags/Java.md), [JPA](../tags/JPA.md), [Hibernate](../tags/Hibernate.md), [Spring_Boot](../tags/Spring_Boot.md), [ModelagemDeDados](../tags/ModelagemDeDados.md)
**URL:** https://jakarta.ee/specifications/persistence/3.1/jakarta-persistence-spec-3.1.html

______________________________________________________________________

## Concept (what is it?)

**JPA inheritance** is how you map a Java class hierarchy (e.g., `User` → `Barber` / `Customer`) into relational tables.

When you annotate:

```java
@Entity
public class Barber extends User { }
```

you’re telling the ORM:

- `Barber` **is persisted**, and
- it **shares the identity** and base fields of `User`, and
- JPA must decide **how** to store the hierarchy in the database.

That “how” is the **inheritance strategy**.

______________________________________________________________________

## The 3 strategies (and what tables they create)

JPA provides 3 main strategies via `@Inheritance` on the base entity:

```java
@Entity
@Inheritance(strategy = InheritanceType.SINGLE_TABLE) // change this
public abstract class User { ... }
```

### 1) `SINGLE_TABLE`

**One table** holds all subclasses.

- Tables: `users`
- A **discriminator column** stores which subtype the row represents

Shape (conceptual):

```
users
  id (PK)
  user_type (discriminator)
  name
  email
  password
  ...columns that only exist for Barber or only for Customer (nullable)
```

Best when:

- subclasses share most fields
- you want simpler schema and fewer joins

Trade-off:

- subtype-specific columns become nullable for other types

### 2) `JOINED`

**Base table + one table per subclass**.

- Tables: `users`, `barbers`, `customers`
- Subclass tables use the same PK as `users.id` (also an FK to `users`)

Shape (conceptual):

```
users
  id (PK)
  name
  email
  password

barbers
  id (PK, FK -> users.id)
  ...barber-only columns

customers
  id (PK, FK -> users.id)
  ...customer-only columns
```

Best when:

- subclasses are expected to diverge significantly
- you want a more normalized schema

Trade-off:

- reads often require joins

### 3) `TABLE_PER_CLASS`

**One table per concrete class**, duplicating base columns.

- Tables: `barbers`, `customers` (each repeats `name/email/password`)

Usually avoid because:

- polymorphic queries (querying `User`) often require `UNION`
- id generation and constraints can be awkward

______________________________________________________________________

## Discriminator: what it is, and why you care

With `SINGLE_TABLE` (and sometimes `JOINED`), JPA uses a discriminator to know which Java type to instantiate when it reads a row.

Example:

```java
import jakarta.persistence.*;

@Entity
@Table(name = "users")
@Inheritance(strategy = InheritanceType.SINGLE_TABLE)
@DiscriminatorColumn(name = "user_type", discriminatorType = DiscriminatorType.STRING)
public abstract class User {
  @Id @GeneratedValue
  private Long id;

  private String name;
  private String email;
  private String password;
}

@Entity
@DiscriminatorValue("BARBER")
public class Barber extends User {
  // barber-specific fields can go here later
}

@Entity
@DiscriminatorValue("CUSTOMER")
public class Customer extends User {
  // customer-specific fields can go here later
}
```

The `users.user_type` column will contain `BARBER` or `CUSTOMER`.

If you don’t configure it, Hibernate often defaults to a discriminator column named `DTYPE`.

______________________________________________________________________

## How this affects relationships (appointments example)

In a barbershop domain, an appointment must reference **exactly one barber** and **exactly one customer**.

With inheritance, your `Appointment` can reference the *subtypes* directly:

```java
import jakarta.persistence.*;
import jakarta.validation.constraints.NotNull;
import java.time.LocalDateTime;

@Entity
public class Appointment {
  @Id @GeneratedValue
  private Long id;

  @NotNull
  private LocalDateTime appointmentTime;

  @ManyToOne(optional = false)
  @JoinColumn(name = "barber_id", nullable = false)
  private Barber barber;

  @ManyToOne(optional = false)
  @JoinColumn(name = "customer_id", nullable = false)
  private Customer customer;
}
```

This is a big practical win of inheritance: the type system prevents you from accidentally setting a customer as the barber.

If you also want navigation collections:

```java
@Entity
public class Barber extends User {
  @OneToMany(mappedBy = "barber")
  private java.util.List<Appointment> appointments;
}

@Entity
public class Customer extends User {
  @OneToMany(mappedBy = "customer")
  private java.util.List<Appointment> appointments;
}
```

Important: with `mappedBy`, **do not** add `@JoinColumn` on the `@OneToMany` side. The foreign keys live on `Appointment` (`barber_id` and `customer_id`).

______________________________________________________________________

## Pros / cons: inheritance vs “single User + role enum”

### Inheritance (Barber extends User, Customer extends User)

Pros:

- **Type safety** in relationships (`Appointment.barber` can only be a `Barber`)
- Natural place for subtype-specific fields and behavior

Cons:

- More ORM complexity (strategy choice, discriminator, joins)
- You must understand how queries behave (polymorphic queries, fetching)

### Single `User` with `role` / `type` field

Pros:

- Simplest mapping (no inheritance tables/discriminator)
- Straightforward auth model (“everyone is a user”)

Cons:

- Relationships need extra rules: `Appointment.barber` and `Appointment.customer` are both `User` so you must enforce `role == BARBER` / `role == CUSTOMER` in service layer (or DB constraints)

______________________________________________________________________

## Practical guidelines

- If barber/customer are mostly the same and you want simplicity: prefer **single User + role**.
- If you want stronger modeling + you expect divergence (barber schedule/services, customer profile/loyalty): use **inheritance**.
- If you choose inheritance and want a simple schema: start with **`SINGLE_TABLE`**.
- Use **`JOINED`** when subtype-only columns become many or you care about normalization.
- Avoid **`TABLE_PER_CLASS`** unless you have a strong reason.

______________________________________________________________________

## Related docs

- Jakarta Persistence spec (Inheritance): https://jakarta.ee/specifications/persistence/3.1/jakarta-persistence-spec-3.1.html
- Hibernate ORM User Guide (Inheritance mappings): https://docs.jboss.org/hibernate/orm/current/userguide/html_single/Hibernate_User_Guide.html#inheritance
