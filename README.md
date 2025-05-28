# Felan
A strongly statically typed programming language with a simple and friendly syntax

# Why Felan?
* Easy to understand
* Powerful type system
* Compile time execution

# Examples
## Hello world
```
@import("basic.felan");

main :: () -> void {
  print("Hello, world!");
};
```
## Printing
```
@import("basic.felan");

main :: () -> void {
  b := 1234;
  print(b);
  a : i8 = 32;
  print(a);
  print("This is a string");
};
```
