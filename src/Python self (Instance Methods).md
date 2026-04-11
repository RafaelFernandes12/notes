# Python self (Instance Methods)

______________________________________________________________________

**Date:** 2026-03-10
**Tags:** [Python](../tags/Python.md), [OOP](../tags/OOP.md)
**URL:** https://docs.python.org/3/tutorial/classes.html | https://docs.python.org/3/reference/datamodel.html#object.__getattribute__

______________________________________________________________________

## Concept (what is `self`?)

In Python, `self` is the **instance** (the object) passed to an **instance method**.

It’s not a keyword (you can name it something else), but by convention it’s always called `self`.

When you write:

```py
obj.method(123)
```

Python actually calls:

```py
Class.method(obj, 123)
```

So `self` is how the method gets access to that specific object’s data.

______________________________________________________________________

## Example 1) Instance attributes

```py
class Counter:
    def __init__(self, start: int = 0):
        self.value = start  # instance attribute

    def inc(self) -> int:
        self.value += 1
        return self.value


c1 = Counter()
c2 = Counter(10)

print(c1.inc())  # 1
print(c1.inc())  # 2
print(c2.inc())  # 11
```

- `self.value` belongs to **each instance** (`c1` and `c2` keep different values).

______________________________________________________________________

## Example 2) `self` is passed implicitly

```py
class Greeter:
    def hello(self, name: str) -> str:
        return f"Hi {name}, I'm {self}!"


g = Greeter()

print(g.hello("Rafael"))
print(Greeter.hello(g, "Rafael"))  # same call, explicit self
```

This is why methods must declare `self` as the first parameter.

______________________________________________________________________

## Example 3) Bound vs unbound function (attribute lookup)

When you access a function defined on a class through an instance, Python returns a **bound method** (it “remembers” the instance as `self`).

```py
class A:
    def f(self, x: int) -> int:
        return x + 1


a = A()
m = a.f

print(m(10))        # 11 (self already bound)
print(A.f(a, 10))   # 11 (manual self)
```

The binding is part of Python’s descriptor protocol.

______________________________________________________________________

## `self` vs `cls` (instance vs class)

### `@classmethod`

Class methods receive the **class** as the first argument, usually named `cls`:

```py
class User:
    def __init__(self, email: str):
        self.email = email

    @classmethod
    def from_github(cls, username: str) -> "User":
        return cls(email=f"{username}@users.noreply.github.com")
```

### `@staticmethod`

Static methods receive **no implicit first argument**:

```py
class Math:
    @staticmethod
    def add(a: int, b: int) -> int:
        return a + b
```

______________________________________________________________________

## Common mistakes

### 1) Forgetting `self`

```py
class X:
    def f():
        return 1

X().f()  # TypeError: takes 0 positional arguments but 1 was given
```

Fix: `def f(self): ...`

### 2) Using a class attribute when you wanted an instance attribute

```py
class Bad:
    items = []  # shared across all instances!

    def add(self, x):
        self.items.append(x)
```

If you want per-instance state, create it in `__init__`:

```py
class Good:
    def __init__(self):
        self.items = []
```
