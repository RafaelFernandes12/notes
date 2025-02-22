# Regex

## Introduction

Regex is a form to match patterns in strings.

```ts
/cat/g
there was a `cat` in the street.
```

Another way to do this is by using the `|` operator.

### Quantifiers

- `+` match one or more ocurrences of the previous character

```ts
/e+/g
th`e`re was a `e` in th`e` str`ee`t, and a `ee` in th`e` hous`e`.
```

- `?` match zero or one ocurrences of the previous character

```ts
/ea?/g
th`e`r`e` was a cat s`ea`rching in th`e` str`e``e`t
```

- `*` match zero or more ocurrences of the previous character

```ts
/re*/g
the`r`e was a cat in the st`ree`t
```

- `{}` match a specific number of ocurrences

```ts
/e{2}/g // only 2
there was a cat in the str`ee`t, and a cat in the house.
/e{2,3}/g // 2 or 3
there was a cat in the str`ee`t, and a dog in the hous`eee`
/e{2,}/g // 2 or more
there was a cat in the str`ee`t, and a dog in the hous`eee`, and the hous`eeee`

```

- `.` match any character except newline

```ts
/.at/g
there was a `cat` in the street, and a `bat` in the house.
```

### Grouping

- `[]` match any character inside the brackets

```ts
/[cb]at/g
there was a `cat` in the street, and a `bat` in the house.
// or by using a range of characters
/[a-z]at/g
there was a `cat``bat``zat``rat``mat``nat``lat``pat``oat``kat``jat``iat``hat``gat``fat``eat``dat``cat``bat``aat`;
```

- `()` group characters

```ts
/(ca|ba)at/g
there was a `caat` in the street, and a `baat` in the house.
```

- `(?:...)` non-capturing group

```ts
const regex = /(?:foo|bar)baz/;

console.log(regex.test("foobaz")); // true
console.log(regex.test("barbaz")); // true
console.log(regex.test("foobar")); // false
```

- `|` - Acts as OR.

```ts
/(c|b)at/g
/cat|bat/g
there was a `cat` in the street, and a `bat` in the house.
```

### Anchors

- `^` match the start of a string

```ts
/^the/gm // the flag m is to match the start of each line, without it, it would only match the first line
`the`re was a cat in the street, and a cat in the house.
`the` monster is ugly
```

- `$` match the end of a string

```ts
/\.$/gm
there was a cat in the street, and a cat in the house of cat`.`
the monster is ugly as a cat`.`
```

- `\b` match a word boundary

```ts
/\b\a\b/g
`a` like for `a` life, am i alive?
```

### Special sequences

- `\d` match any digit

```ts
/\d/g
there was a cat in the street, and a cat in the house of `123`.
```

- `\D` match any non-digit

```ts
/\D/g
`there` `was` `a` `cat` `in` `the` `street`, `and` `a` `cat` `in` `the` `house` `of` 123
```

- `\w` match any word character

```ts
/\w/g
`there` `was` `a` `cat` `in` `the` `street`, `and` `a` `bat` `in` `the` `house`.
```

- `\W` match any non-word character

```ts
/\W/g
there `` was `` a `` cat`.` `123`
```

- `\s` match any whitespace character

```ts
/\s/g
there``was``a``cat. 123
```

- `\S` match any non whitespace character

```ts
/\S/g;
ignore`there``was``a``cat``.``123`;
```

### lookaround

- `(?<=)` positive lookbehind, matches the expression only if it is preceded by the expression inside the parentheses

```ts
/(?<=cat)erhina/g
i have a cat named cat`erhina`
```

- `(?<!)` negative lookbehind, matches the expression only if it is not preceded by the expression inside the parentheses

```ts
/(?<!R)afael/g
Rafael r`afael`
```

- `(?=)` positive lookahead, matches the expression only if it is followed by the expression inside the parentheses

```ts
/.(?=at)/g
i have a `f`at `c`at that i want to `e`at
```

- `(?!)` negative lookahead, matches the expression only if it is not followed by the expression inside the parentheses

```ts
/.(?!at)/g
`i` `h``a``v``e` `a` f`a``t` c`a``t` `t``h``a``t` `i` `w``a``n``t` `t``o` e`a``t`
```

### Flags

- g: global, match all ocurrences

```ts
/cat/g
there was a `cat` in the street, and a `cat` in the house.
/cat/
there was a `cat` in the street, and a cat in the house.
```

- i: case insensitive

```ts
/cat/gi
there was a `cat` in the street, and a `Cat` in the house.
```

- m: multiline, match the start of each line, usually used with anchors

```ts
/^the/gm
`the`re was a cat in the street, and a cat in the house.
`the` monster is ugly
```

### Lazy Matching

It's used to match the smallest possible string.

```ts
/<.*>/g
`<h1>title</h1>` // it'll match the whole string
/<.*?>/g
`<h1>`title</h1> // it'll match only the first tag

```

## Functions

### test

It's used to test if a string matches a pattern.

```ts
const regex = /cat/g;

regex.test("there was a cat in the street, and a cat in the house."); // true
regex.test("there was a dog in the street, and a dog in the house."); // false
```

### match

It's used to extract the matched pattern.

```ts
const regex = /cat/g;
const str = "there was a cat in the street, and a cat in the house.";
str.match(regex); // ['cat', 'cat']
```

### replace

it's used to replace the matched pattern.

```ts
const regex = /cat/g;
const str = "there was a cat in the street, and a cat in the house.";
str.replace(regex, "dog"); // "there was a dog in the street, and a dog in the house."
str.replace(regex, (match) => match.toUpperCase()); // "there was a CAT in the street, and a DOG in the house."
const str = "Hello, world!";
const newStr = str.replace(/(H)(ello)/, (p, r) => {
  console.log(`p: ${p}`); // "Hello"
  console.log(`r: ${r}`); // "H"
  return `${r.toUpperCase()}${p.slice(1)}`;
});
console.log(newStr); // "HEllo, world!"
```

## Questions

### whats the diference of the positive lookahead and the non-capturing group?

The positive lookahead is used to match a pattern only if it is followed by another pattern, while the non-capturing group is used to group characters without capturing them.

```ts
/(?:abc)abc/g
`abcabc`
/(?=abc)abc/g
`abc``abc`
```
