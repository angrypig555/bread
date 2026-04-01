# The bread programming language

-----

### Compilation Process

Bread is transpiled into c++, and compiled from there with g++/clang.

So, the stack would be:

``uncompiled.bread -> ./bread -> g++/clang -> ./compiled.out``

-----

### Basic commands
`print/Hello World!` - Prints hello world, No quotes required! If you want to print a string or integer or boolean, you just have to write the name of the string, boolean or integer.

-----
### Defining Variables

In bread, every integer, boolean and string must be defined before being used.

`int/foo/5` - Define an integer (example: foo with the value of 5)

`bol/foo/true` - define a boolean (example: foo = true) When printing booleans, they will be displayed as 0 or 1 based on if its false or true.

`str/foo/bar` - define a string (example: foo = bar)

`in/foo` - get an input from the user that will be stored in a string (example: input will be stored in foo) If an input string has not been already defined, one will be defined for you.

To initialize an empty variable of any kind, define it without any value and dont put a slash next to it.
For example, ``bol/foo``.

-----

### Exit codes
At the end of the code, a exit/0 will automatically be added on the compiler level.
If you want your code to exit, use the `exit/xxx` function, where ``xxx`` is the **exit code**.

> In general, anything above 0 is an error - for most programs.

-----

### If statements
To declare an if statement, use the if function like this:

```
if/foo/equals/bar
    print/foo equals bar!
    exit/1
endif/
```
> Tabs are optional.

For now, all variables in an if statement have to be declared as a variable before it.

-----

### While statements
To declare a while statement, you can use the built in while function like this:
```
while/foo/true
    print/foo is true
    bol/foo/false
endwhile/
```

-----

### Mathematics
There are 4 basic maths functions, which are:
- `add/foo/4` - Addition. Example: Adds 4 to foo
- `sub/foo/4` - Subtraction. Example: Substracts 4 from foo
- `mul/foo/4` - Multiplication. Example: Multiplies foo by 4
- `div/foo/4` - Division. Example: Divides foo by 4
- `mod/foo/4` - Modulo. Example: Mod foo by 4

These are only just examples, but the numbers can be replaced with an integer.

------
### Other functions
- `wait/5` - Stop the program for X second (5 for example)
-----
### Functions
In bread, functions are shortcuts.
Shortcuts always have to be defined at the top of the .bread file and must start with
`shrtct/shortcutname`
and end with
`endshrtrct/`
Currently you cannot pass anything inside the shortcut, if you want to use a variable inside the shortcut you have to define it before using it in the shortcut or else it will only be local to that shortcut.
You do not need to add tabs while defining what a shortcut does.
Here is an example of a shortcut.
```
shrtct/foo
    print/Im a shortcut!
    int/ran/1
endshrtct/
```
A shortcut must be declared before being used.
It can be used like this (in this example, foo is the shortcut name):
```
code...
foo
more code...
```
Shortcuts should not exit, if yes they will lead to errors while compiling.

-----
### Example Programs

-----
```
print/Hello World!
```

Prints "Hello, World" - and exits with 0.

```
int/counter/0

while/counter/equals/10
    print/counter

    add/counter/1
```