# LLVM要点笔记

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

### LLVM与Clang的关系？
![对比关系](Pictures/LLVM_Clang_Compar.png)
