# spyOn

it's a function similar to `jest.fn()`, but it also tracks calls to the object's methods.
`jest.spyOn(object, methodName);
`

## example

```tsx
export function randomBetween(min: number, max: number): number {
  return Math.floor(Math.random() * (max - min + 1) + min);
}
```

```tsx
const spy = jest.spyOn(Math, "random");

desc("randomBetween", () => {
  it("should return a number between min and max", () => {
    spy.mockReturnValue(0);
    expect(randomBetween(3, 5)).toBe(3);
  });
  it("should return a number between min and max", () => {
    spy.mockReturnValue(0.5);
    expect(randomBetween(3, 5)).toBe(3);
  });
});
```

- The mockReturnValue is used to set the return value of the spy.
- In this case, we are returning the value for that especific test. If we dont't mind returning the same value
  for every test, we can set the return globally like this `jest.spyOn(Math, "random").mockReturnValue(0.5);`

```tsx
const spy = jest.spyOn(Math, "random");

desc("randomBetween", () => {
  it("should return a number between min and max", () => {
    spy.mockReturnValue(0);
    expect(randomBetween(3, 5)).toBe(3);
    expect(spy).toHaveBeenCalled(1);
  });
  it("should return a number between min and max", () => {
    spy.mockReturnValue(0.5);
    expect(randomBetween(3, 5)).toBe(3);
    expect(spy).toHaveBeenCalled(2);
  });
});
```

Both tests will fail, because the spy was called twice.
To fix this issue, we need to reset the spy after each test.

```tsx
const spy = jest.spyOn(Math, "random");

desc("randomBetween", () => {
  afterEach(() => {
    jest.restoreAllMocks(); // resets all mocks
    spy.mockClear(); // resets only the spy
  });
  it("should return a number between min and max", () => {
    spy.mockReturnValue(0);
    expect(randomBetween(3, 5)).toBe(3);
    expect(spy).toHaveBeenCalled(1);
  });
  it("should return a number between min and max", () => {
    spy.mockReturnValue(0.5);
    expect(randomBetween(3, 5)).toBe(3);
    expect(spy).toHaveBeenCalled(2);
  });
});
```

## spy with jest.fn()

```tsx
function MyComponent({ onClick }: { onClick: () => void }) {
  return <button onClick={onClick}>Click me</button>;
}
```

```tsx
import { render, fireEvent, screen } from "@testing-library/react";

test("should call the onClick prop when button is clicked", () => {
  // Create a mock function
  const onClickMock = jest.fn();

  // Render the component with the mock function as a prop
  render(<MyComponent onClick={onClickMock} />);

  // Simulate a click event on the button
  fireEvent.click(screen.getByText("Click me"));

  // Assert that the mock function was called
  expect(onClickMock).toHaveBeenCalledTimes(1);
});
```
