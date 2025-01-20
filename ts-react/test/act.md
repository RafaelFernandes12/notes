# act

The act function is to simulate screen updates, like when you use useState or useEffect hook.

```typescriptreact
import { renderHook } from '@testing-library/react-hooks';
import { useState } from 'react';

function useCounter() {
  const [count, setCount] = useState(0);
  return { count, increment: () => setCount((c) => c + 1) };
}

test('increments count', () => {
  const { result } = renderHook(() => useCounter());

  result.current.increment(); // Triggers state update

  // This might fail because React hasn't finished processing the update yet
  expect(result.current.count).toBe(1);
});
```

With `act`

```typescriptreact
import { renderHook, act } from '@testing-library/react-hooks';
import { useState } from 'react';

function useCounter() {
  const [count, setCount] = useState(0);
  return { count, increment: () => setCount((c) => c + 1) };
}

test('increments count', () => {
  const { result } = renderHook(() => useCounter());

  act(() => {
    result.current.increment(); // Triggers state update inside act
  });

  // Now React has processed the state update
  expect(result.current.count).toBe(1);
});
```

## Example with page component

```tsx
``import React, { useState } from 'react';

export function Counter() {
  const [count, setCount] = useState(0);

  return (
    <div>
      <p data-testid="count">Count: {count}</p>
      <button onClick={() => setCount((prev) => prev + 1)} data-testid="increment-button">
        Increment
      </button>
    </div>
  );
}
```

```tsx
import { render, screen, fireEvent, act } from "@testing-library/react";
import { Counter } from "./Counter";

test("increments the counter", () => {
  render(<Counter />);

  const countElement = screen.getByTestId("count");
  const incrementButton = screen.getByTestId("increment-button");

  // Initial state
  expect(countElement).toHaveTextContent("Count: 0");

  // Use act to wrap the button click that updates the state
  act(() => {
    fireEvent.click(incrementButton);
  });

  // Assert the updated state
  expect(countElement).toHaveTextContent("Count: 1");
});
```
