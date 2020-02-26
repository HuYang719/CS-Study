# LLVM介绍

摘自LLVM项目的创始者Chris Lattner的介绍：[The Architecture of Open Source Applications: LLVM](http://www.aosabook.org/en/llvm.html)

### 什么LLVM？
LLVM是一种编译器基础设施，以C++写成，包含一系列模块化的编译器组件和工具链，用来开发编译器前端和后端。它与其他编译器的主要不同在于内部采用的架构。

### LLVM产生的背景
在2000年前，开源的语言执行工具工呈现两级分化：要么是传统的静态编译器，例如GCC、Free Pascal、FreeBASIC，是一种较为庞大的执行工具，很难重用这些静态编译器的的语法分析器(parser)作为静态分析或者重构；另外一种则以解释器或者Just-In-Time (JIT) compiler的方式提供动态编译。但很少有语言实现工具可以同时支持这两种，即使有也很少开源。

LLVM的提出就是为了改变这样的情况，LLVM现在被广泛用作一种常见的基础架构用来实现大量的不同语言的动态或者静态编译器(例如GCC, Java, .NET, python, Ruby, Scheme, Haskell等等)。此外，LLVM还取代了大量的用于特殊目的的编译器，例如Apple的OpenGL Stack中的动态专门化引擎以及Adobe产品中的图像处理库。LLVM还被用于创建大量的新产品，最著名的是用于OpenCL GPU编程语言和runtime。

### 传统经典编译器的简单介绍

最常见的对传统静态语言编译器（例如大部分的C编译器）一般采用的是三段设计：前端、优化器、后段，参考下图。前端用于解析用户输入的源码，检查错误，建立特定语言的抽象语法树([Abstract Syntax Tree, AST](https://en.wikipedia.org/wiki/Abstract_syntax_tree))用于表示输入代码。AST可以选择性的转换为用于优化的新代码，优化器和后段会作用于这个代码。
![三段设计](Pictures/SimpleCompiler.png)

优化器一般会做大量的转换用于提高代码的实时性，例如减少不必要的计算，它多多少少是独立于语言和生成目标饿。后端（也被称为代码生成器，code generator）将代码映射到目标架构的指令集上。后端需要尽可能生成处能够利用目标架构的特殊优势的良好代码。编译器后端一般包括指令选择，寄存器分配和指令时序分配等。

这个三段模型通用适用于动态解释器和JIT编译器。Java虚拟机([Java Virtual Machine, JVM](https://en.wikipedia.org/wiki/Java_virtual_machine))也是这种模型的一种是心啊，其中Java bytecode用于前端和优化器的中间连接。
