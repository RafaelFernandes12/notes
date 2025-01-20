# userEvent

## upload

```tsx
const input = screen.getByTestId("logoButton");
const file = new File(["dummy content"], "test-logo.png", {
  type: "image/png",
});
    await act(async () => {
      userEvent.upload(input, file);
    });

```
