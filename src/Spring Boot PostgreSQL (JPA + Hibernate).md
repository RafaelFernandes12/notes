# Spring Boot PostgreSQL (JPA + Hibernate)

______________________________________________________________________

**Date:** 2026-03-02
**Tags:** [Spring_Boot](../tags/Spring_Boot.md), [Java](../tags/Java.md), [Postgres](../tags/Postgres.md), [JPA](../tags/JPA.md), [Hibernate](../tags/Hibernate.md), [Flyway](../tags/Flyway.md), [Docker](../tags/Docker.md), [Maven](../tags/Maven.md), [Dependencies](../tags/Dependencies.md), [BancoDeDados](../tags/BancoDeDados.md), [Config](../tags/Config.md)
**URL:** https://docs.spring.io/spring-boot/docs/current/reference/html/data.html#data.sql

______________________________________________________________________

## Concept (what is it?)

To use **PostgreSQL** with a **Spring Boot** app (using **JPA/Hibernate**) you typically need:

1. A running Postgres instance
2. JDBC + JPA dependencies
3. Spring Boot datasource + JPA configuration

Optional but recommended:

- **Flyway** (or Liquibase) for database migrations

---

## How to use (examples)

### Example 1 — Run PostgreSQL locally with Docker (fastest)

Run a container:

```bash
docker run --name barbearia-pg \
  -e POSTGRES_DB=barbearia \
  -e POSTGRES_USER=barbearia \
  -e POSTGRES_PASSWORD=barbearia \
  -p 5432:5432 \
  -d postgres:16
```

Or using `docker-compose.yml`:

```yaml
services:
  postgres:
    image: postgres:16
    container_name: barbearia-pg
    environment:
      POSTGRES_DB: barbearia
      POSTGRES_USER: barbearia
      POSTGRES_PASSWORD: barbearia
    ports:
      - "5432:5432"
    volumes:
      - pgdata:/var/lib/postgresql/data

volumes:
  pgdata:
```

---

### Example 2 — Add Maven dependencies (JPA + Postgres + Flyway)

In `pom.xml`:

```xml
<dependencies>
  <!-- REST API -->
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
  </dependency>

  <!-- JPA (Hibernate) -->
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-jpa</artifactId>
  </dependency>

  <!-- PostgreSQL JDBC driver -->
  <dependency>
    <groupId>org.postgresql</groupId>
    <artifactId>postgresql</artifactId>
  </dependency>

  <!-- Database migrations (recommended) -->
  <dependency>
    <groupId>org.flywaydb</groupId>
    <artifactId>flyway-core</artifactId>
  </dependency>
</dependencies>
```

---

### Example 3 — Configure `application.properties`

Minimal datasource + JPA settings:

```properties
spring.datasource.url=jdbc:postgresql://localhost:5432/barbearia
spring.datasource.username=barbearia
spring.datasource.password=barbearia

# DEV only: auto-create/update tables
spring.jpa.hibernate.ddl-auto=update

# Optional: see SQL in logs
spring.jpa.show-sql=true
```

For real projects, prefer migrations and validate schema:

```properties
spring.jpa.hibernate.ddl-auto=validate
```

---

### Example 4 — A tiny JPA/Hibernate mapping (Entity + Repository)

Entity:

```java
import jakarta.persistence.*;

@Entity
@Table(name = "customers")
public class Customer {
  @Id
  @GeneratedValue(strategy = GenerationType.IDENTITY)
  private Long id;

  @Column(nullable = false, length = 80)
  private String name;

  protected Customer() {}
  public Customer(String name) { this.name = name; }
}
```

Repository:

```java
import org.springframework.data.jpa.repository.JpaRepository;

public interface CustomerRepository extends JpaRepository<Customer, Long> {}
```

At runtime, Spring Data uses Hibernate + the Postgres driver to generate SQL.

---

### Example 5 — Flyway migration for the same table

Create a file like:

`src/main/resources/db/migration/V1__create_customers.sql`

```sql
create table customers (
  id bigserial primary key,
  name varchar(80) not null
);
```

Then set:

```properties
spring.jpa.hibernate.ddl-auto=validate
```

Flyway will apply migrations on startup; Hibernate will validate mappings vs schema.

---

## Common pitfalls

- Using `ddl-auto=update` in production (can lead to surprises). Prefer Flyway/Liquibase.
- Using `EnumType.ORDINAL` in JPA (use `EnumType.STRING` unless you *really* know why).
- Assuming `localhost` works inside Docker for the app container (if app is also in Docker, use the service name).

---

## Links

- `java.sql`/JDBC is used under the hood via driver: https://jdbc.postgresql.org/
- Spring Boot — SQL/DB docs: https://docs.spring.io/spring-boot/docs/current/reference/html/data.html
- Spring Data JPA: https://spring.io/projects/spring-data-jpa
- Hibernate ORM: https://hibernate.org/orm/
- Flyway: https://documentation.red-gate.com/flyway
