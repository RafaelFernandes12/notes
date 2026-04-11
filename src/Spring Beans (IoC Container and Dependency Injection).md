# Spring Beans (IoC Container and Dependency Injection)

______________________________________________________________________

**Date:** 2026-03-16
**Tags:**
- [Java](../tags/Java.md)
- [Spring_Boot](../tags/Spring_Boot.md)
- [Annotations](../tags/Annotations.md)
- [Dependency_Injection](../tags/Dependency_Injection.md)
- [Security](../tags/Security.md)
- [Spring_Security](../tags/Spring_Security.md)
**URL:**
- https://docs.spring.io/spring-framework/reference/core/beans.html
- https://docs.spring.io/spring-boot/reference/features/developing-auto-configuration.html
- https://docs.spring.io/spring-security/reference/servlet/authentication/index.html

______________________________________________________________________

## What is a “Bean” in Spring?

A **Spring Bean** is a **Java object whose lifecycle is managed by Spring’s IoC container**.

“Managed by Spring” means Spring decides:

- **How to create it** (constructor, factory method, proxies)
- **When to create it** (startup vs lazy)
- **How to inject dependencies** (other beans)
- **How long it lives** (scope)
- **How to destroy it** (cleanup callbacks)

This is different from:

- **JavaBean** (old Java convention: getters/setters, no-arg constructor)
- **Bean Validation** (Jakarta Validation annotations like `@NotBlank`)

## How Spring finds/creates beans

### 1) Component scanning (`@Component`, `@Service`, `@Repository`, `@Controller`)

If a class is in a scanned package and has a stereotype annotation, Spring registers it as a bean.

In Spring Boot, `@SpringBootApplication` enables component scanning starting from its package.

Example:

```java
@Service
public class AuthService {
  private final UserRepository userRepository;

  public AuthService(UserRepository userRepository) {
    this.userRepository = userRepository;
  }
}
```

### 2) `@Bean` factory methods (usually in `@Configuration`)

Use `@Bean` when:

- the class comes from a library you can’t annotate
- you need custom construction (args, wrapping, choosing an implementation)
- you need multiple variants of the same type

Example:

```java
@Configuration
class CryptoConfig {

  @Bean
  PasswordEncoder passwordEncoder() {
    return new BCryptPasswordEncoder();
  }
}
```

Notes:

- `@Bean` methods **do not need to be `public`**. Many IDEs warn with something like “public @Bean method” (style rule), but it’s not a runtime problem.
- Prefer putting `@Bean` methods in a `@Configuration` class. `@Configuration` uses proxying so calls between `@Bean` methods still return the *container-managed singleton*.

### 3) Auto-configuration (Spring Boot)

Spring Boot creates lots of beans for you automatically (DataSource, MVC, Jackson, Security filters, etc.) based on:

- what’s on the classpath (starters)
- your `application.properties`

But **not every useful object is automatically exposed as a bean** (example below: `AuthenticationManager`).

## Dependency Injection (DI): how beans get other beans

Spring usually injects dependencies by type.

### Constructor injection (recommended)

```java
@Service
class EmailService {
  private final MailClient client;

  EmailService(MailClient client) {
    this.client = client;
  }
}
```

### Field injection (`@Autowired`)

Works, but is harder to test and hides dependencies.

```java
@Service
class ReportService {
  @Autowired
  private Clock clock;
}
```

## Example (from your codebase): why `UserRepository` injects fine

In your project, this works:

```java
@Autowired
private UserRepository userRepository;
```

Because `UserRepository` extends Spring Data JPA’s `JpaRepository`, and Spring Data registers repository beans automatically.

## Example: “required a bean of type AuthenticationManager could not be found”

If you have:

```java
@Autowired
private AuthenticationManager authenticationManager;
```

and Spring can’t find a bean of that type, the app fails at startup.

One common fix is to **publish an `AuthenticationManager` bean**:

```java
@Configuration
class SecurityBeans {

  @Bean
  AuthenticationManager authenticationManager(
      AuthenticationConfiguration configuration
  ) throws Exception {
    return configuration.getAuthenticationManager();
  }
}
```

Then Spring can inject it into services.

Also, for username/password authentication to work with your DB users, you typically provide:

- a `UserDetailsService` that loads users (e.g., by email)
- a `PasswordEncoder` that matches how you stored passwords (e.g., BCrypt)

## Multiple beans of the same type: `@Primary` and `@Qualifier`

If you define two beans of the same interface, injection becomes ambiguous.

```java
interface PaymentGateway { void charge(int cents); }

@Component
@Primary
class StripeGateway implements PaymentGateway { /* ... */ }

@Component
class PaypalGateway implements PaymentGateway { /* ... */ }

@Service
class BillingService {
  BillingService(@Qualifier("paypalGateway") PaymentGateway gateway) {
    // chooses Paypal explicitly
  }
}
```

## Bean scopes (how long a bean lives)

- `singleton` (default): one instance for the whole application context
- `prototype`: a new instance each time it’s requested
- web scopes (servlet apps): `request`, `session`, etc.

Example:

```java
@Bean
@Scope("prototype")
Tokenizer tokenizer() {
  return new Tokenizer();
}
```

## Lifecycle: initialization and cleanup

Common hooks:

```java
@Component
class Cache {

  @PostConstruct
  void init() {
    // warm up cache
  }

  @PreDestroy
  void shutdown() {
    // cleanup resources
  }
}
```

## Common “bean” problems to recognize fast

- **NoSuchBeanDefinition**: no bean of that type exists (missing `@Component`, not in scan path, missing `@Bean`).
- **NoUniqueBeanDefinition**: multiple beans match (use `@Qualifier` / `@Primary`).
- **Blocked endpoints after adding Security**: default config may require auth for everything; you must `permitAll()` for login/register routes.
- **Calling `@Bean` methods directly** from a non-`@Configuration` class can create extra instances (“lite mode”).
