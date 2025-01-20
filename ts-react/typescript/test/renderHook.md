# renderHook

The renderHook is used to test react hooks in a isolated environment, since we cant render them on screen.
We can access them via `result.current`

```typescriptreact
import { useState } from 'react';

export function useCounter() {
  const [count, setCount] = useState(0);
  const increment = () => setCount((prev) => prev + 1);

  return { count, increment };
}
```

```typescriptreact
import { renderHook, act } from '@testing-library/react-hooks';
import { useCounter } from './useCounter';

test('increments the counter', () => {
  const { result } = renderHook(() => useCounter());

  // Assert initial state
  expect(result.current.count).toBe(0);

  // Act: Call the increment function
  act(() => {
    result.current.increment();
  });

  // Assert state after increment
  expect(result.current.count).toBe(1);
});
```

## keypoints

### Basic setup

```typescriptreact
const { result } = renderHook(() => useMyCustomHook());
```

- result.current contains the current value returned by the hook.
- It updates automatically whenever the hook's state or props change.

### rerender

Use the rerender function to change the hook's props and re-render the hook.

```typescriptreact
const { result, rerender } = renderHook(({ value }) => useMyHook(value), {
  initialProps: { value: 1 },
});

expect(result.current.someState).toBe(1);

rerender({ value: 2 }); // Change the hook's props
expect(result.current.someState).toBe(2);
```

### act

Wrap actions that trigger state changes or lifecycle updates
in act to ensure React processes updates synchronously.

```typescriptreact
act(() => {
  result.current.increment();
});
```

### useEffect

```typescriptreact
import { useEffect } from 'react';

export function useDocumentTitle(title: string) {
  useEffect(() => {
    document.title = title;
    return () => {
      document.title = 'Cleaned up';
    };
  }, [title]);
}
```

```typescriptreact
import { renderHook } from '@testing-library/react-hooks';
import { useDocumentTitle } from './useDocumentTitle';

test('sets and cleans up document title', () => {
  const { rerender, unmount } = renderHook(({ title }) => useDocumentTitle(title), {
    initialProps: { title: 'Initial Title' },
  });

  expect(document.title).toBe('Initial Title');

  rerender({ title: 'Updated Title' });
  expect(document.title).toBe('Updated Title');

  unmount(); // Simulates component unmount
  expect(document.title).toBe('Cleaned up');
});
```

# unmount

## What is `unmount`?

The `unmount` function is used to simulate the unmounting of a component.
When a component is unmounted, React removes it from the UI
and cleans up any associated resources (like event listeners, subscriptions, or effects).

## When to Use `unmount` in Tests?

- To verify that the cleanup logic in `useEffect` runs correctly.
- To test how your code behaves when a component or hook is removed from the DOM.
- To ensure there are no memory leaks, dangling subscriptions, or unintended side effects.


## Example: Using `unmount`

### Hook: `useEventListener`

```tsx
import { useEffect } from "react";

export function useEventListener(event: string, callback: EventListener) {
  useEffect(() => {
    window.addEventListener(event, callback);
    return () => {
      window.removeEventListener(event, callback); // Cleanup
    };
  }, [event, callback]);
}
```

```typescriptreact
import { renderHook } from '@testing-library/react-hooks';
import { useEventListener } from './useEventListener';

test('removes event listener on unmount', () => {
  const mockCallback = jest.fn();

  // Render the hook
  const { unmount } = renderHook(() =>
    useEventListener('resize', mockCallback)
  );

  // Simulate a resize event
  window.dispatchEvent(new Event('resize'));
  expect(mockCallback).toHaveBeenCalledTimes(1);

  // Unmount the hook
  unmount();

  // Simulate another resize event after unmounting
  window.dispatchEvent(new Event('resize'));
  expect(mockCallback).toHaveBeenCalledTimes(1); // Should not be called again
});
```

### RealLife uses

```typescriptreact
useEffect(() => {
  const controller = new AbortController();
  fetch('/api/data', { signal: controller.signal });
  return () => controller.abort(); // Cleanup API call
}, []);
```

```typescriptreact
useEffect(() => {
  const timer = setTimeout(() => {}, 1000);
  return () => clearTimeout(timer); // Cleanup timer
}, []);
```
