# Felan

<p align="center">
  <a href="https://github.com/A404M/felan">
    <img src="logo.png" width="400" alt="Felan logo">
  </a>
</p>

A strongly statically typed programming language with a simple and friendly syntax. The language has been influenced by many other languages to be more user friendly both for new programmers who don't have many years of programming with other languages and those who want a powerful tool for programming.

# Features
* Powerful type system
* Compile time execution
* Operator overloading
* Function overloading
* Generic programming
* Meta programming
* Low level access
* Pointer
* ...

# Requirement
* Linux or *nix like OS
* git
* gnu make
* gcc (you can use clang or tcc but you have to change make for it)
* libffi (for calling C functions)

# How to use ?
Clone the repo
```
git clone https://github.com/A404M/felan
cd felan
```
Building
```
make
```
Run ./build/felan with any file which you want to run
```
./build/felan code/main.felan
```

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
* ~Add code injection~
* Add macro
* Add enum
* Compile AST to BBA
* Compile BBA to ASM
