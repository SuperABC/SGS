# SGS
SGS is one programming language which means SG Script for SGL. This programming language is designed specifically for SGL. In terms of the logic of this language, it is similar to cpp. The executable interpreter has packed the SGL file so no more configuration is needed.

The main properties in SGS is similar to JavaScript and C++, however, there are many syntax sugar in SGS.

### Syntax

#### Type System

SGS is a language with weak type constraint. That is, we need not to tell the interpreter the type explicitly. For programmer, we can regard all variables and function return values as arbitrary type.

#### Statement

##### Declare and Assign

```sgs
let test word.
let test word be "hello world!".
```

##### Function Call

```sgs
let test word be "hello world!".
print a line with content test word.
print a line with test word.
```

##### Conditional Branch

```sgs
let test branch be true.
if test branch is true then
	print a line with "true".
else print a line with "false".
end if.
```

##### Loop

```sgs
let rounds be 10.
loop when rounds > 0.
	let rounds be rounds - 1.
	print a line with rounds.
end loop.
```



#### Function

##### Function Declaration

```sgs
new function add with a, b.
```

##### Function Definition.

```sgs
start add.
	let result be a+b.
end add.
print a line with add with 1, 2.
```

##### Lambda Expression

```sgs
let fp be lambda with a, b
	let result be a+b.
end lambda.
print a line with fp with 1, 2.
```



#### Array and Class

##### Array Definition

```sgs
let arr be [].
arr's push 1.
arr's push "ok".
print a line with arr.
```

##### Class Definition.

```sgs
new class test with
	name,
	value.
let v be test{"sgs", 100}.
print a line with v.
```



### Cpp Generator

Sometimes we may find that the interpreter runs so slowly that we want to do compile and run. Then we can generate cpp code from sgs code.

When we do generate and compile, we can insert pure cpp code in sgs context.

```sgs
embed cpp
	int a, b;
	result = a+b;
end embed
```



