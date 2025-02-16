# Input

```tsx
type InputProps = {
  text?: string;
  className?: ClassNameValue;
} & React.InputHTMLAttributes<HTMLInputElement>;

function Input({ text, className, ...rest }: InputProps) {
  return (
    <div>
      {text && <span>{text}</span>}
      <input
        className={twMerge("bg-slate-100 p-2 text-3xl", className)}
        {...rest}
      />
    </div>
  );
}
```
