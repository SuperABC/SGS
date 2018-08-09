# SGS
SGS is one programming language which means SG Script for SGL. This programming language is designed specifically for SGL. In terms of the logic of this language, it is similar to cpp. The executable interpreter has packed the SGL file so no more configuration is needed.

The main properties in SGS is similar to JavaScript and C++, however, there are many syntax sugar in SGS.

### Syntax

#### Type System

SGS separate types into three groups. They are basic type, array type and class type. For basic type, there are bool, char, integer, float, string five types. Then for array type, we need to tell the element type. Last, for class type, we need to tell the type and name for every elements.

#### Statement

##### Declare and Assign

```sgs
let string test word.
let test word be "hello world!".

let string test word be "hello world!".

let string test one, test two, test three.
let test one, test two, test three be 1.

let string test one, test two, test three be 1, 2，3.
```

##### Function Call

```sgs
let string test word be "hello world!".
print str with content test word.
print str with test word.
```

##### Conditional Branch

```sgs
let bool test branch be true.
if test branch is true then
	print str with "true".
else print str with "false".
end.
```

##### Loop

```sgs
let integer rounds be 10.
loop when rounds > 0.
	let rounds be rounds - 1.
	print int with rounds.
	skip line.
end.
```



#### Function

##### Function Declaration

```sgs
new function add with integer a, integer b return integer.
```

##### Function Definition.

```sgs
start add.
	let result be a+b.
end add.
print int with add with 1, 2.
```

##### Lambda Expression

```sgs
let fp be lambda with a, b return integer
	let result be a+b.
end.
print int with fp with 1, 2.
```



#### Array and Class

##### Array Definition

```sgs
let integer arr.

arr's push 1.
arr's push 2.
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
end
```



