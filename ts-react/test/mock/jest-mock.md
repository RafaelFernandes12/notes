# jest.mock("module", function)

`jest.mock` is a high order function used to mock external modules or dependencies in a test file.
It replaces the module with a mock version of it.

## Example2 for example

```tsx
import { Button } from "@material-ui/core";
import { DataGrid } from "@material-ui/data-grid";
import React from "react";

export const columns = [
  { field: "id", headerName: "ID", width: 100 },
  {
    field: "firstName",
    headerName: "First name",
    width: 150,
    editable: true,
  },
  {
    field: "lastName",
    headerName: "Last name",
    width: 150,
    editable: true,
  },
  {
    field: "age",
    headerName: "Age",
    type: "number",
    width: 110,
    editable: true,
  },
];

export const rows = [
  { id: 1, lastName: "XXXXXXWWWWW", firstName: "Jon", age: 35 },
  { id: 2, lastName: "Lannister", firstName: "Cersei", age: 42 },
  { id: 3, lastName: "Lannister", firstName: "Jaime", age: 45 },
  { id: 4, lastName: "Stark", firstName: "Arya", age: 16 },
  { id: 5, lastName: "Targaryen", firstName: "Daenerys", age: null },
  { id: 6, lastName: "Melisandre", firstName: null, age: 150 },
  { id: 7, lastName: "Clifford", firstName: "Ferrara", age: 44 },
  { id: 8, lastName: "Frances", firstName: "Rossini", age: 36 },
  { id: 9, lastName: "Roxie", firstName: "Harvey", age: 65 },
];

export interface Example2Props {
  onMoney: (n: number) => void;
}

export function Example2({ onMoney }: Example2Props) {
  return (
    <div>
      <Button onClick={() => onMoney(33)}>Give me 33 dollars</Button>

      <div style={{ height: 400, width: "100%" }}>
        <DataGrid
          rows={rows}
          columns={columns}
          pageSize={5}
          checkboxSelection
        />
      </div>
    </div>
  );
}
```

```tsx
import { Example2, rows } from "@/lib/Example2";
import { fireEvent, render, screen } from "@testing-library/react";
import { DataGrid } from "@material-ui/data-grid";

jest.mock("@material-ui/data-grid", () => ({
  DataGrid: jest.fn(() => <div>Table</div>), // Now we are replacing the whole DataGrid component for this div
}));

const mockedGrid = jest.mocked(DataGrid);

describe("Example2", () => {
  afterEach(() => {
    jest.restoreAllMocks(); // resets all mocks
  });
  it("should call onMoney with 33", () => {
    const myOnMoney = jest.fn();
    render(<Example2 onMoney={myOnMoney} />);
    fireEvent.click(screen.getByRole("button", { name: "Give me 33 dollars" }));
    expect(myOnMoney).toHaveBeenCalledTimes(1);
    expect(myOnMoney).toHaveBeenCalledWith(33);
  });

  it("should render a table", () => {
    render(<Example2 onMoney={jest.fn()} />);
    expect(screen.getByText("Table")).toBeInTheDocument();
    expect(mockedGrid).toHaveBeenCalled();
    expect(mockedGrid).toHaveBeenCalledWith(
      {
        rows,
        columns: [
          expect.objectContaining({ field: "id" }), // We're checking if the columns[0] field has the value "id"
          expect.objectContaining({ field: "firstName" }), // We're checking if the columns[1] field has the value "firstName"
          expect.objectContaining({ field: "lastName" }), // We're checking if the columns[2] field has the value "lastName"
          expect.objectContaining({ field: "age" }), // We're checking if the columns[2] field has the value "age"
        ],
        pageSize: 5,
        checkboxSelection: true,
      },
      {},
    );
  });
});
```

### jest.mock()

Using this approach, we are mocking the whole module. Not Returning anything meaningful, to pass to the HTML.
This will return a error with the message `Matcher error: received value must be a mock or spy function`
We are passing a object, when we should pass HTML

```tsx
jest.mock("@material-ui/data-grid"); // this will mock the whole data-grid module
```

The correct approach would be this

```tsx
jest.mock("@material-ui/data-grid", () => ({
  DataGrid: jest.fn(() => <div>Table</div>),
}));
```

or simply this

```tsx
jest.mock("@material-ui/data-grid", () => ({
  DataGrid: jest.fn(),
}));
```

In the first way, we are replacing the DataGrid HTML to a simple div, that says "Table".
The second way also works for what we want. We are replacing the DataGrid component to nothing at all
This is fine, since we are not interested in testing the DataGrid, all we want from it, is to render it without crashing.

Then, why dont we just render the DataGrid itself? Instead of a div?

```tsx
jest.mock("@material-ui/data-grid", () => ({
    <DataGrid rows={rows} columns={columns} pageSize={5} checkboxSelection />
}));
```

This approach doesnt work, because we are creating a recursive loop, where the DataGrid component is calling itself endlessly.
This will cause a stack overflow error.

### jest.mocked()

The mockedGrid variable is the representation for the DataGrid component, we going to use it
to make all testings that we would need to do with the DataGrid component, expect that the HTML is just a div.

```tsx
expect(mockedGrid).toHaveBeenCalled();
expect(mockedGrid).toHaveBeenCalledWith(
  {
    rows,
    columns: [
      expect.objectContaining({ field: "id" }), // We're checking if the columns[0] field has the value "id"
      expect.objectContaining({ field: "firstName" }), // We're checking if the columns[1] field has the value "firstName"
      expect.objectContaining({ field: "lastName" }), // We're checking if the columns[2] field has the value "lastName"
      expect.objectContaining({ field: "age" }), // We're checking if the columns[2] field has the value "age"
    ],
    pageSize: 5,
    checkboxSelection: true,
  },
  {},
);
```

Pay attention that this is only possible by referencing the DataGrid component with the mockedGrid variable.

## Example3 Component

```tsx
import { Example2 } from "./Example2";

export function Example3() {
  return <Example2 onMoney={() => console.log(2)} />;
}
```

```tsx
import { Example2 } from "@/lib/Example2";
import { Example3 } from "@/lib/Example3";
import { render, screen } from "@testing-library/react";

jest.mock("../src/lib/Example2.tsx");
describe("Component", () => {
  test("it should", () => {
    jest.mocked(Example2).mockImplementation(() => <div>Table</div>);
    render(<Example3 />);
    expect(screen.getByText("Table")).toBeInTheDocument();
  });
  test("it should", () => {
    jest.mocked(Example2).mockImplementation(() => <div>teste</div>);
    render(<Example3 />);
    expect(screen.getByText("teste")).toBeInTheDocument();
  });
});
```

We can apply the same rules to our custom components.

### mockImplementation

This is another way of doing this

```tsx
jest.mock("../src/lib/Example2.tsx", () => ({
  Example2: jest.fn(() => <div>Table</div>),
}));
```

or this

```tsx
jest.mock("../src/lib/Example2.tsx", () => {
  Example2 = jest.fn(() => <div>Table</div>),
  return { Example2 };
});
```
# APICalls
```ts
yarn add -D msw@latest
```
