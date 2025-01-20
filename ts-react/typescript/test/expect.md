# Expect

## toHaveAttribute

Test if there's a attribute in the tag

```typescriptreact
expect(link).toHaveAttribute('href', '/about');
```

## getByRole

Get the element by role
In this example we can get the element by its role and children text

```typescriptreact
<Link to="/about"><span>About</span></Link>
<Link to="/home"><span>Home</span></Link>

const about = screen.getByRole('link', {name: 'About'});
const home = screen.getByRole('link', {name: 'Home'});
```

## not.toBeInTheDocument

when trying to assert if something is not in the document, dont use getBy* or findBy*
They will most likely return that the search is undefined or a empty object.

```typescriptreact
expect(screen.queryByTestId("albumLink")).not.toBeInTheDocument()
```

## toBe

when trying to assert if something is something, use expect().toBe()

```typescriptreact
expect(true).toBe(true) // true
expect('marmelada').toBe('merda') // false
expect(10).toBe(10) // true

```

## toHaveBeenCalledWith

when you want to check if a function was called with a certain argument

```typescriptreact
const myFunction = jest.fn();
myFunction(10);
expect(myFunction).toHaveBeenCalledWith(10);
```

```typescriptreact
const rows = [
  { id: 1, firstName: "John", lastName: "Doe", age: 33 },
  { id: 2, firstName: "Jane", lastName: "Doe", age: 33 },
  { id: 3, firstName: "John", lastName: "Smith", age: 33 },
  { id: 4, firstName: "Jane", lastName: "Smith", age: 33 },
];

  it("should render a table", () => {
    render(<Example2 onMoney={jest.fn()} />);
    expect(mockedGrid).toHaveBeenCalled();
    expect(mockedGrid).toHaveBeenLastCalledWith(
      {
        rows,
        columns: [
          expect.objectContaining({ field: "id" }),
          expect.objectContaining({ field: "firstName" }),
          expect.objectContaining({ field: "lastName" }),
          expect.objectContaining({ field: "age" }),
        ],
        pageSize: 5,
        checkboxSelection: true,
      },
      {},
    );
  });
```

## toHaveTextContent

```tsx
<span>test1, test2</span>;

expect(screen.getByText("test1, test2")).toHaveTextContent("test1, test2");
```
