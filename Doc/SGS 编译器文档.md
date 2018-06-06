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

​	SGS语言为一款轻量级程序设计语言，具有很强的可移植性和可扩展性。

​	在编译器前端，SGS侧重编程语言与自然语言的相似度，使得SGS代码更接近于英语，从而可以让绝大多数人们可以理解其内部含义。在编译器后端，SGS侧重对象的处理与类型系统的维护。在SGS中，非基本类型的变量全部作为对象处理。这样的好处是避免了指针的使用，从而让SGS代码更加安全。

### SGS 语言语法

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