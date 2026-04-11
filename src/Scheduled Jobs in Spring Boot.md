# Scheduled Jobs in Spring Boot

______________________________________________________________________

**Date:** 2026-04-02
**Tags:**
- [Java](../tags/Java.md)
- [Spring_Boot](../tags/Spring_Boot.md)
- [BackgroundJobs](../tags/BackgroundJobs.md)
- [Scheduling](../tags/Scheduling.md)
**URL:**
- https://docs.spring.io/spring-framework/reference/integration/scheduling.html
- https://docs.spring.io/spring-framework/docs/current/javadoc-api/org/springframework/scheduling/annotation/Scheduled.html
- https://docs.spring.io/spring-framework/docs/current/javadoc-api/org/springframework/scheduling/annotation/EnableScheduling.html
- https://docs.spring.io/spring-boot/reference/features/task-execution-and-scheduling.html

______________________________________________________________________

## Concept (what is a job in Spring Boot?)

In Spring Boot, a **job** is usually a piece of code that runs **in the background without an HTTP request starting it**.

Common uses:

- syncing data from another system
- cleaning temporary files
- refreshing caches
- generating reports
- sending retries or follow-up notifications

For **simple recurring jobs**, the most common Spring approach is:

1. enable scheduling with `@EnableScheduling`
2. create a Spring bean (`@Component`, `@Service`, etc.)
3. annotate a method with `@Scheduled`

So the basic mental model is:

- **Spring creates the bean**
- **Spring scheduler calls the method automatically**
- **you choose when it runs** with `fixedDelay`, `fixedRate`, or `cron`

## Minimal recipe

### 1) Enable scheduling

```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableScheduling;

@SpringBootApplication
@EnableScheduling
public class DemoApplication {
  public static void main(String[] args) {
    SpringApplication.run(DemoApplication.class, args);
  }
}
```

Without `@EnableScheduling`, methods annotated with `@Scheduled` are just normal methods and will not run automatically.

### 2) Create a bean with a scheduled method

```java
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

@Component
public class HeartbeatJob {

  @Scheduled(fixedDelay = 10000)
  public void run() {
    System.out.println("Job executed");
  }
}
```

Important rules:

- the method should not receive parameters
- the return value is ignored
- the job class must be managed by Spring

## Choosing the trigger type

Spring gives you three very common ways to schedule jobs:

- `fixedDelay`: waits for the previous execution to finish, then counts the delay
- `fixedRate`: tries to keep a regular interval between start times
- `cron`: runs at calendar-based times such as every day at 02:00

## Example 1 — `fixedDelay` job for cleanup

Use `fixedDelay` when you want to avoid starting a new execution immediately after the previous one starts. It is a good default for maintenance jobs.

```java
import java.time.Duration;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

@Component
public class TempFileCleanupJob {

  private final TempFileService tempFileService;

  public TempFileCleanupJob(TempFileService tempFileService) {
    this.tempFileService = tempFileService;
  }

  @Scheduled(fixedDelayString = "${jobs.temp-cleanup.delay-ms:600000}")
  public void deleteOldFiles() {
    tempFileService.deleteFilesOlderThan(Duration.ofHours(24));
  }
}
```

Example property:

```yaml
jobs:
  temp-cleanup:
    delay-ms: 600000
```

Why this is useful:

- you can change the interval without recompiling
- `fixedDelay` is safer for jobs that may sometimes take longer
- the job reads like normal application code because dependencies are injected the usual Spring way

## Example 2 — `fixedRate` job for periodic refresh

Use `fixedRate` when you care more about a regular cadence.

```java
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

@Component
public class ExchangeRateRefreshJob {

  private final ExchangeRateClient exchangeRateClient;
  private final ExchangeRateCache exchangeRateCache;

  public ExchangeRateRefreshJob(
      ExchangeRateClient exchangeRateClient,
      ExchangeRateCache exchangeRateCache
  ) {
    this.exchangeRateClient = exchangeRateClient;
    this.exchangeRateCache = exchangeRateCache;
  }

  @Scheduled(
      initialDelayString = "${jobs.exchange-rates.initial-delay-ms:5000}",
      fixedRateString = "${jobs.exchange-rates.rate-ms:30000}"
  )
  public void refreshRates() {
    var latestRates = exchangeRateClient.fetchLatest();
    exchangeRateCache.replaceAll(latestRates);
  }
}
```

This example is different from the cleanup job:

- `initialDelay` gives the app time to start before the first run
- `fixedRate` is useful when you want something like “every 30 seconds”
- it is a good fit for refreshing caches, metrics, or reference data

Practical note: if a job can run for a long time, `fixedRate` may be a bad fit. In many real systems, `fixedDelay` is easier to reason about.

## Example 3 — cron-based job with feature flag (from your codebase)

Your codebase already has a good real example of a Spring Boot job.

Scheduling is enabled in the application class:

```java
@SpringBootApplication
@EnableScheduling
@EnableConfigurationProperties(ClientSyncProperties.class)
public class CxhubApplication {

  public static void main(String[] args) {
    SpringApplication.run(CxhubApplication.class, args);
  }
}
```

The job configuration is externalized:

```java
@ConfigurationProperties(prefix = "cxhub.sync")
public class ClientSyncProperties {
  private boolean enabled;
  private Long targetTenantId;
  private String cron = "0 */5 * * * *";

  public boolean isEnabled() {
    return enabled;
  }

  public void setEnabled(boolean enabled) {
    this.enabled = enabled;
  }

  public Long getTargetTenantId() {
    return targetTenantId;
  }

  public void setTargetTenantId(Long targetTenantId) {
    this.targetTenantId = targetTenantId;
  }

  public String getCron() {
    return cron;
  }

  public void setCron(String cron) {
    this.cron = cron;
  }
}
```

And the scheduled job itself:

```java
@Component
public class ClientSyncScheduler {

  private final ClientSyncProperties clientSyncProperties;
  private final ClientSyncService clientSyncService;
  private final AuditService auditService;

  public ClientSyncScheduler(
      ClientSyncProperties clientSyncProperties,
      ClientSyncService clientSyncService,
      AuditService auditService
  ) {
    this.clientSyncProperties = clientSyncProperties;
    this.clientSyncService = clientSyncService;
    this.auditService = auditService;
  }

  @Scheduled(cron = "${cxhub.sync.cron:0 */5 * * * *}")
  public void synchronizeClients() {
    if (!clientSyncProperties.isEnabled()) {
      return;
    }

    try {
      clientSyncService.synchronizeClients();
    } catch (RuntimeException exception) {
      auditService.record(
          null,
          clientSyncProperties.getTargetTenantId(),
          "CLIENT_SYNC_FAILED",
          "Client sync failed for tenant %s: %s".formatted(
              clientSyncProperties.getTargetTenantId(),
              exception.getMessage()
          )
      );
    }
  }
}
```

And the properties come from `application.yml`:

```yaml
cxhub:
  sync:
    enabled: ${CXHUB_SYNC_ENABLED:false}
    target-tenant-id: ${CXHUB_TARGET_TENANT_ID:0}
    cron: ${CXHUB_CLIENT_SYNC_CRON:0 */5 * * * *}
```

Why this is a strong example:

- the schedule is configurable
- the job can be turned on/off with a feature flag
- the job delegates business logic to a service instead of putting everything in the scheduler class
- failures are handled explicitly and audited

This is a very good pattern for real applications: keep the scheduled method small and let services do the actual work.

## Understanding Spring cron expressions

Spring cron uses **6 fields**, not the traditional Linux 5-field format:

```text
second minute hour day-of-month month day-of-week
```

Examples:

- `0 * * * * *` → every minute
- `0 */5 * * * *` → every 5 minutes
- `0 0 2 * * *` → every day at 02:00
- `0 0 9 * * MON-FRI` → weekdays at 09:00

You can also define a time zone:

```java
@Scheduled(cron = "0 0 9 * * MON-FRI", zone = "America/Sao_Paulo")
public void sendMorningSummary() {
  // ...
}
```

## Good practices for Spring jobs

### 1) Keep the scheduled method thin

Prefer this:

```java
@Scheduled(cron = "0 */10 * * * *")
public void runBillingSync() {
  billingSyncService.syncPendingInvoices();
}
```

Instead of putting database access, loops, remote calls, and error formatting directly in the job method.

### 2) Make jobs idempotent when possible

If the same job runs twice, the result should ideally still be safe. This matters a lot for retries, restarts, and multi-instance deployments.

### 3) Handle exceptions deliberately

Even when Spring keeps scheduling future executions, you usually want better logs, metrics, or audit records so failures are visible.

### 4) Prefer properties over hardcoded intervals

This:

```java
@Scheduled(cron = "${jobs.invoice-sync.cron:0 */15 * * * *}")
```

is usually better than hardcoding the cron directly in the annotation.

### 5) Remember that every application instance runs the job

If you deploy 3 instances of the same Spring Boot app, all 3 instances may execute the same scheduled method. If you need only one node to run the job, you usually need extra coordination such as a distributed lock, Quartz, or an external scheduler.

### 6) Know the default scheduler is often simple

For small apps, the default configuration is fine. For multiple or long-running jobs, it is common to configure a scheduler pool explicitly:

```java
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.scheduling.TaskScheduler;
import org.springframework.scheduling.concurrent.ThreadPoolTaskScheduler;

@Configuration
public class SchedulingConfig {

  @Bean
  public TaskScheduler taskScheduler() {
    var scheduler = new ThreadPoolTaskScheduler();
    scheduler.setPoolSize(4);
    scheduler.setThreadNamePrefix("jobs-");
    scheduler.initialize();
    return scheduler;
  }
}
```

## When `@Scheduled` is enough vs when you need more

`@Scheduled` is a great fit when:

- the job runs inside the same app
- you just need recurring execution
- persistence of job metadata is not important
- a simple cron/fixed delay/fixed rate is enough

You may need something else when:

- jobs must survive restarts with persistent state
- you need complex retry/recovery flows
- you need distributed coordination
- you need large ETL/batch pipelines

Common alternatives:

- **Quartz** → more advanced scheduling features
- **Spring Batch** → chunk-oriented batch processing
- **Queues/workers** → better when work should be decoupled from the web app and processed asynchronously

## Summary

To create jobs in Spring Boot, the usual path is:

1. enable scheduling with `@EnableScheduling`
2. create a Spring-managed bean
3. add `@Scheduled` to a no-arg method
4. choose `fixedDelay`, `fixedRate`, or `cron`
5. keep the job small, configurable, and safe to run repeatedly

For most application-level recurring work, this is the simplest and most idiomatic Spring Boot solution.
