# JPA One-to-Many and Many-to-One (Associations)

______________________________________________________________________

**Date:** 2026-03-15
**Tags:** [Java](../tags/Java.md), [Spring_Boot](../tags/Spring_Boot.md), [JPA](../tags/JPA.md), [Hibernate](../tags/Hibernate.md), [BancoDeDados](../tags/BancoDeDados.md), [Annotations](../tags/Annotations.md)
**URL:** https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/manytoone , https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/onetomany , https://docs.jboss.org/hibernate/orm/current/userguide/html_single/Hibernate_User_Guide.html#associations , https://docs.spring.io/spring-data/jpa/reference/

______________________________________________________________________

## Concept (what is it?)

In a relational database, a **one-to-many** relationship is usually implemented with a **foreign key on the “many” table**.

Example:

- 1 `teacher` → N `course`
- The FK lives in `course` (e.g. `course.teacher_id` references `teacher.id`)

In JPA, that same relationship is typically mapped with:

- `@ManyToOne` on the **many side** (the entity that contains the FK column)
- `@OneToMany(mappedBy = "...")` on the **one side** (a collection “view” of the rows that point back)

The **most important mental model**:

> The FK is on the *many* table, so the **owning side** in JPA is almost always the `@ManyToOne` side.

That affects how you update the association in code: **the owning side is what drives the SQL updates**.

______________________________________________________________________

## Key ideas you must know

### 1) Owning side vs inverse side (`mappedBy`)

In a bidirectional mapping, only one side is the **owner** (the one that writes the FK).

- `@ManyToOne` is the owning side (it has the FK column via `@JoinColumn`)
- `@OneToMany(mappedBy = "fieldNameOnManySide")` is the inverse side

If you forget `mappedBy` on `@OneToMany`, JPA often maps it as a **join table** (extra table) because it doesn’t know where the FK is.

### 2) Unidirectional vs bidirectional

You can model relationships in different directions:

- **Unidirectional `@ManyToOne`** (common, simple): child → parent only.
- **Bidirectional `@ManyToOne` + `@OneToMany(mappedBy=...)`**: child → parent and parent → children.

Rule of thumb:

- If you only need “appointment → customer”, keep it unidirectional.
- If you also frequently need “customer → appointments”, add the `@OneToMany` collection.

### 3) Fetch type defaults (and why people get surprised)

JPA defaults:

- `@ManyToOne` defaults to **EAGER** (loads parent automatically)
- `@OneToMany` defaults to **LAZY** (loads collection on access)

In APIs, it’s common to explicitly use:

```java
@ManyToOne(fetch = FetchType.LAZY)
```

…to avoid loading big graphs by accident.

### 4) `cascade` and `orphanRemoval`

These are about **entity lifecycle propagation** (not “SQL joins”).

- `cascade = ...` means: when I persist/merge/remove the parent, do the same to the child.
- `orphanRemoval = true` means: if a child is removed from the parent collection, delete that child row.

Common pattern for *aggregate* relationships (parent “owns” children):

```java
@OneToMany(mappedBy = "order", cascade = CascadeType.ALL, orphanRemoval = true)
private List<OrderItem> items = new ArrayList<>();
```

Common warning:

- Cascading from **child → parent** (`@ManyToOne(cascade = ALL)`) is often a mistake (it can delete/update the parent unexpectedly).

______________________________________________________________________

## How to use (examples)

### Example 1 — Teacher ↔ Course (bidirectional) (example found in repo code)

This is the classic mapping: many courses belong to one teacher.

Codebase reference:

- `terceira_idade_API/src/main/java/com/terceiraIdade/terceira_idade_API/models/Teacher.java`
- `terceira_idade_API/src/main/java/com/terceiraIdade/terceira_idade_API/models/Course.java`

Database shape:

```sql
-- teacher (1)
create table teacher (
  id bigint primary key,
  name text not null
);

-- course (N) with FK to teacher
create table course (
  id bigint primary key,
  name text not null,
  teacher_id bigint not null,
  constraint fk_course_teacher foreign key (teacher_id) references teacher(id)
);
```

JPA mapping:

```java
@Entity
@Table(name = "teacher")
public class Teacher {
  @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
  private Long id;

  @Column(nullable = false)
  private String name;

  // inverse side (does NOT own the FK)
  @OneToMany(mappedBy = "teacher")
  private Set<Course> courses = new HashSet<>();

  // convenience methods (keep both sides in sync)
  public void addCourse(Course c) {
    courses.add(c);
    c.setTeacher(this);
  }

  public void removeCourse(Course c) {
    courses.remove(c);
    c.setTeacher(null);
  }
}

@Entity
@Table(name = "course")
public class Course {
  @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
  private Long id;

  @Column(nullable = false)
  private String name;

  // owning side (this side writes course.teacher_id)
  @ManyToOne(fetch = FetchType.LAZY, optional = false)
  @JoinColumn(name = "teacher_id", nullable = false)
  private Teacher teacher;

  public void setTeacher(Teacher teacher) {
    this.teacher = teacher;
  }
}
```

Notes:

- The **FK column** is on `Course`, so `Course.teacher` is the **owning side**.
- `Teacher.courses` is a “reverse navigation” view.
- In a bidirectional association, you usually want **helper methods** (`addCourse/removeCourse`) to keep both sides consistent.

About cascading here:

- If `Teacher` is the aggregate root and `Course` cannot exist without it, consider:

  ```java
  @OneToMany(mappedBy = "teacher", cascade = {CascadeType.PERSIST, CascadeType.MERGE}, orphanRemoval = true)
  private Set<Course> courses = new HashSet<>();
  ```

- If `Course` has an independent lifecycle (can be reassigned, kept for history, etc.), avoid `orphanRemoval` and be careful with `REMOVE` cascades.

Spring Boot + JSON note (very common):

- Bidirectional relationships can cause **infinite recursion** when serializing to JSON.
- In the repo example, annotations like `@JsonIgnoreProperties` / `@JsonIgnore` are used to break the cycle.
- Another common approach is: **don’t expose entities directly**; return DTOs.

______________________________________________________________________

### Example 2 — Order ↔ OrderItem (aggregate) with `cascade` + `orphanRemoval`

This is the relationship where the parent truly “owns” the children.

```java
@Entity
@Table(name = "orders")
public class Order {
  @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
  private Long id;

  @OneToMany(mappedBy = "order", cascade = CascadeType.ALL, orphanRemoval = true)
  private List<OrderItem> items = new ArrayList<>();

  public void addItem(String sku, int quantity) {
    OrderItem item = new OrderItem(sku, quantity);
    item.setOrder(this);     // owning side
    items.add(item);         // inverse side
  }

  public void removeItem(OrderItem item) {
    items.remove(item);
    item.setOrder(null);
  }
}

@Entity
@Table(name = "order_item")
public class OrderItem {
  @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
  private Long id;

  @Column(nullable = false)
  private String sku;

  @Column(nullable = false)
  private int quantity;

  @ManyToOne(fetch = FetchType.LAZY, optional = false)
  @JoinColumn(name = "order_id", nullable = false)
  private Order order;

  protected OrderItem() {}
  public OrderItem(String sku, int quantity) {
    this.sku = sku;
    this.quantity = quantity;
  }

  public void setOrder(Order order) {
    this.order = order;
  }
}
```

What you get from this mapping:

- Saving the `Order` saves all `OrderItem`s (`cascade = ALL`).
- Removing an item from the collection deletes the row (`orphanRemoval = true`).
- Deleting the order deletes all items (because `REMOVE` is part of `ALL`).

Typical Spring service usage:

```java
@Service
public class OrderService {
  private final OrderRepository orderRepository;

  public OrderService(OrderRepository orderRepository) {
    this.orderRepository = orderRepository;
  }

  @Transactional
  public Long createOrder() {
    Order order = new Order();
    order.addItem("SKU-1", 2);
    order.addItem("SKU-2", 1);
    orderRepository.save(order);
    return order.getId();
  }
}
```

______________________________________________________________________

### Example 3 — Common mistake: using `@OneToMany` where it should be `@ManyToOne`

In the `barbearia_do_matheus` codebase, `Appointment` was modeled as an entity `record` and annotated like this:

Codebase reference:

- `barbearia_do_matheus/src/main/java/com/example/barbearia_do_matheus/models/Appointment.java`

```java
@Entity
public record Appointment(
  LocalDateTime appointmentTime,

  @OneToMany User user
) {}
```

Why this is a problem:

- Conceptually: an appointment typically belongs to **one** user → this should be `@ManyToOne` (many appointments → one user).
- Mapping-wise: `@OneToMany` on a single `User user` field is a cardinality mismatch.
- JPA-spec-wise: entity classes must satisfy requirements (non-final class, no-arg constructor, etc.). Java `record`s are **not a good fit** for entities; use records for DTOs/projections instead.

Corrected mapping (typical):

```java
@Entity
@Table(name = "appointments")
public class Appointment {
  @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
  private Long id;

  @Column(nullable = false)
  private LocalDateTime appointmentTime;

  @ManyToOne(fetch = FetchType.LAZY, optional = false)
  @JoinColumn(name = "user_id", nullable = false)
  private User user;
}

@Entity
@Table(name = "users")
public class User {
  @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
  private Long id;

  @OneToMany(mappedBy = "user")
  private List<Appointment> appointments = new ArrayList<>();
}
```

If you don’t need `user.getAppointments()`, you can omit the `@OneToMany` completely and keep only the unidirectional `@ManyToOne`.

______________________________________________________________________

## Spring Boot / Hibernate pitfalls (and how to avoid them)

### 1) Infinite JSON recursion

If you serialize entities directly (controller returns an entity), this is common:

- `Teacher` → `courses` → each `Course` → `teacher` → back to `courses` → …

Fix options:

- Use DTOs (recommended for APIs)
- Or use Jackson annotations (`@JsonIgnore`, `@JsonIgnoreProperties`, `@JsonManagedReference/@JsonBackReference`)

### 2) Lazy loading outside a transaction (`LazyInitializationException`)

If `spring.jpa.open-in-view=false` (often recommended), lazy relationships must be initialized inside a transaction.

Two common solutions:

**Fetch join query**:

```java
@Query("select t from Teacher t left join fetch t.courses where t.id = :id")
Optional<Teacher> findByIdWithCourses(Long id);
```

**EntityGraph**:

```java
@EntityGraph(attributePaths = "courses")
Optional<Teacher> findWithCoursesById(Long id);
```

### 3) N+1 queries

Symptom: you load N parents and then iterate their children → Hibernate runs 1 query for parents + N queries for children.

Fix options:

- Fetch join / `@EntityGraph`
- Batch fetching (`hibernate.default_batch_fetch_size`)

### 4) `Set` + `equals/hashCode`

If you use `Set<Child>` on `@OneToMany`, your entity equality matters.

Rule of thumb:

- Don’t base `equals/hashCode` on mutable fields.
- Be careful using generated IDs in `equals/hashCode` (ID is null before persist).

______________________________________________________________________

## Quick cheatsheet

- **DB FK lives on the “many” table** → model the FK with `@ManyToOne` + `@JoinColumn`.
- For a bidirectional mapping: use `@OneToMany(mappedBy = "...")` on the “one” side.
- Prefer `@ManyToOne(fetch = LAZY)` in APIs.
- Cascade/orphan removal are about **lifecycle**, not about join behavior.
- Keep both sides in sync in code (helper methods) when bidirectional.

______________________________________________________________________

## Links

- `@ManyToOne` Javadoc: https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/manytoone
- `@OneToMany` Javadoc: https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/onetomany
- `@JoinColumn` Javadoc: https://jakarta.ee/specifications/persistence/3.1/apidocs/jakarta.persistence/jakarta/persistence/joincolumn
- Hibernate user guide — associations: https://docs.jboss.org/hibernate/orm/current/userguide/html_single/Hibernate_User_Guide.html#associations
- Spring Data JPA reference: https://docs.spring.io/spring-data/jpa/reference/
