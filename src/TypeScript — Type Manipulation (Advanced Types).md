# TypeScript — Type Manipulation (Advanced Types)

---

**Date:** 2026-02-27
**Tags:** [TypeScript](../tags/TypeScript.md) [Js](../tags/Js.md) [Types](../tags/Types.md) [Generics](../tags/Generics.md) [UtilityTypes](../tags/UtilityTypes.md)
**URL:** https://www.typescriptlang.org/docs/handbook/2/types-from-types.html • https://www.typescriptlang.org/docs/handbook/2/conditional-types.html • https://www.typescriptlang.org/docs/handbook/utility-types.html • https://www.typescriptlang.org/docs/handbook/2/template-literal-types.html

---

## Concept (what)

In TypeScript, a **type** is (mostly) a _compile-time_ description of values.

“Manipulating types” means taking existing types and producing new ones (usually to:

- avoid duplicating shapes
- keep code refactor-friendly (derive types from real functions/services)
- make APIs stricter (or intentionally looser) in specific places

## Examples from this codebase

### 1) Derive a type from a service method (ReturnType + Awaited)

`src/models/models.ts`:

```ts
export type UserType = Awaited<ReturnType<FindUserService["execute"]>>;
```

Why this is useful:

- you don’t manually re-describe the shape returned by `FindUserService.execute`
- if `execute()` return type changes, your `UserType` changes automatically

Related patterns you’ll see a lot:

```ts
type ExecuteReturn = ReturnType<MyService["execute"]>;
type ExecuteResolved = Awaited<ExecuteReturn>;
```

### 2) Extract a nested part of a derived type (indexed access)

`src/modules/certificates/services/utils/brysign.ts`:

```ts
type UserType = Awaited<ReturnType<FindUserService["execute"]>>["persons"];
```

That `[...] ["persons"]` is an **indexed access type**: “give me the type of the `persons` field”.

### 3) Use `as const` to keep literal types (GraphQL union)

`src/modules/users/input/login-user-response.input.ts`:

```ts
types: () => [LoginSuccessResponse, LoginErrorResponse] as const,
```

`as const` makes the array a readonly tuple of those _exact_ classes (instead of “array of something”).

### 4) Use `Record<K, V>` for “map-like” shapes

In a resolver context type:

```ts
req: {
  headers: Record<string, string | undefined>;
}
```

And in `SendMail.execute`:

```ts
obj: Record<string, unknown>;
```

This is a quick way to say “an object whose keys are strings and values are X”.

## Type manipulation cheat sheet (how)

### A) Combine types: union (`|`) and intersection (`&`)

```ts
type Status = "pending" | "paid" | "canceled";

type WithId = { id: number };
type WithTimestamps = { createdAt: Date; updatedAt: Date };

type Entity = WithId & WithTimestamps;
```

Use cases:

- union: represent multiple allowed variants (enums, result types)
- intersection: compose multiple concerns into a single object type

### B) Make properties optional/required/readonly (mapped types)

Built-in utilities:

```ts
type User = { id: number; email: string; nickname: string };

type UserPatch = Partial<User>; // all optional
type UserRequired = Required<User>; // all required
type UserRO = Readonly<User>; // readonly
```

Custom mapped type:

```ts
type Nullable<T> = { [K in keyof T]: T[K] | null };
type NullableUser = Nullable<User>;
```

### C) Select or remove fields: Pick / Omit / Record

```ts
type User = {
  id: number;
  email: string;
  passwordHash: string;
  nickname: string;
};

type PublicUser = Omit<User, "passwordHash">;
type LoginDTO = Pick<User, "email"> & { password: string };

type Headers = Record<string, string | undefined>;
```

### D) Ask the type system questions: keyof / typeof / indexed access

```ts
const config = { env: "dev", port: 4090 };

type Config = typeof config; // { env: string; port: number }
type ConfigKey = keyof Config; // "env" | "port"
type PortType = Config["port"]; // number
```

Indexed access is great for deep shapes:

```ts
type ApiResponse = { data: { items: { id: string }[] } };
type Item = ApiResponse["data"]["items"][number]; // { id: string }
```

### E) Derive from functions: Parameters / ReturnType / Awaited

```ts
async function fetchUser(id: number) {
  return { id, email: "a@b.com" };
}

type FetchUserArgs = Parameters<typeof fetchUser>; // [id: number]
type FetchUserResult = Awaited<ReturnType<typeof fetchUser>>; // { id: number; email: string }
```

### F) Filter unions: Extract / Exclude / NonNullable

```ts
type Event =
  | { type: "created"; id: string }
  | { type: "deleted"; id: string }
  | { type: "failed"; error: string };

type Failure = Extract<Event, { type: "failed" }>;
type NonFailure = Exclude<Event, { type: "failed" }>;

type Maybe = string | null | undefined;
type Def = NonNullable<Maybe>; // string
```

### G) Conditional types + infer (build “type functions”)

```ts
type UnwrapPromise<T> = T extends Promise<infer U> ? U : T;
type UnwrapArray<T> = T extends (infer U)[] ? U : T;

type A = UnwrapPromise<Promise<number>>; // number
type B = UnwrapArray<string[]>; // string
```

Common use cases:

- “if it’s an array, get element type”
- “if it’s a promise, get resolved type”
- “build helper types around libraries without retyping their results”

### H) Template literal types (build string unions)

```ts
type HttpMethod = "GET" | "POST";
type Route = "/users" | "/users/:id";

type Endpoint = `${HttpMethod} ${Route}`;
// "GET /users" | "GET /users/:id" | "POST /users" | "POST /users/:id"
```

Use cases:

- event names: `"user.created" | "user.deleted"`
- building strongly typed “command names” / “job names”

### I) `as const` vs `satisfies` (control inference)

```ts
const roles = ["admin", "user"] as const;
type Role = (typeof roles)[number]; // "admin" | "user"
```

`satisfies` checks a shape without _widening_ it:

```ts
const jobNames = {
  RegistrationMail: "RegistrationMail",
  ForgotPassword: "ForgotPassword",
} satisfies Record<string, string>;

type JobName = (typeof jobNames)[keyof typeof jobNames];
```

## Practical patterns (when to use what)

- **DTOs / Inputs**: `Pick` + `Omit` + `Partial` are the workhorses.
- **Refactor-safe types**: prefer `ReturnType`/`Parameters` to “copy-pasting shapes”.
- **Discriminated unions**: model success/error responses and narrow with `if ("field" in value)`.
- **External payloads**: start with `unknown` (or `Record<string, unknown>`) and validate/narrow.
