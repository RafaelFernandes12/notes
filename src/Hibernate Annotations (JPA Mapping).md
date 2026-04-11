# Hibernate Annotations (JPA Mapping)

______________________________________________________________________

**Date:** 2026-03-02
**Tags:** [Java](../tags/Java.md), [Hibernate](../tags/Hibernate.md), [JPA](../tags/JPA.md), [BancoDeDados](../tags/BancoDeDados.md), [Spring_Boot](../tags/Spring_Boot.md), [Annotations](../tags/Annotations.md)
**URL:** https://docs.jboss.org/hibernate/orm/current/userguide/html_single/Hibernate_User_Guide.html#annotations

______________________________________________________________________

## Concept (what is it?)

In Java persistence, you usually annotate your classes to describe **how objects map to database tables**.

- **JPA annotations** live in `jakarta.persistence.*` (portable across ORM providers).
- **Hibernate annotations** live in `org.hibernate.annotations.*` (Hibernate-specific features).

In Spring Boot apps, you typically use **JPA annotations first**, and only add Hibernate-specific ones when you need extra behavior.

______________________________________________________________________

## Key terms (Entity/Table/Embeddable/Embedded/Component)

### `@Entity` (what is an entity?)

An **entity** is a class with an identity (`@Id`) whose instances are stored as **rows** and tracked by the persistence context.

- Has a primary key (`@Id` / `@EmbeddedId`).
- Lifecycle is managed (persist/merge/remove).
- Typically mapped to exactly one table (or a set of tables with inheritance).

```java
import jakarta.persistence.*;

@Entity
class Customer {
  @Id @GeneratedValue
  Long id;
}
```

### `@Table` (what does it do?)

`@Table` configures **table-level** mapping details for an entity: table name, schema, unique constraints, and indexes.

- Optional. If omitted, the table name is derived from the entity name + naming strategy.
- Use it when you need explicit DB naming/constraints/indexes.

```java
import jakarta.persistence.*;

@Entity
@Table(
  name = "customers",
  uniqueConstraints = {
    @UniqueConstraint(name = "uk_customers_email", columnNames = "email")
  },
  indexes = {
    @Index(name = "idx_customers_created_at", columnList = "created_at")
  }
)
class Customer {
  @Id @GeneratedValue Long id;

  @Column(name = "email", nullable = false)
  String email;

  @Column(name = "created_at", nullable = false)
  java.time.Instant createdAt;
}
```

### `@Embeddable` / `@Embedded` (what are they?)

These model a **value object** (no identity) whose fields are stored in the *same table* as the owning entity.

- `@Embeddable`: marks the value-object type.
- `@Embedded`: uses that type inside an entity.
- The embedded fields become columns of the owning entity’s table.
- You can reuse the same embeddable in many entities.

### “Component” (Hibernate term)

Historically, Hibernate called embedded/value-type mappings **components**. In modern Hibernate/JPA usage:

- Prefer JPA `@Embeddable` / `@Embedded`.
- You’ll still see “component” in docs, blog posts, and older code.

## How to use (examples)

### Example 1 — the “core” mapping annotations (`@Entity`, `@Id`, columns, timestamps)

```java
import jakarta.persistence.*;
import org.hibernate.annotations.CreationTimestamp;
import org.hibernate.annotations.UpdateTimestamp;

import java.time.Instant;

@Entity
@Table(
  name = "customers",
  indexes = {
    @Index(name = "idx_customers_email", columnList = "email", unique = true)
  }
)
public class Customer {
  @Id
  @GeneratedValue(strategy = GenerationType.IDENTITY)
  private Long id;

  @Column(nullable = false, length = 80)
  private String name;

  @Column(nullable = false, unique = true, length = 120)
  private String email;

  @CreationTimestamp
  @Column(nullable = false, updatable = false)
  private Instant createdAt;

  @UpdateTimestamp
  @Column(nullable = false)
  private Instant updatedAt;

  protected Customer() {}
  public Customer(String name, String email) {
    this.name = name;
    this.email = email;
  }
}
```

Notes:

- `@Entity`, `@Table`, `@Id`, `@Column` are **JPA**.
- `@CreationTimestamp` / `@UpdateTimestamp` are **Hibernate** conveniences.
- Prefer `java.time.Instant` (or other `java.time` types) instead of `java.util.Date`.

______________________________________________________________________

### Example 2 — relationships and join tables (`@ManyToOne`, `@OneToMany`, `@JoinColumn`)

Typical “many appointments belong to one customer” mapping:

```java
import jakarta.persistence.*;
import java.time.Instant;
import java.util.ArrayList;
import java.util.List;

@Entity
@Table(name = "appointments")
class Appointment {
  @Id
  @GeneratedValue(strategy = GenerationType.IDENTITY)
  Long id;

  @ManyToOne(fetch = FetchType.LAZY, optional = false)
  @JoinColumn(name = "customer_id", nullable = false)
  Customer customer;

  @Column(name = "start_at", nullable = false)
  Instant startAt;
}

@Entity
@Table(name = "customers")
class Customer {
  @Id
  @GeneratedValue(strategy = GenerationType.IDENTITY)
  Long id;

  @OneToMany(mappedBy = "customer")
  List<Appointment> appointments = new ArrayList<>();
}
```

Notes:

- `FetchType.LAZY` avoids loading the whole graph by default (but can cause N+1 if you’re not careful).
- `mappedBy` marks the relationship as bidirectional and avoids an extra join table.

Many-to-many with join table:

```java
import jakarta.persistence.*;
import java.util.HashSet;
import java.util.Set;

@Entity
class Professional {
  @Id @GeneratedValue
  Long id;

  @ManyToMany
  @JoinTable(
    name = "professional_services",
    joinColumns = @JoinColumn(name = "professional_id"),
    inverseJoinColumns = @JoinColumn(name = "service_id")
  )
  Set<Service> services = new HashSet<>();
}

@Entity
class Service {
  @Id @GeneratedValue
  Long id;
}
```

______________________________________________________________________

### Example 3 — enums and value objects (`@Enumerated`, `@Embeddable`, `@Embedded`)

Enum mapping (prefer `STRING`):

```java
import jakarta.persistence.*;

enum AppointmentStatus { PENDING, CONFIRMED, CANCELLED }

@Entity
class Appointment {
  @Id @GeneratedValue
  Long id;

  @Enumerated(EnumType.STRING)
  @Column(nullable = false, length = 20)
  AppointmentStatus status;
}
```

Value object with `@Embeddable`:

```java
import jakarta.persistence.*;

@Embeddable
public class Money {
  @Column(name = "amount_cents", nullable = false)
  long cents;

  @Column(name = "currency", nullable = false, length = 3)
  String currency;
}

@Entity
class Payment {
  @Id @GeneratedValue
  Long id;

  @Embedded
  Money total;
}
```

Same embeddable used twice (use `@AttributeOverride(s)` to avoid column-name clashes):

```java
import jakarta.persistence.*;

@Embeddable
class Address {
  @Column(nullable = false, length = 80) String street;
  @Column(nullable = false, length = 60) String city;
  @Column(nullable = false, length = 2)  String state;
  @Column(nullable = false, length = 10) String zip;
}

@Entity
@Table(name = "orders")
class Order {
  @Id @GeneratedValue Long id;

  @Embedded
  @AttributeOverrides({
    @AttributeOverride(name = "street", column = @Column(name = "shipping_street")),
    @AttributeOverride(name = "city",   column = @Column(name = "shipping_city")),
    @AttributeOverride(name = "state",  column = @Column(name = "shipping_state")),
    @AttributeOverride(name = "zip",    column = @Column(name = "shipping_zip"))
  })
  Address shipping;

  @Embedded
  @AttributeOverrides({
    @AttributeOverride(name = "street", column = @Column(name = "billing_street")),
    @AttributeOverride(name = "city",   column = @Column(name = "billing_city")),
    @AttributeOverride(name = "state",  column = @Column(name = "billing_state")),
    @AttributeOverride(name = "zip",    column = @Column(name = "billing_zip"))
  })
  Address billing;
}
```

______________________________________________________________________

### Example 4 — composite IDs with `@Embeddable` + `@EmbeddedId`

Use `@EmbeddedId` when the *entity identity itself* is a value object (composite primary key).

```java
import jakarta.persistence.*;
import java.io.Serializable;

@Embeddable
class EnrollmentId implements Serializable {
  @Column(name = "student_id") Long studentId;
  @Column(name = "course_id")  Long courseId;

  // In real code: implement equals/hashCode for IDs.
}

@Entity
@Table(name = "enrollments")
class Enrollment {
  @EmbeddedId
  EnrollmentId id;

  @Column(nullable = false)
  java.time.Instant enrolledAt;
}
```

Rule of thumb:

- `@Embedded` = part of the entity state (value object columns in the same table).
- `@EmbeddedId` = the entity identifier is an embeddable (composite PK).

______________________________________________________________________

### Example 5 — optimistic locking (`@Version`)

Use `@Version` to prevent lost updates when multiple requests update the same row.

```java
import jakarta.persistence.*;

@Entity
class Professional {
  @Id @GeneratedValue
  Long id;

  @Version
  long version;

  @Column(nullable = false)
  boolean active;
}
```

If two transactions edit the same entity concurrently, the second commit will fail with an optimistic locking exception.

______________________________________________________________________

### Example 6 — soft delete (Hibernate-specific)

Instead of deleting rows, you can mark them as deleted.

```java
import jakarta.persistence.*;
import org.hibernate.annotations.SQLDelete;
import org.hibernate.annotations.Where;

@Entity
@Table(name = "customers")
@SQLDelete(sql = "UPDATE customers SET deleted = true WHERE id = ?")
@Where(clause = "deleted = false")
class Customer {
  @Id @GeneratedValue
  Long id;

  @Column(nullable = false)
  boolean deleted = false;
}
```

Notes:

- `@SQLDelete` and `@Where` are Hibernate-only.
- Be explicit about whether you want soft-deleted rows to be queryable (admin screens, audits, etc.).

______________________________________________________________________

## Quick cheatsheet (most-used)

JPA (`jakarta.persistence.*`):

- `@Entity`, `@Table`, `@Id`, `@GeneratedValue`, `@Column`, `@Index`
- `@ManyToOne`, `@OneToMany`, `@OneToOne`, `@ManyToMany`, `@JoinColumn`, `@JoinTable`
- `@Enumerated`
- `@Embeddable`, `@Embedded`, `@AttributeOverride(s)`, `@EmbeddedId`
- `@Version`
- `@Transient`

Hibernate (`org.hibernate.annotations.*`):

- `@CreationTimestamp`, `@UpdateTimestamp`
- `@SQLDelete`, `@Where`
- `@BatchSize`, `@Fetch` (performance tuning)

______________________________________________________________________

## Links

- Hibernate ORM user guide — annotations: https://docs.jboss.org/hibernate/orm/current/userguide/html_single/Hibernate_User_Guide.html#annotations
- JPA (Jakarta Persistence) overview: https://jakarta.ee/specifications/persistence/
- `@Entity` Javadoc: https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/entity
- `@Table` Javadoc: https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/table
- `@Embeddable` Javadoc: https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/embeddable
- `@Embedded` Javadoc: https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/embedded
- `@EmbeddedId` Javadoc: https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/embeddedid
