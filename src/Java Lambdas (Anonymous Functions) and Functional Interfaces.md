# Java Lambdas (Anonymous Functions) and Functional Interfaces

______________________________________________________________________

**Date:** 2026-03-16
**Tags:**
- [Java](../tags/Java.md)
- [Spring_Boot](../tags/Spring_Boot.md)
- [Spring_Security](../tags/Spring_Security.md)
**URL:**
- https://docs.oracle.com/javase/tutorial/java/javaOO/lambdaexpressions.html
- https://docs.oracle.com/javase/8/docs/api/java/util/function/package-summary.html

______________________________________________________________________

## Concept: what is a lambda in Java?

A **lambda expression** is a compact way to write an **anonymous function** (more precisely: an **anonymous implementation of a functional interface**).

It’s used when you want to pass “behavior” as data, like:

- “how to transform this value”
- “what to do when an event happens”
- “how to decide if something matches”

### The syntax

```java
(parameters) -> expression
// or
(parameters) -> { statements; }
```

Examples:

```java
x -> x + 1
(a, b) -> a + b
user -> { log(user); return user.getEmail(); }
```

## Key idea: lambdas target a *functional interface*

A **functional interface** is an interface with **exactly one abstract method**.

Examples in the JDK:

- `Function<T, R>`: `R apply(T t)`
- `Predicate<T>`: `boolean test(T t)`
- `Consumer<T>`: `void accept(T t)`
- `Supplier<T>`: `T get()`

You can create your own:

```java
@FunctionalInterface
interface Formatter {
  String format(String input);
}
```

Then you can pass a lambda anywhere a `Formatter` is expected:

```java
Formatter f = s -> s.trim().toUpperCase();
```

## Example 1 (your Spring Security case): `username -> ...`

In Spring Security, `UserDetailsService` is a functional interface:

```java
public interface UserDetailsService {
  UserDetails loadUserByUsername(String username) throws UsernameNotFoundException;
}
```

So this:

```java
return username -> userRepository.findByEmail(username)
    .map(u -> org.springframework.security.core.userdetails.User.builder()
        .username(u.getEmail())
        .password(u.getPassword())
        .build())
    .orElseThrow(() -> new UsernameNotFoundException("User not found"));
```

means:

- Create a `UserDetailsService`
- When Spring calls `loadUserByUsername(...)`, the parameter is called `username`
- You decide to interpret `username` as **email** and load it from the database

Equivalent code **without** lambdas (anonymous class):

```java
return new UserDetailsService() {
  @Override
  public UserDetails loadUserByUsername(String username) {
    return userRepository.findByEmail(username)
        .map(u -> org.springframework.security.core.userdetails.User.builder()
            .username(u.getEmail())
            .password(u.getPassword())
            .build())
        .orElseThrow(() -> new UsernameNotFoundException("User not found"));
  }
};
```

So **`username ->`** is just “the parameter named `username` of the function”.

## Example 2: `Optional.map(...)` uses a lambda

`Optional.map` expects a `Function<T, R>`.

```java
Optional<String> email = Optional.of("rafael@gmail.com");

Optional<Integer> length = email.map(s -> s.length());
//                         ^ lambda: Function<String, Integer>
```

Same idea: “given a `String s`, return `s.length()`”.

## Example 3: `Stream.filter(...)` with a lambda

```java
List<String> names = List.of("ana", "rafael", "bob");

List<String> longOnes = names.stream()
    .filter(n -> n.length() >= 4)   // Predicate<String>
    .toList();
```

Here the lambda `n -> n.length() >= 4` is a `Predicate<String>`.

## Common details that matter

### Parameter types can be inferred

```java
// type inferred
s -> s.length()

// explicit type also works
(String s) -> s.length()
```

### Parentheses are optional for single parameter

```java
x -> x * 2
(x) -> x * 2
```

### Expression vs block body

```java
// expression body: returns automatically
x -> x + 1

// block body: you must use 'return' when returning a value
x -> { 
  int y = x + 1;
  return y;
}
```

### Capturing variables ("effectively final")

Lambdas can use local variables only if they are **effectively final**:

```java
int min = 4;
Predicate<String> p = s -> s.length() >= min; // ok

// min = 5; // would break: min is no longer effectively final
```

### Method references are a shortcut for some lambdas

```java
list.forEach(System.out::println);
// instead of: list.forEach(x -> System.out.println(x));
```

## Mental model

Whenever you see:

```java
param -> something(param)
```

read it as:

> “When someone calls this function with `param`, do `something` and return the result (if any).”
