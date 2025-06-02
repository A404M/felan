# Felan
A strongly statically typed programming language with a simple and friendly syntax. The language has been influenced by many other languages to be more user friendly both for new programmers who don't have many years of programming with other languages and those who want a powerful tool for programming.

# Features
* Powerful type system
* Compile time execution
* Operator overloading
* Function overloading
* Generic programming
* Low level access
* Pointer
* ...

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

# TODO
* ~Overloading [] operator~
* Add code injection
* Add macro
* Add enum
* Compile AST to BBA
* Compile BBA to ASM
