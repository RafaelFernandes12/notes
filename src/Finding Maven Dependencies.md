# Finding Maven Dependencies

______________________________________________________________________

**Date:** 2026-03-02
**Tags:** [Maven](../tags/Maven.md), [Java](../tags/Java.md), [Dependencies](../tags/Dependencies.md), [Spring_Boot](../tags/Spring_Boot.md)
**URL:** https://mvnrepository.com/

______________________________________________________________________

## Concept (what is it?)

In Maven, each dependency is identified by **coordinates**:

- `groupId`
- `artifactId`
- (optional) `version`

You add these coordinates to your `pom.xml` inside `<dependencies>...</dependencies>`.

To *find* the correct coordinates (and version), you usually search a repository index like **MvnRepository**:

- https://mvnrepository.com/

______________________________________________________________________

## How to find a dependency (step-by-step)

1. Go to https://mvnrepository.com/

1. Search for the library name (examples: `spring-boot-devtools`, `lombok`, `postgresql`)

1. Open the artifact page and confirm:

- groupId
- artifactId
- latest/stable version

4. Copy the **Maven** snippet and paste it into your `pom.xml`

1. Choose the right **scope** (common ones):

- `compile` (default): needed to compile + run
- `runtime`: only needed when running
- `test`: only for tests

6. Verify what got pulled in:

```bash
./mvnw dependency:tree
```

______________________________________________________________________

## Example 1 — Spring Boot DevTools

Search on MvnRepository:

- https://mvnrepository.com/artifact/org.springframework.boot/spring-boot-devtools

If your project uses `spring-boot-starter-parent` (or a Spring Boot BOM), you typically **omit** `<version>` because Spring Boot manages it:

```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-devtools</artifactId>
  <scope>runtime</scope>
  <optional>true</optional>
</dependency>
```

______________________________________________________________________

## Example 2 — Lombok (compile-time only)

Search:

- https://mvnrepository.com/artifact/org.projectlombok/lombok

Typical Maven snippet (version may be managed or not depending on your parent/BOM):

```xml
<dependency>
  <groupId>org.projectlombok</groupId>
  <artifactId>lombok</artifactId>
  <version>1.18.36</version>
  <scope>provided</scope>
</dependency>
```

Notes:

- `provided` means: needed to compile, not packaged at runtime.
- IDE support usually requires enabling annotation processing.

______________________________________________________________________

## Example 3 — PostgreSQL JDBC driver (runtime)

Search:

- https://mvnrepository.com/artifact/org.postgresql/postgresql

```xml
<dependency>
  <groupId>org.postgresql</groupId>
  <artifactId>postgresql</artifactId>
  <scope>runtime</scope>
</dependency>
```

If you’re not using any dependency management, you’ll also include `<version>...</version>` (copy it from MvnRepository).

______________________________________________________________________

## When should you NOT copy the version?

If your `pom.xml` has a parent/BOM that manages versions (common in Spring Boot), adding versions manually can:

- override the platform-tested version
- create conflicts across transitive dependencies

How to confirm what version Maven will use:

```bash
./mvnw help:effective-pom
```

______________________________________________________________________

## Links

- MvnRepository (search dependencies): https://mvnrepository.com/
- Maven Central (official repository): https://repo.maven.apache.org/maven2/
- Maven dependency plugin (`dependency:tree`): https://maven.apache.org/plugins/maven-dependency-plugin/
