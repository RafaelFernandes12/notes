# twoChildren

here's how to create a component with two children

```tsx
import React from 'react';

type MyComponentProps = {
  firstChild: React.ReactNode;
  secondChild: React.ReactNode;
};

const MyComponent: React.FC<MyComponentProps> = ({ firstChild, secondChild }) => {
  return (
    <div className="my-component">
      <div className="first-child">{firstChild}</div>
      <div className="second-child">{secondChild}</div>
    </div>
  );
};

const App = () => {
  return (
    <MyComponent
      firstChild={<div>This is the First Child</div>}
      secondChild={<div>This is the Second Child</div>}
    />
  );
};

export default App;
``
