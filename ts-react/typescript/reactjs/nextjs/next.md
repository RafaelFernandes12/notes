# How to build a Next.js app

Create the next application

```bash
npx create-next-app@latest
```

Add dependencies

```bash
yarn add <dependencies>@14.2.3 // change to the one you want
```

Remove dependencies

```bash
yarn remove <dependencie>
```

## Setup jest for testing

install the dependencies

```bash
yarn add -D jest jest-environment-jsdom @testing-library/react @testing-library/dom @testing-library/jest-dom ts-node @types/jest @types/testing-library__jest-dom @testing-library/react-hooks
```

```bash
yarn create jest
```

Create a jest.config.ts file at the route level

```typescriptreact
import type { Config } from 'jest'
import nextJest from 'next/jest.js'

const createJestConfig = nextJest({
  // Provide the path to your Next.js app to load next.config.js and .env files in your test environment
  dir: './',
})

// Add any custom config to be passed to Jest
const config: Config = {
  coverageProvider: 'v8',
  testEnvironment: 'jsdom',
  // Add more setup options before each test is run
  setupFilesAfterEnv: ['<rootDir>/jest.setup.ts'],
  moduleNameMapper: {
    '^@/(.*)$': '<rootDir>/src/$1',
  },
}

// createJestConfig is exported this way to ensure that next/jest can load the Next.js config which is async
export default createJestConfig(config)
```

Create a jest.setup.ts file at the route level

```typescriptreact
import '@testing-library/jest-dom'
```

Add scripts to the package.json

```typescriptreact
"test": "jest",
"test:watch": "jest --watch",
"test:cov": "jest --coverage"
```

## MSW

Install the dependencies

```bash
yarn add -D msw@latest
```

Paste this to src/mocks/node.ts

```tsx
import { setupServer } from "msw/node";
import { handlers } from "./handlers";

export const server = setupServer(...handlers);
```

paste this to src/mocks/handlers.ts

```tsx
import { http, HttpResponse } from "msw";

export const handlers = [
  // Intercept "GET https://example.com/user" requests...
  http.get("https://example.com/user", () => {
    // ...and respond to them using this JSON response.
    return HttpResponse.json({
      id: "c7b3d8e0-5e0b-4b0f-8b3a-3b9f4b3d3b3d",
      firstName: "John",
      lastName: "Maverick",
    });
  }),
];
```
