#include <iostream>
#include <memory>
#include <string>

struct Foo {
    ~Foo()  {} // this gives compile error with make_shared
    //~Foo() {} // this does not
};

int main(int argc, char** argv)
{
   // this works for both throwing and non-throwing
   std::shared_ptr<Foo> foo;
   // this works only for non-throwing
   foo = std::make_shared<Foo>();
   return 0;
}