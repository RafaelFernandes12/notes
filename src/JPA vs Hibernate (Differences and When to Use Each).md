# JPA vs Hibernate (Differences and When to Use Each)

______________________________________________________________________

**Date:** 2026-03-31
**Tags:** [Java](../tags/Java.md), [Spring_Boot](../tags/Spring_Boot.md), [JPA](../tags/JPA.md), [Hibernate](../tags/Hibernate.md), [BancoDeDados](../tags/BancoDeDados.md), [SQL](../tags/SQL.md)
**URL:** https://jakarta.ee/specifications/persistence/ | https://hibernate.org/orm/documentation/

______________________________________________________________________

## Concept (what is it?)

`JPA` and `Hibernate` are related, but they are **not the same thing**.

- `JPA` (`Jakarta Persistence API`) is the **standard/specification** for persistence in Java.
- `Hibernate` is a **real ORM framework** that implements JPA and adds extra features of its own.

The most useful mental model is:

- **JPA = the contract**
- **Hibernate = one implementation of that contract**

So in a normal Spring Boot project, you usually do **not** choose `JPA` *instead of* `Hibernate`.
What usually happens is:

1. you write your mappings and most persistence code using **JPA annotations and APIs**
2. **Hibernate** runs underneath as the JPA provider

That is why people often say “I am using JPA”, even though Hibernate is the engine doing the real work at runtime.

Important nuance:

- `JPA` by itself is **not enough to run your app**
- you still need an implementation such as **Hibernate** or **EclipseLink**

______________________________________________________________________

## Difference in one sentence

- `JPA` defines the common persistence model
- `Hibernate` makes that model work in a real application and also offers provider-specific features beyond the spec

______________________________________________________________________

## JPA vs Hibernate

| Aspect | JPA | Hibernate |
|---|---|---|
| What it is | Specification / API | Framework / ORM implementation |
| Typical package | `jakarta.persistence.*` | `org.hibernate.*` |
| Main goal | Standardize persistence concepts | Execute persistence and provide extra ORM features |
| Portability | Higher | Lower when you depend on Hibernate-specific features |
| Examples | `@Entity`, `@Id`, `@Column`, `@ManyToOne`, `EntityManager` | `@CreationTimestamp`, `@BatchSize`, `@Formula`, `Session` |
| Can it run alone? | No, it needs a provider | Yes, it is a provider |
| Default recommendation | Write application code against it | Use underneath unless you need extra features directly |

______________________________________________________________________

## Where each fits in the stack

If you are using Spring Boot with a relational database, the stack often looks like this:

- `Flyway` = version and migrate the database schema
- `JPA` = define the persistence contract and mapping model
- `Hibernate` = implement that contract and execute the ORM behavior
- `Spring Data JPA` = add repository abstractions like `JpaRepository`
- `PostgreSQL` / `MySQL` / another SQL database = store the actual data

This helps avoid a common confusion: `JPA` and `Hibernate` are about **mapping and persistence**, while `Flyway` is about **schema migrations**.

______________________________________________________________________

## When should I use JPA?

Use `JPA` as your **default programming model**.

That usually means:

- use `jakarta.persistence` annotations such as `@Entity`, `@Table`, `@Id`, `@Column`, `@OneToMany`, `@ManyToOne`
- think in terms of entities, relationships, and persistence context
- prefer standard APIs and standard mappings first
- keep your code portable and easier to reason about

`JPA` is the best default choice when:

- you are building a normal CRUD or business application
- your database is relational
- standard entity mapping is enough
- you want cleaner, more portable code
- you do not want to couple your whole codebase to one provider

In practice, this is how **most** Spring Boot applications should be written.

______________________________________________________________________

## When should I use Hibernate directly?

Use `Hibernate` directly when `JPA` is **not enough** for what you need.

Typical reasons:

- you need provider-specific convenience annotations
- you need advanced fetch tuning or batching
- you need a custom type or database-specific mapping
- you need features the JPA spec does not expose well
- you are solving a performance problem and Hibernate offers a targeted tool for it

Examples of Hibernate-specific things you might reach for:

- `@CreationTimestamp`
- `@UpdateTimestamp`
- `@BatchSize`
- `@Formula`
- `Session`
- second-level cache features

This is usually a **targeted decision**, not your default style.

Good rule: prefer `JPA` first, then opt into `Hibernate` features only when you can clearly explain why the spec is not enough.

______________________________________________________________________

## Rule of thumb

In most real projects, the right answer is:

1. **write against JPA**
2. **run with Hibernate**
3. **use Hibernate-specific features only in isolated places**
4. **use Flyway for schema changes, not Hibernate `ddl-auto`**

So the choice is usually not `JPA or Hibernate`.
It is usually:

- `JPA` for the code you write
- `Hibernate` for the provider running underneath

______________________________________________________________________

## How to recognize which one you are using

Quick heuristic:

- if the import is `jakarta.persistence.*`, you are using **JPA**
- if the import is `org.hibernate.*`, you are using a **Hibernate-specific** feature

That simple distinction is very useful when reading code.

______________________________________________________________________

## Examples

### Example 1 — JPA entity mapping from this project

From this codebase, `Tenant` is mapped almost entirely with **JPA annotations**:

```java
@Entity
@Table(name = "tenants")
public class Tenant {

  @Id
  @GeneratedValue(strategy = GenerationType.IDENTITY)
  private Long id;

  @Column(nullable = false, length = 150)
  private String name;

  @Column(nullable = false, unique = true, length = 255)
  private String email;

  @ElementCollection(fetch = FetchType.EAGER)
  @CollectionTable(name = "tenant_roles", joinColumns = @JoinColumn(name = "tenant_id"))
  @Column(name = "role", nullable = false, length = 64)
  @Enumerated(EnumType.STRING)
  private Set<UserRole> roles = new LinkedHashSet<>();
}
```

Why this is a good `JPA` example:

- `@Entity`, `@Table`, `@Id`, `@Column`, `@ElementCollection`, `@CollectionTable`, `@JoinColumn`, `@Enumerated` come from `jakarta.persistence`
- the code is written against the **standard persistence API**
- Hibernate reads these mappings and turns them into SQL behavior at runtime

This is the normal and recommended style for most entity classes.

______________________________________________________________________

### Example 2 — JPA relationship mapping from this project

`RefreshToken` also uses standard JPA mapping annotations:

```java
@Entity
@Table(name = "refresh_tokens")
public class RefreshToken {

  @Id
  @GeneratedValue(strategy = GenerationType.IDENTITY)
  private Long id;

  @ManyToOne(optional = false, fetch = FetchType.LAZY)
  @JoinColumn(name = "tenant_id", nullable = false)
  private Tenant tenant;

  @Column(nullable = false, unique = true, length = 120)
  private String token;
}
```

What is happening here:

- `@ManyToOne` and `@JoinColumn` describe a relationship in **JPA terms**
- in Java, you navigate with `refreshToken.getTenant()`
- in the database, Hibernate maps that object reference to the `tenant_id` foreign key and issues the SQL needed to load it

Again, this is a case where **you write JPA**, while **Hibernate executes the mapping behavior**.

______________________________________________________________________

### Example 3 — Spring Data JPA on top, Hibernate underneath

This project also has a repository layer:

```java
public interface TenantRepository extends JpaRepository<Tenant, Long> {

  @EntityGraph(attributePaths = {"roles"})
  Optional<Tenant> findByEmailIgnoreCase(String email);
}
```

And the application config includes:

```yaml
spring:
  jpa:
    open-in-view: false
    hibernate:
      ddl-auto: none
  flyway:
    baseline-on-migrate: true
    locations: classpath:db/migration
```

How to read this stack:

- `JpaRepository` is from **Spring Data JPA**
- the entity mappings are **JPA**
- `spring.jpa.hibernate` tells you **Hibernate** is the provider being configured
- `Flyway` manages schema evolution separately

This is a very common real-world setup:

- **JPA** for mappings
- **Hibernate** as the provider
- **Spring Data JPA** for repositories
- **Flyway** for migrations

______________________________________________________________________

### Example 4 — A direct “JPA vs Hibernate feature” comparison

This project uses a portable JPA callback approach for timestamps:

```java
@Column(name = "created_at", nullable = false)
private Instant createdAt;

@PrePersist
void prePersist() {
  if (createdAt == null) {
    createdAt = Instant.now();
  }
}
```

That is a **JPA-style** solution.

If you wanted a Hibernate-specific convenience feature instead, you could write:

```java
import org.hibernate.annotations.CreationTimestamp;

@Column(name = "created_at", nullable = false, updatable = false)
@CreationTimestamp
private Instant createdAt;
```

Comparison:

- `@PrePersist` is standard JPA and more portable
- `@CreationTimestamp` is shorter, but it couples the entity to Hibernate

This is one of the clearest examples of **when to use each**:

- if you want standard, provider-agnostic code, prefer the **JPA** version
- if you want Hibernate-specific convenience and accept the coupling, use the **Hibernate** version

______________________________________________________________________

## Decision guide

| Situation | Better default choice |
|---|---|
| You want standard entity mappings | JPA |
| You are building a normal Spring Boot CRUD app | JPA + Hibernate underneath |
| You want portability between providers | JPA |
| You need a provider-specific feature or tuning | Hibernate |
| You want repository abstractions | Spring Data JPA |
| You need schema versioning/migrations | Flyway |

______________________________________________________________________

## Common confusions

### “Can I use JPA without Hibernate?”

Not by itself.
You can use `JPA` with **another provider** such as `EclipseLink`, but the spec still needs a real implementation.

### “Can I use Hibernate without JPA?”

Yes.
Hibernate has its own APIs and features.
But in most Spring Boot applications, that should not be your default choice for all persistence code.

### “Is Spring Data JPA the same thing as JPA?”

No.

- `JPA` = persistence spec
- `Hibernate` = implementation/provider
- `Spring Data JPA` = repository abstraction layer built on top of JPA

### “Is JPA responsible for database migrations?”

No.
That is where tools like `Flyway` or `Liquibase` fit.

______________________________________________________________________

## Bottom line

If you are wondering what to use in day-to-day code, the answer is usually:

- write your entities and persistence logic in **JPA style**
- let **Hibernate** be the implementation underneath
- use **Hibernate-specific features only when the standard API is not enough**

In this codebase, that is exactly what is happening most of the time:

- the mappings are mostly `jakarta.persistence.*`
- Hibernate is the provider under Spring Boot
- Flyway handles schema changes

That is a very good default architecture for a Java + SQL application.

______________________________________________________________________

## Links

- Jakarta Persistence (JPA): https://jakarta.ee/specifications/persistence/
- Hibernate ORM documentation: https://hibernate.org/orm/documentation/
- Spring Data JPA: https://spring.io/projects/spring-data-jpa
- Spring Boot SQL / data access docs: https://docs.spring.io/spring-boot/reference/data/sql.html
