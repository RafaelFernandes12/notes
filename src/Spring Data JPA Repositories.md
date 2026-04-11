# Spring Data JPA Repositories

______________________________________________________________________

**Date:** 2026-03-15
**Tags:** [Java](../tags/Java.md), [Spring_Boot](../tags/Spring_Boot.md), [JPA](../tags/JPA.md), [Hibernate](../tags/Hibernate.md)
**URL:** https://docs.spring.io/spring-data/jpa/reference/jpa/repositories.html

______________________________________________________________________

## Concept (what is it?)

In Spring Data JPA, a **Repository** is an interface that represents the **data access layer** for an entity.

Instead of writing common CRUD SQL/JPA code yourself, you declare an interface like:

```java
public interface UserRepository extends JpaRepository<User, Integer> {
  User findByEmail(String email);
}
```

and Spring will generate an implementation at runtime.

What repositories are for:

- **CRUD**: `save`, `findById`, `findAll`, `deleteById`, ...
- **Queries**: derived queries by method name (`findByEmail`), custom JPQL/native queries
- **Pagination/sorting**: `Pageable`, `Sort`
- **Exists/count**: `existsBy...`, `countBy...`

They usually sit between **service/business logic** and the database.

______________________________________________________________________

## How to create a repository (syntax)

### Minimal repository

```java
import org.springframework.data.jpa.repository.JpaRepository;

public interface AppointmentRepository extends JpaRepository<Appointment, Integer> {
}
```

Where:

- `Appointment` is your `@Entity`
- `Integer` is the ID type (`@Id` field type)

You typically **don’t** need to annotate the interface with `@Repository` if component scanning is set up (Spring Data registers it).

### Which base interface?

- `CrudRepository<T, ID>`: basic CRUD
- `PagingAndSortingRepository<T, ID>`: adds pagination/sorting
- `JpaRepository<T, ID>`: most common; extends the above and adds JPA-specific convenience methods (flush, batch deletes, etc.)

In practice: use `JpaRepository` most of the time.

______________________________________________________________________

## How to use it (examples)

### Example 1 — derived query method (based on your codebase)

Your current repository:

```java
public interface UserRepository extends JpaRepository<User, Integer> {
  User findByEmail(String email);
}
```

Spring parses the method name and builds a query.

Common variations:

```java
Optional<User> findByEmail(String email);
boolean existsByEmail(String email);
List<User> findAllByNameContainingIgnoreCase(String namePart);
```

Tip: prefer returning `Optional<User>` when “not found” is normal.

______________________________________________________________________

### Example 2 — relationship-based query methods (appointments)

If you have `Appointment` with `@ManyToOne Barber barber` and `@ManyToOne Customer customer`, you can query by FK using property navigation:

```java
public interface AppointmentRepository extends JpaRepository<Appointment, Integer> {
  List<Appointment> findByBarberId(Integer barberId);
  List<Appointment> findByCustomerId(Integer customerId);
  List<Appointment> findByBarberIdAndAppointmentTimeBetween(
      Integer barberId,
      LocalDateTime start,
      LocalDateTime end
  );
}
```

This is usually cleaner than manually writing joins.

______________________________________________________________________

### Example 3 — pagination and sorting

```java
public interface AppointmentRepository extends JpaRepository<Appointment, Integer> {
  Page<Appointment> findByCustomerId(Integer customerId, Pageable pageable);
}
```

Usage:

```java
Pageable page = PageRequest.of(0, 20, Sort.by("appointmentTime").descending());
Page<Appointment> result = repo.findByCustomerId(customerId, page);
```

______________________________________________________________________

### Example 4 — custom query when method names get too complex

```java
public interface AppointmentRepository extends JpaRepository<Appointment, Integer> {
  @Query("""
    select a
    from Appointment a
    where a.barber.id = :barberId
      and a.appointmentTime >= :from
      and a.appointmentTime < :to
    order by a.appointmentTime
  """)
  List<Appointment> findSchedule(
      @Param("barberId") Integer barberId,
      @Param("from") LocalDateTime from,
      @Param("to") LocalDateTime to
  );
}
```

Use `@Query` when:

- you need explicit joins or projections
- the derived method name is unreadable
- you need performance tweaks

______________________________________________________________________

## Pros and cons

### Pros

- Very fast to build CRUD and common queries
- Less boilerplate than EntityManager/DAO
- Strong integration with Spring (transactions, pagination, auditing)
- Derived queries are great for simple reads

### Cons / trade-offs

- Method-name queries can become unreadable for complex conditions
- Easy to accidentally create N+1 issues if you fetch relationships without planning
- Repository should not become “business logic”; keep that in services

______________________________________________________________________

## Practical guidelines

- Keep repositories focused on **persistence operations**, not domain rules.
- For writes, prefer service methods like `scheduleAppointment(...)` that:
  - validate input
  - check invariants (barber exists, time slot free)
  - then call repositories.
- Prefer `Optional<T>` for finders that might not return anything.
- Use pagination for endpoints that can grow.

______________________________________________________________________

## Related docs

- Spring Data JPA — Repositories: https://docs.spring.io/spring-data/jpa/reference/jpa/repositories.html
- Query methods (derived queries): https://docs.spring.io/spring-data/jpa/reference/repositories/query-methods-details.html
- `JpaRepository` API: https://docs.spring.io/spring-data/commons/docs/current/api/org/springframework/data/jpa/repository/JpaRepository.html
