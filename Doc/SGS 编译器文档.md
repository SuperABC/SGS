# SGS 编译器文档

[TOC]

---
## 前言

​	

### 项目概览

```mermaid
graph TD
	subgraph FrontEnd
	A[SGS Code] ==>|Tokenizer| B[TokenStream]
	B ==>|Parser| C[AST1 : without type info]
    C -->|C-Codegen| C1[C code]
    C -->|AST-Printer| C2[text AST]
	end
	subgraph BackEnd
	C ==>|Transformer| D[AST2 : with type info]
	D ==>|CodeGenerator| E[LLVM IR]
	D -->|DotGenerator| F[DOT File]
	end
	subgraph LLVM
	E ==>|llc| G[Executable File] 
	E -->|lli| H[Interpreter]
	end
```





## SGS 语言手册

### SGS 语言简介

​	SGS语言为一款轻量级程序设计语言，具有很强的可移植性和可扩展性。目前开发团队已经完成 SGS 编译器及解释器，使其可以覆盖各类应用场景的需求。

​	在编译器前端，SGS 侧重编程语言与自然语言的相似度，使得 SGS 代码更接近于英语，从而可以让绝大多数人们可以理解其内部含义。在编译器后端，SGS 侧重对象的处理与类型系统的维护。在 SGS 中，非基本类型的变量全部作为对象处理。这样的好处是避免了指针的使用，从而让 SGS 代码更加安全。

### SGS 语言语法

SGS语法与英语有很大相似度。

- 变量声明与赋值语句：`let <type> <var> [be <exp>].`

  其中 `let` 和 `be` 为关键字（ `be` 及其后 `<exp>` 可省），`<type>`为变量类型，`<var>`为变量名字。若变量已声明过，则省略type。

- 函数声明语句：`new function <name> [with <type1> <para1>, <type2> <para 2>... ] return <type>. `

  其中 `new`、`function`、`with` 和 `return` 为关键字，`name` 为函数名，`<type1>`、`<para1>` 及其后的 `<type>` 和 `<para>` 为参数的类型和名字，若无参数，则省略 `with` 及其与 `return` 之间的内容。`<type>` 为函数返回值，若无返回值，则省略 `return` 及其后内容。

-  函数定义语句 `start <name>. <block>. end <name>.`

   其中 `start` 和 `end `为关键字，`<name>` 为函数名。在 `<block>` 中定义函数主体，SGS 默认为每个有返回值的函数定义一个名为 `result` 的局部变量。

- 类声明语句 `new class <name> with <type1> <para1>[, <type2> <para2>...]` 

  其中 `new` 、`class` 和 `with` 为关键字，`<name>` 为类名，`type1` 、`para1` 及其后的 `type2` 和 `para2` 为内部成员变量的类型和名字。

- 函数调用语句 `<func> with <para1> <exp1>, <para2> <exp2> ...`  , `<func> with <exp1>, <exp2>...`

  其中 `with` 为关键字，`<func>` 为函数名。SGS 函数调用支持两种方式：显式参数传递与隐式参数传递。第一种为显式参数传递，`para1` 为形参，`exp1` 为实参，此种传参方式支持乱序，即参数顺序不必与声明顺序一致。第二种为隐式参数传递，`exp1` 位实参，此种方式与 C 类似，参数顺序必须与声明顺序一致。

- 

- 

- 

### SGS 语言语义

## 编译器设计

### 通用部分

#### AST1

#### 类型系统实现与 AST2

### 编译器前端

#### 词法分析

#### 语法分析


 ### 编译器后端

#### 语义分析

#### 代码生成

### 辅助工具

#### 命令行 AST 输出

#### 生成可执行 C 代码

#### 生成 DOT 文件得到 AST2 图形显示

## 测试

### 简单测试

### 复杂测试

## 后记

### 分工

### 额外说明

