# Maven Wrapper (mvnw) Commands

______________________________________________________________________

**Date:** 2026-03-02
**Tags:** [Maven](../tags/Maven.md), [Java](../tags/Java.md), [Spring_Boot](../tags/Spring_Boot.md), [Bash](../tags/Bash.md)
**URL:** https://maven.apache.org/wrapper/

______________________________________________________________________

## Concept (what is it?)

**Maven Wrapper** (`mvnw` / `mvnw.cmd`) is a small script committed to a project so you can run Maven **without having Maven installed globally**.

When you run `./mvnw ...` it:

- downloads a specific Maven distribution (first run only) to `~/.m2/wrapper/dists/...`
- runs that Maven version against your project

It still needs a **JDK** to compile Java projects (because it calls `javac`).

---

## How to use (common commands)

### Cheat sheet (goals/plugins you’ll use a lot)

Lifecycle goals:

```bash
./mvnw validate
./mvnw compile
./mvnw test
./mvnw package
./mvnw verify
./mvnw install
```

Common flags:

```bash
./mvnw -q package                 # quiet
./mvnw -DskipTests package         # skip running tests (still compiles them)
./mvnw -Dtest=MyTest test          # run a single test class
./mvnw -Dtest=MyTest#oneCase test  # run a single test method
./mvnw -Pdev test                  # activate Maven profile "dev"
```

Useful info commands:

```bash
./mvnw help:effective-pom
./mvnw help:effective-settings
./mvnw dependency:tree
./mvnw dependency:go-offline
```

Multi-module (reactor) tips:

```bash
./mvnw -pl :module-a test          # only module-a (by artifactId)
./mvnw -pl module-a -am test       # also build required dependencies (-am)
```

### Check versions (debug your environment)

```bash
./mvnw -v
java -version
javac -version
```

### Run a Spring Boot app

```bash
./mvnw spring-boot:run
```

Quiet mode (less logs):

```bash
./mvnw spring-boot:run --quiet
```

### Clean build output

```bash
./mvnw clean
```

Useful after changing Java versions (clears `target/`):

```bash
./mvnw clean spring-boot:run
```

### Build a runnable JAR

```bash
./mvnw clean package
java -jar target/*.jar
```

### Run tests

```bash
./mvnw test
```

Skip tests (faster local iteration):

```bash
./mvnw package -DskipTests
```

### Extra diagnostics (when something fails)

Show full stack traces:

```bash
./mvnw -e test
```

Show debug logs:

```bash
./mvnw -X test
```

---

## Example 1 — Running from the “wrong” directory

`mvnw` lives next to your `pom.xml`.

If you are in a parent folder and `./mvnw` doesn’t exist, either:

```bash
cd path/to/project
./mvnw spring-boot:run
```

or run it with the full relative path:

```bash
./path/to/project/mvnw spring-boot:run
```

---

## Example 2 — “release version 21 not supported”

This happens when Maven is calling a `javac` that **doesn’t support** the configured `--release` (for example, trying to compile for Java 21 with a Java 17 compiler).

Checklist:

```bash
./mvnw -v        # check “Java version” and “Java home”
javac -version   # must exist and match the intended JDK
```

Common fix on Linux distros: install the *JDK* (not only the runtime/headless package) so `javac` exists.

---

## Example 3 — UnsupportedClassVersionError

If you see something like:

```
UnsupportedClassVersionError: ... class file version 69.0 ... up to 65.0
```

It means you compiled with a newer Java than the runtime supports.

Typical fix:

```bash
./mvnw clean
./mvnw package
java -version
java -jar target/*.jar
```

---

## Notes & links

- Maven Wrapper: https://maven.apache.org/wrapper/
- Config lives in `.mvn/wrapper/maven-wrapper.properties`
- Spring Boot Maven plugin (`spring-boot:run`): https://docs.spring.io/spring-boot/maven-plugin/
- Maven dependency plugin: https://maven.apache.org/plugins/maven-dependency-plugin/
- Maven Surefire plugin (unit tests): https://maven.apache.org/surefire/maven-surefire-plugin/
