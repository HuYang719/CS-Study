# C++语法知识回顾
太长时间只用C+STL了，记录一下项目中比较细节的知识点，会抠的比较细很基础。
如果非常熟悉C++，请跳过该部分。

## C++代码结构
#### #include "" 和#include <>的区别

- 系统自带的头文件用尖括号括起来，这样编译器会在系统文件目录下查找。 

- 用户自定义的文件用双引号括起来，编译器首先会在用户目录下查找，然后在到C++安装目录（比如VC中可以指定和修改库文件查找路径，Unix和Linux中可以通过环境变量来设定）中查找，最后在系统文件中查找。 

- 无论这个文件是C++提供的还是自己编写的，使用#include "文件名"命令一定是正确的。

## C++函数及语法

#### getchar()函数？
从stdin中读取一个字符，失败则返回EOF。

EOF是<stdio.h>中定义的用于表示文件结尾的int类型的负值整数常量表达式。

#### unique_ptr是？
std::unique_ptr 是通过指针占有并管理另一对象，并在 unique_ptr 离开作用域时释放该对象的智能指针。 

#### make_unique是：
构造 T 类型对象并将其包装进 std::unique_ptr。

#### static的用法？
[参考](https://zhuanlan.zhihu.com/p/37439983)

#### extern用法？

#### virtual用法？

## class的用法
[参考](https://www.runoob.com/cplusplus/cpp-classes-objects.html)

最简单的定义Box数据类型:
```cpp
class Box
{
   public:
      double length;   // 盒子的长度
      double breadth;  // 盒子的宽度
      double height;   // 盒子的高度
};
```
定义对象：
```cpp
Box box1;
Box box2;
```
使用public类型：
```cpp
box1.lenght = 10.0;
```
可以在类中定义函数:

```cpp
class Box
{
   public:
      double length;      // 长度
      double breadth;     // 宽度
      double height;      // 高度
   
      double getVolume(void)
      {
         return length * breadth * height;
      }
};
```
或在外部用范围解析运算符::定义该函数：
```cpp
class Box
{
   public:
      double length;         // 长度
      double breadth;        // 宽度
      double height;         // 高度
      double getVolume(void);// 返回体积
};

double Box::getVolume(void)
{
    return length * breadth * height;
}
```

三种修饰符：public,private,protected:
- public: 类的外部可以使用，以上例子中皆采用public
- private: 私有成员，类和友元函数可以访问私有成员，默认下为私有，通常的用法是将数据设为私有，通过public函数调用数据，举例如下：
```cpp 
class Box
{
   public:
      double length;
      void setWidth( double wid );
      double getWidth( void );
 
   private:
      double width;
};
 
// 成员函数定义
double Box::getWidth(void)
{
    return width ;
}
 
void Box::setWidth( double wid )
{
    width = wid;
}
 
// 程序的主函数
int main( )
{
   Box box;
 
   // 不使用成员函数设置长度
   box.length = 10.0; // OK: 因为 length 是公有的
   cout << "Length of box : " << box.length <<endl;
 
   // 不使用成员函数设置宽度
   // box.width = 10.0; // Error: 因为 width 是私有的
   box.setWidth(10.0);  // 使用成员函数设置宽度
   cout << "Width of box : " << box.getWidth() <<endl;
 
   return 0;
}
```
- protected: 保护成员变量或函数与私有成员十分相似，但有一点不同，保护成员在派生类（即子类）中是可访问的。

构造函数和析构函数：
- 构造函数可用于为某些成员变量设置初始值，**名称和类一致**
```cpp
class Line
{
   public:
      void setLength( double len );
      double getLength( void );
      Line();  // 这是构造函数
 
   private:
      double length;
};
 
// 成员函数定义，包括构造函数
Line::Line(void)
{
    cout << "Object is being created" << endl;
}
```
默认的构造函数没有任何参数，但如果需要，构造函数也可以带有参数。这样在创建对象时就会给对象赋初始值，如下面的例子所示：
```cpp
class Line
{
   public:
      void setLength( double len );
      double getLength( void );
      Line(double len);  // 这是构造函数
 
   private:
      double length;
};
 
// 成员函数定义，包括构造函数
Line::Line( double len)
{
    cout << "Object is being created, length = " << len << endl;
    length = len;
}

Line line(10.0);

```

**也可使用初始化列表来初始化字段**
```cpp
Line::Line( double len): length(len)
{
    cout << "Object is being created, length = " << len << endl;
}

//等同于

Line::Line( double len)
{
    length = len;
    cout << "Object is being created, length = " << len << endl;
}

```

- 析构函数：加～作为前缀，不返回任何值，也**不可带有任何参数**，用于释放资源
```cpp
class Line
{
   public:
      void setLength( double len );
      double getLength( void );
      Line();   // 这是构造函数声明
      ~Line();  // 这是析构函数声明
 
   private:
      double length;
};
 
// 成员函数定义，包括构造函数
Line::Line(void)
{
    cout << "Object is being created" << endl;
}
Line::~Line(void)
{
    cout << "Object is being deleted" << endl;
}
```