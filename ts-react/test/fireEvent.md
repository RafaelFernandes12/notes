# fire Event

## change

Helps to change the value of the input field.

```typescriptreact
fireEvent.change(screen.getByRole("textbox"), {
  target: { value: "songName" },
});
expect(screen.getByRole("textbox")).toHaveValue("songName");
```
## click

Helps to click on the element.

```typescriptreact
fireEvent.click(screen.getByRole("button"));
```

## mouseDown

Simulate a mousedown event on the element. That is when you press the left side of the mouse
if you release the mouse button it will trigger the mouse up event.
The combination of these two events is used to trigger the click event.

```typescriptreact
