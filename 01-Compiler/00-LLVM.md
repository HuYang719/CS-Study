# LLVM介绍

摘自LLVM项目的创始者Chris Lattner的介绍：[The Architecture of Open Source Applications: LLVM](http://www.aosabook.org/en/llvm.html)

### 什么是LLVM？
LLVM是一种编译器基础设施，以C++写成，包含一系列模块化的编译器组件和工具链，用来开发编译器前端和后端。它与其他编译器的主要不同在于内部采用的架构。

### LLVM产生的背景
在2000年前，开源的语言执行工具工呈现两级分化：要么是传统的静态编译器，例如GCC、Free Pascal、FreeBASIC，是一种较为庞大的执行工具，很难重用这些静态编译器的的语法分析器(parser)作为静态分析或者重构；另外一种则以解释器或者Just-In-Time (JIT) compiler的方式提供动态编译。但很少有语言实现工具可以同时支持这两种，即使有也很少开源。

LLVM的提出就是为了改变这样的情况，LLVM现在被广泛用作一种常见的基础架构用来实现大量的不同语言的动态或者静态编译器(例如GCC, Java, .NET, python, Ruby, Scheme, Haskell等等)。此外，LLVM还取代了大量的用于特殊目的的编译器，例如Apple的OpenGL Stack中的动态专门化引擎以及Adobe产品中的图像处理库。LLVM还被用于创建大量的新产品，最著名的是用于OpenCL GPU编程语言和runtime。

### 传统经典编译器的简单介绍

最常见的对传统静态语言编译器（例如大部分的C编译器）一般采用的是三段设计：前端、优化器、后段，参考下图。前端用于解析用户输入的源码，检查错误，建立特定语言的抽象语法树([Abstract Syntax Tree, AST](https://en.wikipedia.org/wiki/Abstract_syntax_tree))用于表示输入代码。AST可以选择性的转换为用于优化的新代码，优化器和后段会作用于这个代码。
![三段设计](Pictures/SimpleCompiler.png)

优化器一般会做大量的转换用于提高代码的实时性，例如减少不必要的计算，它多多少少是独立于语言和生成目标饿。后端（也被称为代码生成器，code generator）将代码映射到目标架构的指令集上。后端需要尽可能生成处能够利用目标架构的特殊优势的良好代码。编译器后端一般包括指令选择，寄存器分配和指令时序分配等。

这个三段模型通用适用于动态解释器和JIT编译器。Java虚拟机([Java Virtual Machine, JVM](https://en.wikipedia.org/wiki/Java_virtual_machine))也是这种模型的一种是心啊，其中Java bytecode用于前端和优化器的中间连接。

#### 三段式设计的优势
这种经典的传统三段式设计方法最大的优势在于编译器可以支持多种语言和架构。如果编译器使用一种常见的编码作为优化器的表征，那么前端可以采用任意的编程语言，后端可以作用于任意的目标架构，参见下图。
![RetargetableCompiler](Pictures/RetargetableCompiler.png)

在这种设计下，如果编译器要支持一种新的语言(例如BASIC)需要重新实现一个新的前端，但是当前的优化器和后端可以被重新使用。如果不采用这种三段模式，那么这些部分就无法被分割，实现一个对新编程语言的执行就需要从头做起，需要支持N个目标架构和M种编程语言将会需要N乘M个编译器。

另一个三段式设计方法的优点在于编译器可以服务于采用不同编程语言的各类程序员。对于一个开源项目来说，这意味着这将会有一个更大的群体和更多的贡献，能够进一步提高和增强现有的编译器。这也是为什么一些开源编译器服务于许多社区(例如GCC)都在尝试生成更好的机器码而不是单一化编译器(例如FreePASCAL)。

最后一个优点是这种分割方式能让前后端的开发人员很好的维护和增进自己的部分，属于松耦合。对于开源软件来说，松耦合可以帮助减少其他人员开发时的障碍。

### 现有编程语言的实现

尽管三段式设计的优势巨大，但在实际中基本不可能被完全实现。回顾从LLVM之前的开源语言实现，你会发现对Perl,Python,Ruby和Java的实现没有共享任何代码。更进一步，Glasgow Haskeel COmpiler (GHC)和FreeBASIC等项目可以重映射在不同的CPU上，但他们只能支持一种编程语言。

之前提到过，这里有三种成功实现了这种模型，第一种是个Java和.NET虚拟机。他们的系统提供了一种JIT编译器，支持runtime和一种定义好的bytecode格式。这意味着任何语言都可以编译成bytecode的格式并且在运行时利用优化器和JIT的优势。作为代价的是，这些实现基本无法提供对runtime的选择上的灵活性：他们都采用JIT编译，垃圾回收和专门的面向对象模型。这导致了当编译的语言不是很符合该模型时（例如C）只有部分优化后的性能。

另一个成功的案例可以说是最不幸的，但是也是一种重新使用编译器技术的常见方式：将输入的源代码翻译成C代码然后送入已有的C代码编译器。这允许重新使用优化器和代码生成器，有很好的灵活性来控制runtime，这种方式也非常方便前端执行者理解和维护。但不幸的是，采用这种方式会阻止有效的异常处理的实现，并且只提供了非常糟糕的debug过程，减慢了编译速度，并且当新的语言想要加入其他特点（C语言没有的）时容易出现问题。

最后一个对三段式模型的成功实现是[GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection)。GCC支持许多的前端和后端，并且有非常活跃和广泛的社区贡献者。GCC有一段很长的成为C编译器的历史，并且支持了多种架构，一些其他语言也依赖于GCC。渐渐的，GCC社区慢慢衍化出了一个更清晰的设计。GCC4.4，拥有一种对优化器的新的表示方式，与提到的三段式模型更加接近。

尽管上述三个案例都非常成功，但这三个方式都拥有非常大的使用局限性，因为他们被设计用于单个且庞大的应用。举个例子，现实中将GCC嵌入到其他应用，比如将GCC用作一个runtime/JIT编译器或者提取并重新使用GCC中的一部分基本是不可能的。想要使用GCC中C++的前端用于文件生成、重构、静态分析工具的开发者不得不将GCC用作一个巨大单一的应用来实现他们的想法，即很难抽取GCC中一部分用作他用。

这里有一些对GCC为什么很难被重新用做库的解释，包括GCC中对全局变量的滥用，没有仔细设计的数据结构，宏的应用等。最难以修复的问题是它最早被设计时产生的内部架构的问题。具体来说，GCC存在layering和抽象泄露(leaky abstractions)的问题：后端需要使用前端的AST来生成debug信息，而前端生成了后端的数据结构，因此整个编译器以来于许多全局结构。

**总结：三段式设计有重要的优势，但现实中三种成功实现并没有完全达到当初的目标。其中，GCC已经拥有了尽可能的前后端分离模式的设计，但依然存在前后端的大量耦合，因此给其他开发者的重用带来了极大的不便。**

### LLVM的代码表示：LLVM IR

在了解了历史背景后，现在让我们来探究LLVM吧！LLVM最重要的设计就是中间层表示，LLVM Intermediate Representation (IR), 这是一种在编译器中用来代表代码的形式。LLVM IR被设计用于处理中间层的表示和优化器中的转化。它最初设计时有许多特定的目标，包括支持轻量级的运行时优化，过程优化，全代码分析以及强大的重构转换等等。最重要的一个方面是它本身被定义为一流的语言(a first class language)拥有经过很好定义的语义符号。下面用一个例子来具体说明，这一个非常简单的.ll文件的例子：

```
define i32 @add1(i32 %a, i32 %b) {
	entry:
		%tmp1 = add i32 %a, %b
		ret i32 %tmp1
}

define i32 @add2(i32 %a, i32 %b) {
	entry:
		%tmp1 = icmp eq i32 %a, 0
		br i1 %tmp1, label %done, label %recurse

	recurse:
		%tmp2 = sub i32 %a, 1
		%tmp3 = add i32 %b, 1
		%tmp4 = call i32 @add2(i32 %tmp2, i32 %tmp3)
		ret i32 %tmp4

	done:
		ret i32 %b
}
```
上面这段LLVM IR代码与下面这段C代码相关，都表示了两种不同的整数求和方式：

```C
unsigned add1(unsigned a, unsigned b) {
	return a+b;
}

// Perhaps not the most efficient way to add two numbers.
unsigned add2(unsigned a, unsigned b) {
	if(a == 0) return b;
	return add2(a-1, b+1);
}
```
可以从这个例子中看到，LLVM IR是一种底层类似于RISC的虚拟指令集。和真正的RISC指令集很像，它支持一些简单的线性序列质量例如：加、减、比较、分支。这些指令有三种形式，意味着他们采用一定数量的输入并生成结果放入一个不同的寄存器中。LLVM IR支持labels并且看起来很像一种汇编语言的变体。

与大部分的RISC指令集不同，LLVM强调类型，拥有一种简单的类型系统(例如，i32时一种32bit的整型，i32\*\*是一个指针，指向32bit的整形)，还有一些机器的细节被抽象了。例如calling被抽象为call和ret指令和明确的参数。另一种与机器码明显的不同在于LLVM IR不会使用一个固定的寄存器集合，它可以使用无限的寄存器，用一个%开头表示。

不只是被当作一种语言实现，LLVM IR实际上被定义为三种同构的形式：上述的文本形式，可以被优化器修订的可审查的内存数据格式，一种有效密集型的二进制"bitcode"格式。LLVM项目也提供了工具将从文本格式转为二进制格式：llvm-as汇编文本格式.ll文件为.bc文件，.bc文件包含bitcode编码；而llvm-dis将.bc文件转为.ll文件。

LLVM IR，编译器的中间表示层是非常有意思的，因为它可以为编译器的优化器提供一个完美的世界：不像编译器的前端和后端，优化器不会被一种特定的语言或者架构所约束。另一方面，IR也必须服务好前后端：它必须设计的能够被前端很好的生成，也必须能够足够强大到让优化器针对现实中的架构去优化。

**总结：LLVM IR类似于RISC的虚拟指令，是编译器的中间层表示，服务于优化器。LLVM IR有三种等价形式，并可以相互转化。**

#### 动手写一个LLVM IR优化器

为了让读者对优化器如何工作有一个直观感受，我们一起来看一些例子。这里有许多不同类别的编译器优化器，所以很难提供一个能解决任意问题的方法。不过，大部分的优化器都遵循一个简单的三部分结构：
- 寻找到需要转化的结构
- 验证这个转化是否安全以及正确
- 完成转化，更新代码

最繁琐的优化器是对运算模式的识别，例如：对任意的整数X，X-X是0, X-0是X, (X\*2)-X是X。第一个问题是他们在LLVM IR中是什么样的，这里有一些例子供参考：
```
⋮    ⋮    ⋮
%example1 = sub i32 %a, %a
⋮    ⋮    ⋮
%example2 = sub i32 %b, 0
⋮    ⋮    ⋮
%tmp = mul i32 %c, 2
%example3 = sub i32 %tmp, %c
```
上面是对转换的简单的展示，LLVM提供了一个指令简化结构用于对其他更多更高等级转换的使用。这些特定的转换在SimplifySubInst函数中，形式如下：

```C
// X-0 -> X
if (match(Op1, m_Zero()))
	return Op0;

// X - X -> 0
if (Op0 == Op1)
	return Constant::getNullValue(Op0->getType());

// (X*2) - X -> X
if(match(Op0, m_Mul(m_Specific(Op1), m_ConstantInt<2>())))
	return Op1;

...

return 0; // Nothing matched, return null to indicate no transformation. 
```
在这个代码中，Op0和Op1都是一个整数相减指令的左右操作符。LLVM用C++执行，尽管C++并不是以模式匹配的能力出名(与一些功能函数语言相比，例如 Objective Caml)，但是它确实提供了一个一般性的系统让我们去实现。函数match()和函数m_允许我们在LLVM IR上执行一些公开的模式匹配运算。例如，m_Specific用于判断乘法左边运算符和Op1是否一致。

这三个例子都是模式匹配，如果匹配上，则函数返回替代的结果，如果没有匹配的则最后返回一个空指针。函数SimplifyInstruction的可以被不同的优化器调用，一个简单的驱使函数类似如下：

```C
for (BasicBlock::iterator I = BB->begin(), E = BB->end(); I != E; ++I)
	if (Value *V = SimplifyInstruction(I))
		I->replaceAllUsesWith(V);
```
这段代码对每一条指令进行简单的循环，检查它们是否可以进行简化。如果可以(因为SimplifyInstruction 返回非空)，它使用replaceAllUsesWith方法来更新代码中的指令，将其转化为更简单的形式。

**总结：LLVM IR优化器中会在SimplifySubInst函数中列出可简化的表达式，SimplifyInstruction用于对每一条指令检测，如果返回非空，则进行替换**




















