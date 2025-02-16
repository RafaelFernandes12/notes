# style

Create style for html elements.

```css
.mainHeader {
  background-color: #fff;
  display: flex;
  justify-content: space-between;
  padding: 0 24px !important;
  align-items: center;
  background-color: #fff;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.15);
  z-index: 1;
}
```

translate to this

```ts
const mainHeader = {
  backgroundColor: "#fff",
  display: "flex",
  justifyContent: "space-between",
  padding: "0 24px !important",
  alignItems: "center",
  boxShadow: "0 2px 8px rgba(0, 0, 0, 0.15)",
  zIndex: "1",
};
```

To apply the ts style to an element, you can use the following code:

```tsx
<div style={mainHeader}>Hello World</div>
```

To apply the css style to an element, you can use the following code:

```tsx
import style "./style.css";
<div className={style.mainHeader}>Hello World</div>
```

## twMerge

```tsx
import { ClassNameValue, twMerge } from "tailwind-merge";
interface SongCardProps {
  className?: ClassNameValue;
  children: React.ReactNode;
}

export function Root(props: SongCardProps) {
  return (
    <div className="flex w-full items-center rounded bg-blueButton px-3 py-4">
      <div className={twMerge(``, props.className)} data-testid="container">
        {props.children}
      </div>
    </div>
  );
}
```
