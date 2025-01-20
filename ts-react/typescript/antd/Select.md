# Select

## optionFilterProp

When i type in the select it will filter based on searchText value.
It only accepts string values.

```tsx
<Select
  showSearch
  mode="multiple"
  onSelect={onSelectSubmodel}
  onDeselect={onDeselectSubmodel}
  optionFilterProp="searchText"
  options={
    Array.isArray(submodels)
      ? submodels.map(({ header }) => ({
          label: createLabel(header),
          value: header.globalId || "",
          searchText: header.displayName,
        }))
      : []
  }
/>
```

## onSearch

The callback function triggered when input changed
(search: string) => void
The example below will output the search value in the console.

```tsx
<Select
  showSearch
  mode="multiple"
  onSelect={onSelectSubmodel}
  onDeselect={onDeselectSubmodel}
  onSearch={(e) => console.log(e)}
  optionFilterProp="searchText"
  options={
    Array.isArray(submodels)
      ? submodels.map(({ header }) => ({
          label: createLabel(header),
          value: header.globalId || "",
          searchText: header.displayName,
        }))
      : []
  }
/>
```
