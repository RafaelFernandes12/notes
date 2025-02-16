# Regular Expressions (Regex) Documentation

Regular Expressions (Regex) are patterns used to match character combinations in strings. They provide powerful tools for searching, extracting, and manipulating text.
https://regexr.com/

## 1. Basic Anchors

- `^` - Matches the start of a string.
  ```ts
  /^abc/.test("abc"); // true
  /^abc/.test(" abc"); // false
  ```
- `$` - Matches the end of a string.
  ```ts
  /abc$/.test(" abc"); // false
  /abc$/.test("abc "); // true
  ```
- `^...$` - Matches the entire string.
  ```ts
  /^abc$/.test("abc"); // true
  /^abc$/.test(" abc"); // false
  /^abc$/.test("abc "); // false
  ```

## 2. Character Classes

- `.` - Matches any single character except newline.
  ```ts
  /.b./.test("abc"); // true
  /.b./.test("ac"); // false
  ```
- `\d` - Matches any digit (0-9).
  ```ts
  /\d/.test("abc123"); // true
  ```
- `\D` - Matches any non-digit character.
  ```ts
  /\D/.test("123"); // false
  ```
- `\w` - Matches any word character (letters, digits, underscore).
  ```ts
  /\w/.test("Hello"); // true
  ```
- `\W` - Matches any non word character (letters, digits, underscore).
  ```ts
  /\W/.test("Hello"); // false
  ```
- `\s` - Matches any whitespace character (spaces, tabs, newlines).
  ```ts
  /\s/.test("Hello World"); // true
  ```
- `\S` - Matches any non whitespace character (spaces, tabs, newlines).
  ```ts
  /\S/.test("Hello World"); // false
  ```

## 3. Quantifiers

- `*` - Matches 0 or more occurrences.
  ```ts
  /a*/.test("aaa"); // true
  /a*/.test("b"); // true (matches empty string)
  ```
- `+` - Matches 1 or more occurrences.
  ```ts
  /a+/.test("aaa"); // true
  /a+/.test("b"); // false
  ```
- `?` - Matches 0 or 1 occurrence.
  ```ts
  /a?b/.test("b"); // true
  /a?b/.test("ab"); // true
  ```
- `{n}` - Matches exactly `n` occurrences.
  ```ts
  /a{3}/.test("aaa"); // true
  /a{3}/.test("aa"); // false
  ```

## 4. Groups and Alternation

- `()` - Groups expressions.
  ```ts
  /(ab)+/.test("abab"); // true
  ```
- `|` - Acts as OR.
  ```ts
  /yes|no/.test("yes"); // true
  /yes|no/.test("maybe"); // false
  ```

## 5. Escape Characters

- `\.` - Matches a literal `.`.
  ```ts
  /a\.b/.test("a.b"); // true
  ```
- `\*`, `\?`, `\+`, `\(`, `\)`, etc., escape special characters.

## 6. Lookaheads and Lookbehinds

- `(?=...)` - Positive lookahead (matches if followed by ...).

  ```ts
  /a(?=b)/.test("ab"); // true
  /a(?=b)/.test("ac"); // false
  ```

  ```ts
  const bold = /(?<=\*)\b(.+?)\b(?=\*)/g;
  const italic = /(?<=_)\b(.+?)\b(?=_)/g;
  const underline = /(?<=~)\b(.+?)\b(?=~)/g;
  const text = "This is *bold*, _italic_, and ~underlined~ text.";

  // Extract bold text
  const boldMatches = text.match(bold);
  console.log("Bold:", boldMatches); // Output: ["bold"]

  // Extract italic text
  const italicMatches = text.match(italic);
  console.log("Italic:", italicMatches); // Output: ["italic"]

  // Extract underlined text
  const underlineMatches = text.match(underline);
  console.log("Underline:", underlineMatches); // Output: ["underlined"]
  ```

- `(?!...)` - Negative lookahead (matches if NOT followed by ...).
  ```ts
  /a(?!b)/.test("ac"); // true
  ```

## 7. Common Use Cases

- Match a valid email:
  ```ts
  /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
  ```
- Match a phone number (basic example):
  ```ts
  /^\d{3}-\d{3}-\d{4}$/;
  ```
- Match a URL:
  ```ts
  /^(https?:\/\/)?(www\.)?[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}(/;
  ```

Regex is a versatile tool used in validation, searching, and text processing. Mastering it can greatly enhance text manipulation capabilities.
