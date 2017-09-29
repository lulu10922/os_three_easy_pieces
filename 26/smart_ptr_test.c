#include <iostream>
#include <memory>
#include <boost/smart_ptr.hpp>

class Simple {
 public:
  Simple(int param = 0) {
    number = param;
    std::cout << "Simple: " << number << std::endl;  
  }
  ~Simple() {
    std::cout << "~Simple: " << number << std::endl;
  }
  void PrintSomething() {
    std::cout << "PrintSomething: " << info_extend.c_str() << std::endl;
  }
  std::string info_extend;
  int number;
};
//==============auto_ptr============
void TestAutoPtr()
{
    std::auto_ptr<Simple> my_memory(new Simple(1));
    if (my_memory.get())
    {
		my_memory->PrintSomething();
//        my_memory.get()->info_extend = "Addition";
        my_memory->info_extend = "Addition";     //what is the difference between "my_memory->" and "my_memory.get()->", may be the former is
                                                 //operator overload, the latter is directly using the pointer.
		my_memory->PrintSomething();
        (*my_memory).info_extend += " other";
		my_memory->PrintSomething();
    }
}

//1 auto_ptr不应该使用赋值运算符 = 
void TestAutoPtr2()
{
    std::auto_ptr<Simple> my_memory(new Simple(1));
    std::auto_ptr<Simple> my_memory2;
    if (my_memory.get())
    {
		my_memory2 = my_memory;
        my_memory2->PrintSomething();
        my_memory->PrintSomething();        //result in core dump, because my_memory became a dangling pointer.
    }
}

//2 内存泄漏，release仅仅是交出内存的所有权
void TestAutoPtr3()
{
    std::auto_ptr<Simple> my_memory(new Simple(1));
    if (my_memory.get())
    {
		my_memory.release();
    }
}

//================scoped_ptr==============
//没有赋值运算符，没有release成员函数的智能指针
void TestScopedPtr() {
  boost::scoped_ptr<Simple> my_memory(new Simple(1));
  if (my_memory.get()) {
    my_memory->PrintSomething();
    my_memory.get()->info_extend = "Addition";
    my_memory->PrintSomething();
    (*my_memory).info_extend += " other";
    my_memory->PrintSomething();
	
    //my_memory.release();           // 编译 error: scoped_ptr 没有 release 函数
    //std::auto_ptr<Simple> my_memory2;
    //my_memory2 = my_memory;        // 编译 error: scoped_ptr 没有重载 operator=，不会导致所有权转移
  }
}

//================shared_ptr=================
//可以复制，内部使用引用计数
void TestSharedPtr(boost::shared_ptr<Simple> memory) {  // 注意：无需使用 reference (或 const reference),如果使用reference，计数不会增加
  memory->PrintSomething();
  std::cout << "TestSharedPtr UseCount: " << memory.use_count() << std::endl;
}

void TestSharedPtr2() {
  boost::shared_ptr<Simple> my_memory(new Simple(1));
  if (my_memory.get()) {
    my_memory->PrintSomething();
    my_memory.get()->info_extend = "Addition";
    my_memory->PrintSomething();
    (*my_memory).info_extend += " other";
    my_memory->PrintSomething();
  }

  std::cout << "TestSharedPtr2 UseCount: " << my_memory.use_count() << std::endl;
  TestSharedPtr(my_memory);
  std::cout << "TestSharedPtr2 UseCount: " << my_memory.use_count() << std::endl;
  //my_memory.release();// 编译 error: 同样，shared_ptr 也没有 release 函数
}

//===================scoped_array=============
//类似于scoped_array，用于动态数组。没有重载operator *
void TestScopedArray() {
  boost::scoped_array<Simple> my_memory(new Simple[2]); // 使用内存数组来初始化
  if (my_memory.get()) {
	my_memory[0].PrintSomething();                      //使用的是.引用！！！
	my_memory.get()[0].info_extend = "Addition";
	my_memory[0].PrintSomething();
	//(*my_memory)[0].info_extend += " other";            // 编译 error，scoped_ptr 没有重载 operator*
	//my_memory[0].release();                             // 同上，没有 release 函数
	boost::scoped_array<Simple> my_memory2;
	//my_memory2 = my_memory;                             // 编译 error，同上，没有重载 operator=
  }
}

//====================shared_array=================
//
void TestSharedArray(boost::shared_array<Simple> memory) {  // 注意：无需使用 reference (或 const reference)
  std::cout << "TestSharedArray UseCount: " << memory.use_count() << std::endl;
}

void TestSharedArray2() {
  boost::shared_array<Simple> my_memory(new Simple[2]);
  if (my_memory.get()) {
    my_memory[0].PrintSomething();
    my_memory.get()[0].info_extend = "Addition 00";
    my_memory[0].PrintSomething();
    my_memory[1].PrintSomething();
    my_memory.get()[1].info_extend = "Addition 11";
    my_memory[1].PrintSomething();
    //(*my_memory)[0].info_extend += " other";  // 编译 error，scoped_ptr 没有重载 operator*
  }
  std::cout << "TestSharedArray2 UseCount: " << my_memory.use_count() << std::endl;
  TestSharedArray(my_memory);
  std::cout << "TestSharedArray2 UseCount: " << my_memory.use_count() << std::endl;
}

//=====================weak_ptr===============
//boost::weak_ptr 是专门为 boost::shared_ptr 而准备的。有时候，我们只关心能否使用对象，并不关心内部的引用计数。
//boost::weak_ptr 是 boost::shared_ptr 的观察者（Observer）对象，观察者意味着 boost::weak_ptr 只对 boost::shared_ptr
//进行引用，而不改变其引用计数，当被观察的 boost::shared_ptr 失效后，相应的 boost::weak_ptr 也相应失效。

//boost::weak_ptr 主要用在软件架构设计中，可以在基类（此处的基类并非抽象基类，而是指继承于抽象基类的虚基类）中定义一个
//boost::weak_ptr，用于指向子类的 boost::shared_ptr，这样基类仅仅观察自己的 boost::weak_ptr
//是否为空就知道子类有没对自己赋值了，而不用影响子类 boost::shared_ptr 的引用计数，
void TestWeakPtr() {
  boost::weak_ptr<Simple> my_memory_weak;
  boost::shared_ptr<Simple> my_memory(new Simple(1));
  std::cout << "TestWeakPtr boost::shared_ptr UseCount: " << my_memory.use_count() << std::endl;
  my_memory_weak = my_memory;
  std::cout << "TestWeakPtr boost::shared_ptr UseCount: " << my_memory.use_count() << std::endl;
}

int main(void)
{
    //TestAutoPtr3();
	//TestScopedPtr();
	TestSharedPtr2();
	//TestScopedArray();
	//TestSharedArray2();
	//TestWeakPtr();
    return 0;
}

