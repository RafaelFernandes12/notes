# UseMutation

```tsx
import { useMutation } from "@tanstack/react-query";

const mutation = useMutation({
  mutationFn: (newData) => {
    // Perform the mutation (e.g., send a POST request)
    return fetch("/api/data", {
      method: "POST",
      body: JSON.stringify(newData),
    }).then((res) => res.json());
  },
  onSuccess: (data) => {
    // Handle success (e.g., update the UI or show a notification)
    console.log("Mutation succeeded:", data);
  },
  onError: (error) => {
    // Handle error (e.g., show an error message)
    console.error("Mutation failed:", error);
  },
  // it's called before the mutation is executed
  onMutate: (newTodo) => {
    // Optimistically update the UI
    queryClient.setQueryData(["todos"], (oldTodos) => [
      ...oldTodos,
      { id: Date.now(), ...newTodo },
    ]);

    // Return a context object to use in onError or onSettled
    return { previousTodos: queryClient.getQueryData(["todos"]) };
  },
  // it's called after the mutation is executed whether it succeeded or not
  onSettled: (data, error, variables, context) => {
    // Perform cleanup or final actions
    console.log("Mutation settled:", data, error);

    // Optionally refetch data
    queryClient.invalidateQueries(["todos"]);
  },
});

mutation.mutate({ name: "New Item" });
<button onClick={() => mutate({ name: "newData" })}>
  <EditIcon />
</button>;
```

- mutationFn: The function that performs the mutation (e.g., an API call).
- mutate: A function to trigger the mutation. Pass the data you want to send to the server as an argument - isLoading: A boolean indicating whether the mutation is in progress.
- isError: A boolean indicating whether the mutation failed
- isSuccess: A boolean indicating whether the mutation succeeded.
- data: The data returned by the mutation after it succeeds.
- error: The error object if the mutation fails.

* onSuccess: A callback that runs when the mutation succeeds.
* onError: A callback that runs when the mutation fails.

# UseQuery

```tsx
const fetchTodoById = async (id) => {
  const response = await fetch(`/api/todos/${id}`);
  return response.json();
};

const { data, isLoading, isError, error } = useQuery({
  queryKey: ["todo", id], // Unique key with parameters
  queryFn: () => fetchTodoById(id), // Function to fetch data
});

if (isLoading) return <div>Loading...</div>;
if (isError) return <div>Error: {error.message}</div>;

return (
  <div>
    <h1>{data.title}</h1>
    <p>{data.description}</p>
  </div>
);
```

- queryKey: A unique key (usually an array) that identifies the query. Used for caching and refetching.
- queryFn: The function that performs the data fetching (e.g., an API call).
- data: The data returned by the query after it succeeds.
- isLoading: A boolean indicating whether the query is in progress (initial load).
- isError: A boolean indicating whether the query failed.
- error: The error object if the query fails.
- isSuccess: A boolean indicating whether the query succeeded.
- refetch: A function to manually refetch the data.
- isFetching: A boolean indicating whether the query is fetching data (including background refetches).
- staleTime: The time (in milliseconds) before the data is considered stale and eligible for refetching.
- cacheTime: The time (in milliseconds) before inactive queries are removed from the cache.

```tsx
const { data, isLoading, isError, error } = useQuery({
  queryKey: ["todos"],
  queryFn: fetchTodos,
  refetchOnWindowFocus: true, // Refetch data when the window regains focus
  staleTime: 5000, // Data is considered fresh for 5 seconds
  refetchInterval: 3000, // Refetch data every 3 seconds
});
```

## dependant Queries

```tsx
const fetchUser = async (userId) => {
  const response = await fetch(`/api/users/${userId}`);
  return response.json();
};

const fetchPosts = async (userId) => {
  const response = await fetch(`/api/users/${userId}/posts`);
  return response.json();
};

const { data: user, isLoading: isUserLoading } = useQuery({
  queryKey: ["user", userId],
  queryFn: () => fetchUser(userId),
});

const { data: posts, isLoading: isPostsLoading } = useQuery({
  queryKey: ["posts", userId],
  queryFn: () => fetchPosts(userId),
  enabled: !!user, // Only fetch posts if user data is available
});

if (isUserLoading || isPostsLoading) return <div>Loading...</div>;

return (
  <div>
    <h1>{user.name}</h1>
    <ul>
      {posts.map((post) => (
        <li key={post.id}>{post.title}</li>
      ))}
    </ul>
  </div>
);
```
