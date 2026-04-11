# JWT Bearer Authentication (Spring Boot + Spring Security)

______________________________________________________________________

**Date:** 2026-03-16
**Tags:**
- [Java](../tags/Java.md)
- [Spring_Boot](../tags/Spring_Boot.md)
- [Spring_Security](../tags/Spring_Security.md)
- [Security](../tags/Security.md)
- [Rest](../tags/Rest.md)
- [JWT](../tags/JWT.md)
**URL:**
- https://www.rfc-editor.org/rfc/rfc7519
- https://www.rfc-editor.org/rfc/rfc6750
- https://docs.spring.io/spring-security/reference/servlet/index.html
- https://github.com/jwtk/jjwt

______________________________________________________________________

This note is a **code walkthrough** of a working approach to JWT Bearer auth using a **custom Spring Security filter**.

It is based on these files from your project:

- `src/main/java/com/example/barbearia_do_matheus/config/SecurityConfig.java`
- `src/main/java/com/example/barbearia_do_matheus/config/JwtAuthenticationFilter.java`
- `src/main/java/com/example/barbearia_do_matheus/controllers/AuthController.java`
- `src/main/java/com/example/barbearia_do_matheus/services/AuthService.java`

## 1) `SecurityConfig.java` — wires Spring Security + JWT filter

File: `src/main/java/com/example/barbearia_do_matheus/config/SecurityConfig.java`

```java
package com.example.barbearia_do_matheus.config;

import java.util.Arrays;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.ProviderManager;
import org.springframework.security.authentication.dao.DaoAuthenticationProvider;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;
import org.springframework.web.cors.CorsConfiguration;
import org.springframework.web.cors.CorsConfigurationSource;
import org.springframework.web.cors.UrlBasedCorsConfigurationSource;

import com.example.barbearia_do_matheus.repository.UserRepository;

@Configuration
@EnableWebSecurity
public class SecurityConfig {

  private final UserRepository userRepository;

  public SecurityConfig(UserRepository userRepository) {
    this.userRepository = userRepository;
  }

  @Bean
  SecurityFilterChain securityFilterChain(HttpSecurity http, JwtAuthenticationFilter jwtAuthFilter) throws Exception {
    http
        // For stateless APIs, CSRF is commonly disabled.
        .csrf(csrf -> csrf.disable())

        // Enable CORS using the bean below.
        .cors(cors -> cors.configurationSource(corsConfigurationSource()))

        // Authorization rules:
        // - allow /auth/** without being logged in
        // - allow /error/** (useful while debugging exceptions)
        // - everything else requires an authenticated user
        .authorizeHttpRequests((authorize) -> authorize
            .requestMatchers("/auth/**").permitAll()
            .requestMatchers("/error/**").permitAll()
            .anyRequest().authenticated())

        // Do not create HTTP sessions. Each request must bring its token.
        .sessionManagement(session -> session
            .sessionCreationPolicy(SessionCreationPolicy.STATELESS))

        // Run our JWT filter BEFORE Spring’s username/password auth filter.
        // Our filter reads Authorization: Bearer <token> and sets the SecurityContext.
        .addFilterBefore(jwtAuthFilter, UsernamePasswordAuthenticationFilter.class);

    return http.build();
  }

  @Bean
  AuthenticationManager authenticationManager() {
    // DaoAuthenticationProvider = “load user from UserDetailsService and verify password”
    DaoAuthenticationProvider authenticationProvider = new DaoAuthenticationProvider(userDetailsService());
    authenticationProvider.setPasswordEncoder(passwordEncoder());

    // ProviderManager is a common AuthenticationManager implementation.
    ProviderManager providerManager = new ProviderManager(authenticationProvider);

    // Optional: keep credentials around after authentication.
    providerManager.setEraseCredentialsAfterAuthentication(false);

    return providerManager;
  }

  @Bean
  CorsConfigurationSource corsConfigurationSource() {
    CorsConfiguration configuration = new CorsConfiguration();

    // Frontend origin allowed to call your API
    configuration.setAllowedOrigins(Arrays.asList("http://localhost:4200"));
    configuration.setAllowedMethods(Arrays.asList("GET", "POST", "PUT", "DELETE", "PATCH", "OPTIONS"));
    configuration.setAllowedHeaders(Arrays.asList("*"));
    configuration.setAllowCredentials(true);

    UrlBasedCorsConfigurationSource source = new UrlBasedCorsConfigurationSource();
    source.registerCorsConfiguration("/**", configuration);
    return source;
  }

  @Bean
  UserDetailsService userDetailsService() {
    // Spring Security calls this during login (AuthenticationManager.authenticate)
    // and also in JwtAuthenticationFilter when validating the token.
    return username -> userRepository.findByEmail(username)
        .map(user -> org.springframework.security.core.userdetails.User.builder()
            .username(user.getEmail())
            .password(user.getPassword())
            .build())
        .orElseThrow(() -> new UsernameNotFoundException("User not found"));
  }

  @Bean
  PasswordEncoder passwordEncoder() {
    // Must match how you store passwords.
    return new BCryptPasswordEncoder();
  }
}
```

### What each bean does (quick map)

- `securityFilterChain(...)`: configures **which routes require auth** + **adds the JWT filter**.
- `authenticationManager()`: used by your `AuthService.login()` to validate email+password.
- `corsConfigurationSource()`: CORS rules for browser clients.
- `userDetailsService()`: how Spring loads a user from the DB.
- `passwordEncoder()`: how passwords are hashed/verified.

## 2) `JwtAuthenticationFilter.java` — reads Bearer token and authenticates the request

File: `src/main/java/com/example/barbearia_do_matheus/config/JwtAuthenticationFilter.java`

```java
package com.example.barbearia_do_matheus.config;

import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.lang.NonNull;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.web.authentication.WebAuthenticationDetailsSource;
import org.springframework.stereotype.Component;
import org.springframework.web.filter.OncePerRequestFilter;

import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.io.Decoders;
import io.jsonwebtoken.security.Keys;

import java.io.IOException;
import java.util.Date;
import java.util.function.Function;

import javax.crypto.SecretKey;

@Component
public class JwtAuthenticationFilter extends OncePerRequestFilter {

  @Value("${application.security.jwt.secret-key}")
  private String secretKey;

  private final UserDetailsService userDetailsService;

  public JwtAuthenticationFilter(UserDetailsService userDetailsService) {
    this.userDetailsService = userDetailsService;
  }

  @Override
  protected void doFilterInternal(
      @NonNull HttpServletRequest request,
      @NonNull HttpServletResponse response,
      @NonNull FilterChain filterChain) throws ServletException, IOException {

    // 1) Get token from Authorization header (Bearer) OR from cookie
    final String authHeader = request.getHeader("Authorization");
    String jwt = null;
    final String userEmail;

    if (authHeader != null && authHeader.startsWith("Bearer ")) {
      jwt = authHeader.substring(7);
    } else if (request.getCookies() != null) {
      for (Cookie cookie : request.getCookies()) {
        if ("user_credentials".equals(cookie.getName())) {
          jwt = cookie.getValue();
        }
      }
    }

    // No token = don’t authenticate; just continue.
    if (jwt == null) {
      filterChain.doFilter(request, response);
      return;
    }

    // 2) Parse token and extract identity (subject)
    userEmail = extractUsername(jwt);

    // 3) If nobody authenticated yet, authenticate this request based on JWT
    if (userEmail != null && SecurityContextHolder.getContext().getAuthentication() == null) {
      UserDetails userDetails = this.userDetailsService.loadUserByUsername(userEmail);

      // 4) Validate signature + expiration + subject
      if (isTokenValid(jwt, userDetails)) {
        UsernamePasswordAuthenticationToken authToken = new UsernamePasswordAuthenticationToken(
            userDetails,
            null,
            userDetails.getAuthorities());

        authToken.setDetails(new WebAuthenticationDetailsSource().buildDetails(request));
        SecurityContextHolder.getContext().setAuthentication(authToken);
      }
    }

    filterChain.doFilter(request, response);
  }

  private String extractUsername(String token) {
    // Your AuthService uses .subject(email), so we read Claims::getSubject
    return extractClaim(token, Claims::getSubject);
  }

  private <T> T extractClaim(String token, Function<Claims, T> claimsResolver) {
    // IMPORTANT: decoding MUST match how you sign tokens in AuthService.
    // If your property is HEX, use Decoders.HEX instead of BASE64.
    byte[] keyBytes = Decoders.BASE64.decode(secretKey);
    SecretKey key = Keys.hmacShaKeyFor(keyBytes);

    final Claims claims = Jwts.parser()
        .verifyWith(key)
        .build()
        .parseSignedClaims(token)
        .getPayload();

    return claimsResolver.apply(claims);
  }

  private boolean isTokenValid(String token, UserDetails userDetails) {
    final String username = extractUsername(token);
    final boolean isTokenExpired = extractClaim(token, Claims::getExpiration).before(new Date());
    return (username.equals(userDetails.getUsername())) && !isTokenExpired;
  }
}
```

### What each method does

- `doFilterInternal(...)`: the main pipeline — extract token → parse → validate → set authentication.
- `extractUsername(...)`: reads the token `sub` (subject) claim.
- `extractClaim(...)`: verifies signature and returns a claim.
- `isTokenValid(...)`: checks subject matches + token not expired.

## 3) `AuthService.java` — login + mint JWT (and register)

File: `src/main/java/com/example/barbearia_do_matheus/services/AuthService.java`

```java
package com.example.barbearia_do_matheus.services;

import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.io.Decoders;
import io.jsonwebtoken.security.Keys;

import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;

import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import javax.crypto.SecretKey;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.web.server.ResponseStatusException;

import com.example.barbearia_do_matheus.dto.Login;
import com.example.barbearia_do_matheus.models.User;
import com.example.barbearia_do_matheus.repository.UserRepository;

import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletResponse;

@Service
public class AuthService {

  @Value("${application.security.jwt.secret-key}")
  private String secretKey;

  @Value("${application.security.jwt.expiration}")
  private long jwtExpiration;

  private UserRepository userRepository;
  private AuthenticationManager authenticationManager;

  public AuthService(UserRepository userRepository, AuthenticationManager authenticationManager) {
    this.userRepository = userRepository;
    this.authenticationManager = authenticationManager;
  }

  public ResponseEntity<Map<String, String>> login(Login login, HttpServletResponse response) {
    // 1) Validate email+password using Spring Security’s AuthenticationManager
    authenticationManager.authenticate(
        new UsernamePasswordAuthenticationToken(
            login.email(),
            login.password()));

    // 2) Load your domain user (for any extra data you want)
    User user = userRepository.findByEmail(login.email()).orElseThrow();

    // 3) Generate JWT (subject = email)
    String jwtToken = generateToken(
        org.springframework.security.core.userdetails.User.builder()
            .username(user.getEmail())
            .password(user.getPassword())
            .build());

    // 4) Optionally also store the JWT in an HttpOnly cookie
    Cookie cookie = new Cookie("user_credentials", jwtToken);
    cookie.setHttpOnly(true);
    cookie.setPath("/");
    cookie.setMaxAge(24 * 60 * 60); // 1 day
    response.addCookie(cookie);

    // 5) Return token in JSON
    return ResponseEntity.ok(Map.of("token", jwtToken));
  }

  public User register(User user) {
    // 1) Prevent duplicates at the application level
    //    (still add a UNIQUE constraint in the DB; this check can race)
    User existingUser = userRepository.findByEmail(user.getEmail()).orElse(null);
    if (existingUser != null) {
      throw new ResponseStatusException(HttpStatus.CONFLICT, "Email already in use");
    }

    // 2) Hash the password before saving
    String hashedPassword = new BCryptPasswordEncoder().encode(user.getPassword());
    user.setPassword(hashedPassword);

    return userRepository.save(user);
  }

  private String generateToken(UserDetails userDetails) {
    // IMPORTANT: decoding MUST match JwtAuthenticationFilter
    byte[] keyBytes = Decoders.BASE64.decode(secretKey);
    SecretKey key = Keys.hmacShaKeyFor(keyBytes);

    return Jwts.builder()
        .claims(new HashMap<>())
        .subject(userDetails.getUsername())
        .issuedAt(new Date(System.currentTimeMillis()))
        .expiration(new Date(System.currentTimeMillis() + jwtExpiration))
        .signWith(key)
        .compact();
  }
}
```

### Notes about these methods

- `login(...)`: authenticates credentials and mints a JWT.
- `register(...)`: checks if email exists, hashes password, saves user.
- `generateToken(...)`: sets `sub`, `iat`, `exp`, signs token.

⚠️ In your current codebase, be careful about password hashing **happening in more than one place** (controller + service). Hash once.

## 4) `AuthController.java` — endpoints for register/login

File: `src/main/java/com/example/barbearia_do_matheus/controllers/AuthController.java`

```java
package com.example.barbearia_do_matheus.controllers;

import java.util.Map;

import org.springframework.http.ResponseEntity;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.barbearia_do_matheus.dto.Login;
import com.example.barbearia_do_matheus.dto.Register;
import com.example.barbearia_do_matheus.models.Barber;
import com.example.barbearia_do_matheus.models.Customer;
import com.example.barbearia_do_matheus.models.User;
import com.example.barbearia_do_matheus.services.AuthService;

import jakarta.servlet.http.HttpServletResponse;

@RestController()
@RequestMapping("auth/")
public class AuthController {

  private final AuthService authService;
  private final PasswordEncoder passwordEncoder;

  public AuthController(AuthService authService, PasswordEncoder passwordEncoder) {
    this.authService = authService;
    this.passwordEncoder = passwordEncoder;
  }

  @PostMapping("login")
  public ResponseEntity<Map<String, String>> login(@RequestBody Login login, HttpServletResponse response) {
    // Delegates to AuthService which authenticates and returns JWT
    return authService.login(login, response);
  }

  @PostMapping("register/customer")
  public User registerCustomer(@RequestBody Register dto) {
    // Convert DTO -> Entity subtype (Customer)
    Customer c = new Customer();
    c.setName(dto.getName());
    c.setEmail(dto.getEmail());
    c.setPassword(dto.getPassword());

    return authService.register(c);
  }

  @PostMapping("register/barber")
  public User registerBarber(@RequestBody Register dto) {
    // Convert DTO -> Entity subtype (Barber)
    Barber b = new Barber();
    b.setName(dto.getName());
    b.setEmail(dto.getEmail());

    // NOTE: if AuthService.register() also hashes, this becomes double hashing.
    b.setPassword(passwordEncoder.encode(dto.getPassword()));

    return authService.register(b);
  }

  @GetMapping("/")
  public String teste() {
    return "hello";
  }
}
```

### What each endpoint does

- `POST /auth/login`: validates credentials, returns JWT (and sets cookie).
- `POST /auth/register/customer`: creates a `Customer` user.
- `POST /auth/register/barber`: creates a `Barber` user.
- `GET /auth/`: test route.

## End-to-end test (curl)

Login:

```bash
curl -s -X POST "http://localhost:8080/auth/login" \
  -H "Content-Type: application/json" \
  -d '{"email":"rafael8@gmail.com","password":"123456"}'
```

Use the returned token:

```bash
curl -i "http://localhost:8080/user/" \
  -H "Authorization: Bearer <TOKEN_HERE>"
```

## Common errors (direct mapping to this code)

- **`SignatureException: JWT signature does not match`**
  - secret key bytes used in `AuthService.generateToken()` ≠ bytes used in `JwtAuthenticationFilter.extractClaim()`
  - common cause: property looks like HEX but you decode as BASE64 (or using an old token)

- **`BadCredentialsException`**
  - wrong password, or password stored incorrectly (plaintext / double-hashed)

- **Duplicates allowed even with `@Column(unique=true)`**
  - DB has no UNIQUE constraint; add a real `UNIQUE(email)` index/constraint.
