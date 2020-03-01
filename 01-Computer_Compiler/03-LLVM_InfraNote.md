# LLVM要点笔记

## Project过程中的一些实际问题

### LLVM Installment
http://clang.llvm.org/get_started.html

命令:
```bash
cmake -DLLVM_ENABLE_PROJECTS=clang -G "Unix Makefiles" ../llvm 
```
安装后为debug版本，有20个G，可以通过以下命令安装release版本：
```bash
cmake -G "Unix Makefiles" \
>   -DCMAKE_BUILD_TYPE=Release \
>   -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
>   -DLLVM_BUILD_LLVM_DYLIB=ON \
>   --enable-optimized \
>   --enable-targets=host-only \
>   ../llvm
```
### 安装之后要将/build/bin加入path
根据官网安装之后，注意要add llvm to your path，否则会报错llvm-config not found
我的方法是vim /etc/paths，加入/llvm/build/bin的**绝对**路径。

![Ubuntu下安装以及链接方法可参考](https://www.isi.edu/~pedro/Teaching/CSCI565-Fall16/Projects/LLVM-Installation/LLVMInstructions-Ubuntu.pdf)

### 没有"llvm/ADT/STLExtras.h"头文件

```bash
clang++ -g -O3 toy.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o toy
```

### LLVM与Clang的关系？
![对比关系](Pictures/LLVM_Clang_Compar.png)


