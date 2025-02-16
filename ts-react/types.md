# Omit

```tsx
export type songProps = {
  id: number;
  name: string;
  bpm: number;
  compass: string;
  lyric: string;
  tone: string;
  createdAt: string;
  albums: albumProps[];
};

type data = Omit<songProps, "id" | "createdAt" | "albums">;
```

# Record

```tsx
const userRoles: Record<string, string> = {
  admin: "Administrator",
  editor: "Content Editor",
  user: "Regular User",
};
type User = {
  name: string;
  age: number;
};

type UserMap = Record<string, User>;

const users: UserMap = {
  user1: { name: "Alice", age: 25 },
  user2: { name: "Bob", age: 30 },
};
```
